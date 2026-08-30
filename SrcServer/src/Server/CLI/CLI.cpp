#include <iostream>
#include "CLI.h"
#include <chrono>
#include <ctime>
#include <ratio>
#include <windows.h> // Para SetConsoleTextAttribute

#include "smwsock.h"
#include "SrcServer\onserver.h"
#include "netplay.h"
#include "smPacket.h"
#include "GM\GM.h"

extern int bla;  // No início do arquivo

using namespace std;
using std::chrono::system_clock;

CLI commandLine;

CLI::CLI()
{
	done = nullptr;
}

CLI::~CLI()
{
}

// Função para definir cor do texto no console
void SetConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Função para imprimir texto colorido
void PrintColored(const std::string& text, int color) {
    SetConsoleColor(color);
    std::cout << text;
    SetConsoleColor(7); // Reset para branco
}

// Função para imprimir o desenho ASCII do DRACO
void PrintDracoLogo() {
    SetConsoleColor(12); // Azul claro
    std::cout << "\n";
	// Arte ASCII DRACO
	SetConsoleColor(4); // Vermelho
	std::cout << R"(
    ============================================
    =                                          =
    =     DDDD  RRRR  AAAA  CCCC  OOOO       =
    =     D  D  R  R  A  A  C     O  O       =
    =     D  D  RRRR  AAAA  C     O  O       =
    =     D  D  R R   A  A  C     O  O       =
    =     DDDD  R  R  A  A  CCCC  OOOO       =
    =                                          =
    ============================================
    )" << std::endl;
    std::cout << "\n";
    SetConsoleColor(7); // Reset para branco
}

bool CLI::run(bool* done)
{
	this->done = done;

	std::string input;

	system_clock::time_point today = system_clock::now();
	std::time_t tt = system_clock::to_time_t(today);

	// Limpar tela
	system("cls");
	
	// Imprimir logo do DRACO
	PrintDracoLogo();
	
	// Cabeçalho colorido

		SetConsoleColor(10); // Verde
		std::cout << "================================================================\n";
		std::cout << "                    SERVIDOR WDPT CARREGOU                     \n";
		std::cout << "================================================================\n\n";

	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "The Server is ready!\n\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(14); // Amarelo
	std::cout << "Today is: " << ctime(&tt);
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";

	SetConsoleColor(13); // Magenta
	std::cout << "Version: 1.00.03 - WDPT YT01 - 100% Free!\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(12); // Azul claro
	std::cout << "Developer: Draco - PrimeRPG Corp Team\n\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(10); // Verde
	std::cout << "Type: 'help;' to see examples. Type 'exit;' to quit the server.\n\n";
	
	// Informações do servidor em uma caixa
	SetConsoleColor(8); // Cinza
	std::cout << "+----------------------------------------------------------------+\n";
	std::cout << "|                        SERVER INFO                           |\n";
	std::cout << "+----------------------------------------------------------------+\n";
	
	SetConsoleColor(7); // Branco
	std::cout << "| ";
	SetConsoleColor(11); // Ciano
	std::cout << ">> Server IP    : ";
	SetConsoleColor(15); // Branco brilhante
	std::cout << szDefaultServIP;
	SetConsoleColor(8); // Cinza
	std::cout << " |\n";
	
	SetConsoleColor(7); // Branco
	std::cout << "| ";
	SetConsoleColor(11); // Ciano
	std::cout << ">> Server Port  : ";
	SetConsoleColor(15); // Branco brilhante
	std::cout << TCP_SERVPORT;
	SetConsoleColor(8); // Cinza
	std::cout << " |\n";
	
	SetConsoleColor(7); // Branco
	std::cout << "| ";
	SetConsoleColor(11); // Ciano
	std::cout << ">> Server Name  : ";
	SetConsoleColor(15); // Branco brilhante
	std::cout << "Files Draco - WhatsApp: +55 (11)99826-9982 - Instagram: marcus.perciani";
	SetConsoleColor(8); // Cinza
	std::cout << " |\n";
	
	SetConsoleColor(7); // Branco
	std::cout << "| ";
	SetConsoleColor(11); // Ciano
	std::cout << ">> Copyright    : ";
	SetConsoleColor(15); // Branco brilhante
	std::cout << "Draco - cloudice.com.br";
	SetConsoleColor(8); // Cinza
	std::cout << " |\n";
	
	SetConsoleColor(8); // Cinza
	std::cout << "+----------------------------------------------------------------+\n\n";
	
	SetConsoleColor(7); // Reset para branco

	while (!(*this->done))
	{
		SetConsoleColor(12); // Azul claro
		std::cout << "DRACO> ";
		SetConsoleColor(7); // Branco
		std::cin >> input;

		if (input.back() != ';')
		{
			SetConsoleColor(12); // Azul claro
			std::cout << "! ";
			SetConsoleColor(14); // Amarelo
			std::cout << "Commands ends with ;\n";
			SetConsoleColor(7); // Branco
		}
		else
		{
			process(input);
		}
	}

	return true;
}

