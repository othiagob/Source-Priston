#include "Questions.h"
#include "..\\Database\SQLConnection.h"
#include <boost/algorithm/string.hpp>
#include "../Shop/NewShop.h"
#include <Chat/ChatServer.h>

extern int SendToAll(char* Buff, int Size);

using namespace Events;

std::string SelectedQuestion;
std::vector<std::string> Answer;
BOOL Setup = FALSE;

int EventStage = 0;
int MinStart = 0;
int rewardCoins = 0;

Questions::Questions()
{
}

Questions::~Questions()
{
}

void SendMessageToAll(std::string sender, std::string message)
{
	SERVERCHAT->SendChatAllEx(EChatColor::CHATCOLOR_Global, "%s> %s", sender.c_str(), message.c_str());
}

int Questions::getStage()
{
	return EventStage;
}

int Questions::setStage(int stage)
{
	EventStage = stage;

	if (!stage)
	{
		SendMessageToAll("Quiz", "Ninguém acertou dessa vez!");
	}

	return TRUE;
}

int Questions::getMinBegin()
{
	return MinStart;
}

int Questions::setMinBegin(int min)
{
	MinStart = min;

	return TRUE;
}

BOOL Questions::CheckTime(UINT Time1, UINT Time2)
{
	if (Time1 > Time2)
		Time2 = (Time2 - Time1) * -1;

	if (Time1 - Time2 >= 2) return TRUE;

	return FALSE;
}

BOOL Questions::CheckRandomQuestion(UINT ID)
{
	auto db = SQLConnection::GetConnection(DATABASEID_EventosDB);

	int isValid = 0;

	if (db && db->Open())
	{
		const char* const query = "SELECT QuestionNum FROM QuestionsList WHERE QuestionNum=? AND Active=1";

		if (db->Prepare(query))
		{
			db->BindInputParameter(&ID, 1, PARAMTYPE_Integer);

			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Integer, &isValid);
			}
		}

		db->Close();

		if (isValid)
			return TRUE;
	}

	return FALSE;
}

BOOL Questions::GetQuestion()
{
	auto db = SQLConnection::GetConnection(DATABASEID_EventosDB);
	int Count = 0, random = 0;

	char Question[256] = { 0 };
	char AnswerQuestion[256] = { 0 };

	SelectedQuestion.clear();
	Answer.clear();

	if (db && db->Open())
	{
		const char* const query = "SELECT * FROM QuestionsList WHERE Active=1";
		if (db->Prepare(query))
		{
			db->Execute(FALSE);

			while (db->NextRow())
			{
				Count++;
			}
		}

		db->Close();

		if (!Count) return FALSE;

		// Verifica se o numero gerado é de uma pergunta ativa, se não vai tentando até encontrar uma válida
		while (1)
		{
			if (Count > 200) { Count = 0; break; };

			srand(static_cast<unsigned long>(time(NULL)));
			random = rand() % Count + 1;
			if (CheckRandomQuestion(random)) break;
			Count++;
		}
	}

	if (db && db->Open())
	{
		const char* const query = "SELECT Question,Answer,Reward FROM QuestionsList WHERE Active=1 AND QuestionNum=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(&random, 1, PARAMTYPE_Integer);

			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_String, Question, sizeof(Question));
				db->GetData(2, PARAMTYPE_String, AnswerQuestion, sizeof(AnswerQuestion));
				db->GetData(3, PARAMTYPE_Integer, &rewardCoins);
			}

			SelectedQuestion = Question;
			Answer = split(AnswerQuestion, ',');

			char msg[32] = { 0 };
			sprintf_s(msg, sizeof(msg), "Valendo %d Coins", rewardCoins);

			SendMessageToAll("Quiz", msg);
			SendMessageToAll("Quiz", SelectedQuestion.c_str());

			// Status = Em progresso
			EventStage = Stage::Stage_InProgress;
		}

		db->Close();
		return TRUE;
	}

	return FALSE;
}

BOOL Questions::CheckAnswers(rsPLAYINFO* Player, std::string Message)
{
	if (EventStage == Stage::Stage_InProgress)
	{
		UINT uVecSize = Answer.size();

		for (UINT u = 0; u < uVecSize; u++)
		{
			if (boost::iequals(Answer[u].c_str(), Message))
			{
				EventStage = Stage::Stage_Finished;

				char msg[64] = { 0 };
				sprintf_s(msg, sizeof(msg), "Parabéns %s! Você foi o mais rápido.", Player->szName);
				SendMessageToAll("Quiz", msg);

				ZeroMemory(msg, sizeof(msg));

				sprintf_s(msg, sizeof(msg), "A resposta era: %s", Answer[u].c_str());
				SendMessageToAll("Quiz", msg);

				NewShop::GetInstance()->addCoinsToPlayer(Player, Player->szID, rewardCoins, 1, WhereCoinsComeFrom::FROM_QUESTIONS);

				return TRUE;
			}

		}
	}

	return FALSE;
}

BOOL Questions::MessageBegin()
{
	if (!Setup)
	{
		SendMessageToAll("Quiz", "1 minuto, prepare-se!");
		EventStage = Stage::Stage_ToBegin;
		return TRUE;
	}

	Setup = FALSE;

	return FALSE;
}



