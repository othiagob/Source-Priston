#pragma once

#define _LANGUAGE_ENGLISH
#define _LANGUAGE_ENGLISH_FIELDLEVEL
#define _LANGUAGE_ENGLISH_NEXTUP   

// Attack and Defense rate of Server
#define	ATTACK_RATE					70	
#define	DEFENSE_RATE				80

// GameMaster Mode
#define	_WINMODE_DEBUG
#define GAME_GM

// Definitions of Server
//#define	_PACKET_PASS_XOR			0xED
#define	_MODE_EXP64
#define	_SECURITY_UPDATE

#ifndef _HH
#define _HH

static int _PACKET_PASS_XOR = 0x8B;

#endif