bool CLI::process(const std::string& cmd)
{

	if (cmd.compare("help;") == 0)
		handleHelpCommand();
	else if (cmd.compare("exit;") == 0)
		handleExitCommand();
	else if (cmd.compare("shutdown;") == 0)
		handleShutdownCommand();
	else if (cmd.compare("clear;") == 0)
		system("cls");
	else if (cmd.compare("connected;") == 0)
		handleConnectedCommand();
	else if (cmd.compare(0, 4, "kick") == 0)
		handleKickCommand(cmd);
	else if (cmd.compare("reloadGMS;") == 0)
		handleReloadGMSCommand();
	else if (cmd.compare("showGMS;") == 0)
		handlePrintGMSCommand();
	else if (cmd.compare("reloadConfig;") == 0)
		handleReloadConfigCommand();
	else {
		SetConsoleColor(12); // Azul claro
		std::cout << ">> ";
		SetConsoleColor(14); // Amarelo
		std::cout << "Invalid command. ";
		SetConsoleColor(10); // Verde
		std::cout << "To see examples, type 'help;'\n";
		SetConsoleColor(7); // Reset para branco
	}
	return false;
}

void CLI::handleHelpCommand()
{
	SetConsoleColor(10); // Verde
	std::cout << "\n================================================================\n";
	std::cout << "                        COMANDOS DISPONÍVEIS                   \n";
	std::cout << "================================================================\n\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(14); // Amarelo
	std::cout << "Commands ends with ;\n\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "Command: exit; ";
	SetConsoleColor(8); // Cinza
	std::cout << "<Close the world>\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "Command: shutdown; ";
	SetConsoleColor(8); // Cinza
	std::cout << "<Close the world in 8 minutes>\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "Command: clear; ";
	SetConsoleColor(8); // Cinza
	std::cout << "<Clear CLI screen>\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "Command: connected; ";
	SetConsoleColor(8); // Cinza
	std::cout << "<Shows who's connected to the world>\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "Command: kick(player name); ";
	SetConsoleColor(8); // Cinza
	std::cout << "<Expel user from the world>\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "Command: reloadGMS; ";
	SetConsoleColor(8); // Cinza
	std::cout << "<Reload Game Masters from Database>\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "Command: showGMS; ";
	SetConsoleColor(8); // Cinza
	std::cout << "<Show active Game Masters>\n";
	
	SetConsoleColor(7); // Branco
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "Command: reloadConfig; ";
	SetConsoleColor(8); // Cinza
	std::cout << "<Reload the server settings>\n\n";
	
	SetConsoleColor(7); // Reset para branco
}

void CLI::handleExitCommand()
{
	*this->done = true;
}

