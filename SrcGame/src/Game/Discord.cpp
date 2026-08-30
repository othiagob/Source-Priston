
#include "Discord.h"
#include <discord_rpc.h>
#include <chrono>
#include <field.h>
Discord Discord_Handle;

static int64_t StartTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();



char* szFields_discord[] = {
u8"Mata das Acácias",
u8"F. Bamboo",
u8"Jardim da Liberdade",
u8"Cidade de Ricarten",
u8"Refúgio dos A.",
u8"Castelo dos Perdidos",
u8"Vila Ruinen",
u8"Terra Maldita",
u8"Terra Esquecida",
u8"Cidade de Navisko",
u8"Oásis",
u8"C. B. A.",
u8"Terra Proibida",
u8"C. A. 1",
u8"C. A. 2",
u8"C. A. 3",
u8"Sala do GM",
u8"F. dos Espíritos",
u8"F. das Ilusões",
u8"Vale Tranquilo",
u8"E. dos Ventos",
u8"Cidade de Pillai",
u8"Templo Maldito 1",
u8"Templo Maldito 2",
u8"C. dos Cogumelos",
u8"Caverna das Abelhas",
u8"Santuário Sombrio",
u8"E. d. F. do Caos",
u8"Coração de Perum",
u8"Eura",
u8"S.o.D",
u8"Vale Galubia",
u8"Sala de Desafios",
u8"Castelo Abençoado",
u8"Lago da Ganância",
u8"Santuario Congelado",
u8"Covil do Kelvezu",
u8"Ilha Perdida",
u8"Templo Perdido",
u8"Mapa 2D",
u8"Torre sem Fim 1",
u8"Torre sem Fim 2",
u8"Templo Maldito 3",
u8"Torre sem Fim 3",
u8"Laboratório Secreto",
u8"Arma Antiga",
u8"Mina de Gelo 1",
u8"Arquipélago Perdido",
u8"Covil do Eragon",
u8"Arena de Batalha",
u8"Núcleo de Ferro",
u8"Abismo do Mar",
u8"Atlantis",
};
void Discord::Initialize()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    // https://discord.com/developers/applications  aplicação do discord, tem que criar 1 para colocar o numero da inicialização
    Discord_Initialize("898283856086597702", &handlers, 1, "0");
}
DiscordRichPresence discordPresence;
void Discord::Update(smCHAR* lpCurPlayer)
{
    if (std::is_null_pointer<decltype(lpCurPlayer)>::value)
        return;

    memset(&discordPresence, 0, sizeof(discordPresence));

    char buffer1[128];
	char buffer2[128];
	char buffer3[128];
	char buffer4[128];

    if (lpCurPlayer->PartyFlag) {
		wsprintf(buffer1, "Jogando em Grupo (%d/%d)", InterfaceParty.PartyMemberCount, PARTY_PLAYER_MAX);
    }
    else
		wsprintf(buffer1, "Jogando Solo");

	discordPresence.state = buffer1;

    wsprintf(buffer2, "%s", szFields_discord[StageField[lpCurPlayer->OnStageField]->FieldCode]);
    if (lstrcmp(buffer2, "") == 0)
        return;

    discordPresence.details = buffer2;


    discordPresence.startTimestamp = StartTime;

    
	discordPresence.largeImageKey = "photo";
    discordPresence.largeImageText = "Priston Tale";
    

	switch (lpCurPlayer->smCharInfo.JOB_CODE)
	{
	case JOBCODE_FIGHTER:

		wsprintf(buffer3, "fighter_class");

		break;

	case JOBCODE_MECHANICIAN:

		wsprintf(buffer3, "mechanician_class");

		break;

	case JOBCODE_ARCHER:

		wsprintf(buffer3, "archer_class");

		break;

	case JOBCODE_PIKEMAN:

		wsprintf(buffer3, "pikeman_class");

		break;

	case JOBCODE_ATALANTA:

		wsprintf(buffer3, "atalanta_class");

		break;

	case JOBCODE_KNIGHT:

		wsprintf(buffer3, "knight_class");

		break;

	case JOBCODE_MAGICIAN:

		wsprintf(buffer3, "magician_class");

		break;

	case JOBCODE_PRIESTESS:

		wsprintf(buffer3, "priestess_class");

		break;

	/*case JOBCODE_ASSASSINE:

		wsprintf(buffer3, "assassine_class");

		break;

	case JOBCODE_SHAMAN:

		wsprintf(buffer3, "shaman_class");

		break;*/
	}

    discordPresence.smallImageKey = buffer3;

	switch (lpCurPlayer->smCharInfo.JOB_CODE)
	{
	case JOBCODE_FIGHTER:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Lutador");

		break;

	case JOBCODE_MECHANICIAN:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Mecânico");

		break;

	case JOBCODE_ARCHER:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Arqueira");

		break;

	case JOBCODE_PIKEMAN:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Pike");

		break;

	case JOBCODE_ATALANTA:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Atalanta");

		break;

	case JOBCODE_KNIGHT:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Cavaleiro");

		break;

	case JOBCODE_MAGICIAN:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Mago");

		break;

	case JOBCODE_PRIESTESS:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Sacerdotisa");

		break;

	/*case JOBCODE_ASSASSINE:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Assassina");

		break;

	case JOBCODE_SHAMAN:

		wsprintf(buffer4, "%s | Lv. %d | %s", lpCurPlayer->smCharInfo.szName, lpCurPlayer->smCharInfo.Level, "Xama");

		break;*/
	}

	discordPresence.smallImageText = buffer4;

    discordPresence.instance = 1;

    Discord_UpdatePresence(&discordPresence);
}

void Discord::Shutdown()
{
    Discord_Shutdown();
    Discord_ClearPresence();
}