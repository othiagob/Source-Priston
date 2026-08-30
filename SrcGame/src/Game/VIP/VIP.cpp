#include "..\\sinbaram\\sinLinkHeader.h"

extern int dm_SendTransDamage(smCHAR* lpChar, int PowParam1, int PowParam2, int AttackState, int Resistance, DWORD dwSkillCode, int UseAccuracy);

int VIP::vipCommands() {

	char msg[32] = { 0 };
	short vipLevel = getVipLevel();

	if (!vipLevel) return FALSE;

	CHATGAMEHANDLE->AddChatBoxText("> Você é VIP!", EChatColor::CHATCOLOR_Error);

	switch (getVipLevel()) 
	{
	case 1:
	case 2:
	case 3:
		CHATGAMEHANDLE->AddChatBoxText("> Comandos disponíveis", EChatColor::CHATCOLOR_Error);
		CHATGAMEHANDLE->AddChatBoxText("Comando /premium (Abre o Negociante Mughy)", EChatColor::CHATCOLOR_Error);
		CHATGAMEHANDLE->AddChatBoxText("Comando /repot (Abre a Loja da Allya)", EChatColor::CHATCOLOR_Error);
		CHATGAMEHANDLE->AddChatBoxText("Comando /bau (Abre o bau)", EChatColor::CHATCOLOR_Error);
		CHATGAMEHANDLE->AddChatBoxText("Comando /core (Abre o core azul)", EChatColor::CHATCOLOR_Error);
		CHATGAMEHANDLE->AddChatBoxText("Comando /mvip (mapa VIP)", EChatColor::CHATCOLOR_Error);
		CHATGAMEHANDLE->AddChatBoxText("Aviso: Você ganha 10% de Experiência Adicional!", EChatColor::CHATCOLOR_Error);
		break;
	}

	return TRUE;	
}

void VIP::setTimePremiuns() {
	chaPremiumitem.SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_30DAY, true, UpKeepItemName[0], 30);
	chaPremiumitem.SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS, SINITEM_TIME_30DAY, true, UpKeepItemName[2], 30);
	chaPremiumitem.SetUpKeepItem(nsPremiumItem::MANA_RECHAR_P, SINITEM_TIME_30DAY, true, UpKeepItemName[3], 30);
	chaPremiumitem.SetUpKeepItem(nsPremiumItem::PHENIX_PET, SINITEM_TIME_30DAY, true, UpKeepItemName[6], 30);
	chaPremiumitem.SetUpKeepItem(nsPremiumItem::HELP_PET_MUTE, SINITEM_TIME_30DAY, true, UpKeepItemName[10], 30);

	cHelpPet.PetKind = HoLowLevelPet::Type::Mute + 1;
	cHelpPet.PetShow = 1;
	cHelpPet.ShowPet();
}
