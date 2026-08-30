#pragma once

#define   SIN_ADD_FIRE				0x00000001	//Fogo
#define   SIN_ADD_ICE				0x00000002	//Gelo
#define   SIN_ADD_LIGHTNING			0x00000004	//Raio
#define   SIN_ADD_POISON			0x00000008	//Veneno
#define   SIN_ADD_BIO				0x00000010	//Orgânico
#define   SIN_ADD_Critico			0x00000020	//Crítico
#define   SIN_ADD_ATTACK_RATE		0x00000040	//Taxa de Ataque
#define   SIN_ADD_DAMAGE_MIN		0x00000080	//Poder de Ataque Min.
#define   SIN_ADD_DAMAGE_MAX		0x00000100	//Poder de Ataque Máx.
#define   SIN_ADD_ATTACK_SPEED		0x00000200	//Vel. de Ataque
#define   SIN_ADD_ABSORB			0x00000400	//Absorção
#define   SIN_ADD_DEFENCE			0x00000800	//Taxa de Defesa
#define   SIN_ADD_BLOCK_RATE		0x00001000	//Bloqueio
#define   SIN_ADD_MOVE_SPEED		0x00002000	//Velocidade
#define   SIN_ADD_LIFE				0x00004000	//HP Máx.
#define   SIN_ADD_MANA				0x00008000	//MP Máx.
#define   SIN_ADD_STAMINA			0x00010000	//RES>
#define   SIN_ADD_LIFEREGEN			0x00020000 	//Regen de HP.
#define   SIN_ADD_MANAREGEN			0x00040000  	//Regen de Mana.
#define   SIN_ADD_STAMINAREGEN		0x00080000  	//Regen de RES.

#define   SIN_ADD_NUM				1
#define   SIN_ADD_PERCENT			2


// Mix Ashen

{ 1, { sinWA1, sinWC1, sinWH1, sinWM1, sinWP1, sinWS1, sinWS2, sinWT1 },
{ 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_DAMAGE_MIN, SIN_ADD_DAMAGE_MAX, SIN_ADD_ATTACK_RATE, SIN_ADD_CRITICAL, SIN_ADD_MANA, 0, 0, 0 },
{ 7, 8, 35, 2, 10, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+7 de Poder de Ataque Min\r+8 de Poder de Ataque Max\r+35 de Taxa de Ataque\r+2 de Crítico\r+10 de Mana\r"},

{ 2, {sinDA1, sinDA2, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 80, 2.0f, 10, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+80 de Defesa\r+2.0 de Absorção\r+10 de Vida\r" },

{ 3, {sinDS1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_ABSORB, SIN_ADD_BLOCK_RATE, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 1.5f, 2, 10, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+1.5 de Absorção\r+2% de Bloqueio\r+10 de Vida\r" },

{ 4, {sinOM1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_STAMINA, 0, 0, 0, 0 },
{ 20, 1.4f, 15, 45, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0 },
"+20 de Defesa\r+1.4 de Absorção\r+15 de Vida\r+45 de Res.\r" },

{ 5, {sinOA2, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ATTACK_RATE, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 15, 30, 5, 10, 15, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+15 de Defesa\r+30 de Taxa de Ataque\r+5 de Vida\r+10 de Mana\r+15 de Res.\r" },

{ 6, {sinDG1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 10, 1.0f, 5, 10, 15, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+10 de Defesa\r+1.0 de Absorção\r+5 de Vida\r+10 de Mana\r+15 de Res.\r" },

{ 7, {sinDB1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_MOVE_SPEED, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 10, 1.2f, 0.9f, 5, 10, 15, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+10 de Defesa\r+1.2 de Absorção\r+0.9 de Absorção\r+5 de Vida\r+10 de Mana\r+15 de Res.\r" },

{ 8, {sinOA1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 0.4f, 0.8f, 4, 8, 12, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+0.4 de Reg. de Vida\r+0.3 de Reg. de Mana\r+4 de Vida\r+8 de Mana\r+12 de Res.\r" },

{ 9, {sinOR1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, 0, 0, 0, 0, 0 },
{ 0.4f, 2, 6, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+0.4 de Reg. de Vida\r+2 de Vida\r+6 de Mana\r" },

// Mix Laroy

{ 10, { sinWA1, sinWC1, sinWH1, sinWM1, sinWP1, sinWS1, sinWS2, sinWT1},
{ 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 },
{ SIN_ADD_DAMAGE_MIN, SIN_ADD_DAMAGE_MAX, SIN_ADD_ATTACK_RATE, SIN_ADD_CRITICAL, SIN_ADD_MANA, 0, 0, 0 },
{ 8, 9, 40, 3, 15, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+8 de Poder de Ataque Min\r+9 de Poder de Ataque Max\r+40 de Taxa de Ataque\r+3 de Crítico\r+15 de Mana\r" },

{ 11, {sinDA1, sinDA2, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 120, 3.0f, 15, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+120 de Defesa\r+3.0 de Absorção\r+15 de Vida\r" },

{ 12, {sinDS1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 },
{ SIN_ADD_ABSORB, SIN_ADD_BLOCK_RATE, SIN_ADD_LIFE, 0, 0, 0, 0, 0 },
{ 2.0f, 3, 20, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+2.0 de Absorção\r+3% de Bloqueio\r+20 de Vida\r" },

{ 13, {sinOM1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_STAMINA, 0, 0, 0, 0 },
{ 55, 2.0f, 20, 60, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0 },
"+55 de Defesa\r+2.0 de Absorção\r+20 de Vida\r+60 de Res.\r" },

{ 14, {sinOA2, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ATTACK_RATE, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 25, 40, 10, 15, 20, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+25 de Defesa\r+40 de Taxa de Ataque\r+10 de Vida\r+15 de Mana\r+20 de Res.\r" },

{ 15, {sinDG1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 30, 2.0f, 10, 15, 20, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+30 de Defesa\r+2.0 de Absorção\r+10 de Vida\r+15 de Mana\r+20 de Res.\r" },

{ 16, {sinDB1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 },
{ SIN_ADD_DEFENCE, SIN_ADD_ABSORB, SIN_ADD_MOVE_SPEED, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0 },
{ 15, 1.8f, 1.8f, 10, 15, 20, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0 },
"+15 de Defesa\r+1.8 de Absorção\r+1.8 de Absorção\r+10 de Vida\r+15 de Mana\r+20 de Res.\r" },

{ 17, {sinOA1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_MANAREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, SIN_ADD_STAMINA, 0, 0, 0 },
{ 0.6f, 1.0f, 6, 10, 14, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0 },
"+0.6 de Reg. de Vida\r+1.0 de Reg. de Mana\r+6 de Vida\r+10 de Mana\r+14 de Res.\r" },

{ 18, {sinOR1, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0 },
{ SIN_ADD_LIFEREGEN, SIN_ADD_LIFE, SIN_ADD_MANA, 0, 0, 0, 0, 0 },
{ 0.8f, 6, 10, 0, 0, 0, 0, 0 },
{ SIN_ADD_NUM, SIN_ADD_NUM, SIN_ADD_NUM, 0, 0, 0, 0, 0 },
"+0.8 de Reg. de Vida\r+6 de Vida\r+10 de Mana\r" },