void CLI::handleShutdownCommand()
{
	rsShutDown();
	SetConsoleColor(12); // Azul claro
	std::cout << ">> ";
	SetConsoleColor(14); // Amarelo
	std::cout << "The world will be closed in 8 minutes\n";
	SetConsoleColor(7); // Reset para branco
}

void CLI::handleConnectedCommand()
{
	int count = 0;

	SetConsoleColor(10); // Verde
	std::cout << "\n================================================================\n";
	std::cout << "                        USUÁRIOS CONECTADOS                       \n";
	std::cout << "================================================================\n\n";

	for (int cnt = 0; cnt < CONNECTMAX; cnt++)
	{
		if (rsPlayInfo[cnt].lpsmSock)
		{
			rsPLAYINFO& player = rsPlayInfo[cnt];

			if (player.lpsmSock)
			{
				SetConsoleColor(7); // Branco
				std::cout << ">> ";
				SetConsoleColor(11); // Ciano
				std::cout << "Id: " << player.szID;
				SetConsoleColor(7); // Branco
				std::cout << " - ";
				SetConsoleColor(14); // Amarelo
				std::cout << "Name: " << player.szName;
				SetConsoleColor(7); // Branco
				std::cout << " - ";
				SetConsoleColor(13); // Magenta
				std::cout << "IP: " << player.lpsmSock->szIPAddr;
				SetConsoleColor(7); // Branco
				std::cout << " - ";
				SetConsoleColor(12); // Azul claro
				std::cout << "Stage: " << player.Position.Area << "\n";
				count++;
			}
		}
	}

	if (count) {
		SetConsoleColor(10); // Verde
		std::cout << "\n>> ";
		SetConsoleColor(15); // Branco brilhante
		std::cout << "Connected users: " << count << "\n\n";
	} else {
		SetConsoleColor(12); // Azul claro
		std::cout << "\n>> ";
		SetConsoleColor(8); // Cinza
		std::cout << "No users connected at the moment.\n\n";
	}
	
	SetConsoleColor(7); // Reset para branco
}

void CLI::handleKickCommand(const std::string& cmd)
{
	if (cmd.length() < 6)
		return;

	std::string name;
	bool flag = false;

	for (auto it = cmd.begin() + 5; it != cmd.end(); ++it)
	{
		if (*it == ')')
		{
			flag = true;
			break;
		}

		name += *it;

	}

	if (!flag) return;

	rsPLAYINFO* player = FindUserFromName(const_cast<char*>(name.c_str()));

	if (!player)
	{
		player = FindUserFromName2(const_cast<char*>(name.c_str()));
	}

	if (player)
	{
		DisconnectUser(player->lpsmSock);
		SetConsoleColor(10); // Verde
		std::cout << ">> ";
		SetConsoleColor(11); // Ciano
		std::cout << "User '" << name << "' has been kicked from the server.\n";
		SetConsoleColor(7); // Reset para branco
	}
	else
	{
		SetConsoleColor(12); // Azul claro
		std::cout << ">> ";
		SetConsoleColor(14); // Amarelo
		std::cout << "User '" << name << "' not found or not connected.\n";
		SetConsoleColor(7); // Reset para branco
	}
}

void CLI::handlePrintGMSCommand()
{
	GameMasters::getInstance()->print();
}

void CLI::handleReloadGMSCommand()
{
	GameMasters::getInstance()->readFromDatabase();
	SetConsoleColor(10); // Verde
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "GMS reloaded successfully!\n";
	SetConsoleColor(7); // Reset para branco
}

void CLI::handleReloadConfigCommand()
{
	int rsRefreshConfig();

	rsRefreshConfig();
	
	SetConsoleColor(10); // Verde
	std::cout << ">> ";
	SetConsoleColor(11); // Ciano
	std::cout << "Settings reloaded successfully!\n";
	SetConsoleColor(7); // Reset para branco
}

CLI* CLI::getInstance()
{
	return &commandLine;
}
