#include "Skills.h"
#include <windows.h>
#include <stdio.h>
#include "smPacket.h"
#include "..\\smwsock.h"
#include <iostream>

using namespace std;

Skills skill;

Skills::Skills()
{
	ZeroMemory(Buffer, sizeof(Buffer));
	MeuInt = 0;
	MeuFloat = 0.0f;
}

Skills::~Skills()
{
}



void Skills::readMechanician()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Server\\Skills\\Mecanico.ini", "r"); // Abre o arquivo Mecanico.ini para leitura
	if (fp == NULL) // Se falhar
		MessageBox(0, "Mecanico.ini não foi carregado!", "Erro!", 0x30); // Exibir mensagem de erro
	else
	{
		while (fscanf_s(fp, "%s", Buffer, sizeof(Buffer)) != EOF) // Lê o arquivo Lutador.ini até o final ( EndOfFile )
		{
			// [Mecânico - 1ª Tier início]
			if (strcmp(Buffer, "TA1S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					E_Shield_BlockRate[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA1S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					E_Shield_UseTime[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA1S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					E_Shield_UseMana[i] = MeuInt;
				}
			} // * Escudo Extremo *
			else if (strcmp(Buffer, "TA2S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Mechanic_Bomb_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA2S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Mechanic_Bomb_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA2S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Mechanic_Bomb_Attack_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA2S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Mechanic_Bomb_UseMana[i] = MeuInt;
				}
			} // * Bomba Mecânica *
			else if (strcmp(Buffer, "TA3S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					P_Absorb[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA3S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					P_Absorb[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA3S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					P_Absorb_UseTime[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA3S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					P_Absor_bUseMana[i] = MeuInt;
				}
			} // * Absorção Física *
			else if (strcmp(Buffer, "TA4S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					PlusPoison[i] = MeuInt;
				}
			} // * Resistência a Veneno *	
			// [Mecânico - 1ª Tier Fim]

			// [Mecânico - 2ª Tier início]
			else if (strcmp(Buffer, "TA5S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					G_Smash_Attack_Rate[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA5S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					G_Smash_Attack_Rage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA5S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					G_Smash_UseMana[i] = MeuInt;
				}
			} // * Grande Golpe *
			else if (strcmp(Buffer, "TA6S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Maximize_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA6S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Maximize_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA6S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Maximize_UseMana[i] = MeuInt;
				}

			} // * Maximizar *
			else if (strcmp(Buffer, "TA7S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Automation_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA7S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Automation_Speed[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA7S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Automation_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA7S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Automation_UseMana[i] = MeuInt;
				}
			} // * Automação *
			else if (strcmp(Buffer, "TA8S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spark_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA8S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spark_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA8S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spark_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA8S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spark_UseMana[i] = MeuInt;
				}
			} // * Spark *
			// [Mecânico - 2ª Tier Fim]

			// [Mecânico - 3ª Tier início]
			else if (strcmp(Buffer, "TA9S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Metal_Armor_Defense[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA9S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Metal_Armor_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA9S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Metal_Armor_UseMana[i] = MeuInt;
				}
			} // * Armadura Metálica *
			else if (strcmp(Buffer, "TA10S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Smash_AttackRate[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA10S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Smash_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA10S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Smash_UseMana[i] = MeuInt;
				}

			} // * Golpe Grandioso *
			else if (strcmp(Buffer, "TA11S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					M_Weapon_Mastey[i] = MeuInt;
				}
			} // * Mestre dos Mecânicos *	
			else if (strcmp(Buffer, "TA12S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spark_Shield_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA12S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spark_Shield_Defense[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA12S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spark_Shield_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA12S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spark_Shield_UseMana[i] = MeuInt;
				}
			} // * Escudo Espinhoso *
			// [Mecânico - 3ª Tier Fim]

			// [Mecânico - 4ª Tier início]
			else if (strcmp(Buffer, "TA13S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Impulsion_LightingDamage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA13S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Impulsion_LightingDamage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA13S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Impulsion_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA13S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Impulsion_LightingNum[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA13S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Impulsion_UseMana[i] = MeuInt;
				}
			} // * Impulso *
			else if (strcmp(Buffer, "TA14S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Compulsion_AddAbsorb[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA14S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Compulsion_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA14S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Compulsion_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA14S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Compulsion_UseMana[i] = MeuInt;
				}
			} // * Implosão  *
			else if (strcmp(Buffer, "TA15S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Magnetic_Sphere_AddDamage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA15S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Magnetic_Sphere_AddDamage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA15S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Magnetic_Sphere_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA15S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Magnetic_Sphere_AttackDelay[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA15S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Magnetic_Sphere_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA15S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Magnetic_Sphere_UseMana[i] = MeuInt;
				}
			} // * Esfera Magnética *	
			else if (strcmp(Buffer, "TA16S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Metal_Golem_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA16S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Metal_Golem_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA16S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Metal_Golem_Hit[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA16S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Metal_Golem_Defense[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA16S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Metal_Golem_Life[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TA16S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Metal_Golem_UseMana[i] = MeuInt;
				}
			} //  * Golem de Metal  *	
			// [Mecânico - 4ª Tier Fim]
		}
		fclose(fp);
	}
}
void Skills::readFighter()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Server\\Skills\\Lutador.ini", "r"); // Abre o arquivo Lutador.ini para leitura
	if (fp == NULL) // Se falhar
		MessageBox(0, "Lutador.ini não foi carregado!", "Erro!", 0x30); // Exibir mensagem de erro
	else
	{
		while (fscanf_s(fp, "%s", Buffer, sizeof(Buffer)) != EOF) // Lê o arquivo Lutador.ini até o final ( EndOfFile )
		{
			// [Lutador - 1ª Tier início]
			if (strcmp(Buffer, "TB1S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Melee_Mastery_DamagePercent[i] = MeuInt;
				}
			} // * Mestre das Armas *
			else if (strcmp(Buffer, "TB2S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					PlusFire[i] = MeuInt;
				}
			} // * Resistência a Fogo *
			else if (strcmp(Buffer, "TB3S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Raving_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB3S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Ravind_Speed[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB3S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuFloat = (float)atof(Buffer);
					Raving_UseLife[i] = MeuFloat;
				}
			}
			else if (strcmp(Buffer, "TB3S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Raving_UseMana[i] = MeuInt;
				}
			} // * Raiva *
			else if (strcmp(Buffer, "TB4S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Impact_Attack_Rating[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB4S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Impact_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB4S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Impact_UseMana[i] = MeuInt;
				}
			} // * Impacto *
			// [Lutador - 1ª Tier Fim]

			// [Lutador - 2ª Tier início]
			else if (strcmp(Buffer, "TB5S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					T_Impact_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB5S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					T_Impact_Hit[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB5S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					T_Impact_UseMana[i] = MeuInt;
				}
			} // * Impacto Triplo *
			else if (strcmp(Buffer, "TB6S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					B_Swing_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB6S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					B_Swing_Critical[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB6S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					B_Swing_UseMana[i] = MeuInt;
				}
			} // * Girada Brutal *
			else if (strcmp(Buffer, "TB7S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Roar_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB7S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Roar_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB7S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Roar_UseMana[i] = MeuInt;
				}
			} // * Rugido *
			else if (strcmp(Buffer, "TB8S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					R_Zecram_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB8S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					R_Zecram_UseMana[i] = MeuInt;
				}
			} // * Fúria de Zecram *
			// [Lutador - 2ª Tier Fim]

			// [Lutador - 3ª Tier início]
			else if (strcmp(Buffer, "TB9S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Concentration_AttackRate[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB9S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Concentration_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB9S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Concentration_UseMana[i] = MeuInt;
				}
			} // * Concentração *
			else if (strcmp(Buffer, "TB10S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					A_Crash_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB10S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					A_Crash_AttackRate[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB10S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					A_Crash_UseMana[i] = MeuInt;
				}
			} // * Golpe da Vingança *
			else if (strcmp(Buffer, "TB11S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Swift_Axe_Speed[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB11S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Swift_Axe_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB11S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Swift_Axe_UseMana[i] = MeuInt;
				}
			} // * Machado Veloz *
			else if (strcmp(Buffer, "TB12S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					B_Crash_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB12S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					B_Crash_DemonDamage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB12S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					B_Crash_UseMana[i] = MeuInt;
				}
			} // * Quebra Ossos *
			// [Lutador - 3ª Tier Fim]

			// [Lutador - 4ª Tier início]
			else if (strcmp(Buffer, "TB13S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Destoryer_DamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB13S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Destoryer_AddCritical[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB13S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Destoryer_UseMana[i] = MeuInt;
				}
			} // * Destroyer *
			else if (strcmp(Buffer, "TB14S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Berserker_AddAttack[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB14S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Berserker_SubAbsorb[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB14S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Berserker_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB14S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Berserker_UseMana[i] = MeuInt;
				}
			} // * Fúria *
			else if (strcmp(Buffer, "TB15S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Cyclone_Strike_DamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB15S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Cyclone_Strike_AreaDamage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB15S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Cyclone_Strike_AreaDamage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB15S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Cyclone_Strike_AttackNum[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB15S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Cyclone_Strike_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TB15S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Cyclone_Strike_UseMana[i] = MeuInt;
				}
			} // * Golpe Giratório *
			else if (strcmp(Buffer, "TB16S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Boost_Health_Life[i] = MeuInt;
				}
			} // * Bônus de Vitalidade *
			// [Lutador - 4ª Tier Fim]
		}
		fclose(fp);
	}
}
void Skills::readPikeman()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Server\\Skills\\Pike.ini", "r"); // Abre o arquivo Pike.ini para leitura
	if (fp == NULL) // Se falhar
		MessageBox(0, "Pike.ini não foi carregado!", "Erro!", 0x30); // Exibir mensagem de erro
	else
	{
		while (fscanf_s(fp, "%s", Buffer, sizeof(Buffer)) != EOF) // Lê o arquivo Pike.ini até o final ( EndOfFile )
		{
			// [Pike - 1ª Tier início]
			if (strcmp(Buffer, "TC1S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Pike_Wind_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC1S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Pike_Wind_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC1S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Pike_Wind_Push_Lenght[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC1S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Pike_Wind_UseMana[i] = MeuInt;
				}
			} // * Sopro Afiado  *
			else if (strcmp(Buffer, "TC2S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					PlusIce[i] = MeuInt;
				}
			} // * Resistência de Gelo *
			else if (strcmp(Buffer, "TC3S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Critical_Hit_Critical[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC3S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Critical_Hit_UseMana[i] = MeuInt;
				}
			} // * Ataque Crítico *
			else if (strcmp(Buffer, "TC4S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Jumping_Crash_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC4S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Jumping_Crash_Attack_Rating[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC4S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Jumping_Crash_Damage[i] = MeuInt;
				}
			} // * Pulo Fatal *
			// [Pike - 1ª Tier Fim]

			// [Pike - 2ª Tier início]
			else if (strcmp(Buffer, "TC5S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					G_Pike_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC5S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					G_Pike_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC5S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					G_Pike_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC5S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					G_Pike_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC5S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					G_Pike_Time[i] = MeuInt;
				}
			} // * Espinhos de Gelo *
			else if (strcmp(Buffer, "TC6S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Tornado_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC6S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Tornado_Stun[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC6S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Tornado_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC6S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Tornado_UseMana[i] = MeuInt;
				}
			} // * Tornado *
			else if (strcmp(Buffer, "TC7S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					W_D_Mastery_Block[i] = MeuInt;
				}
			} // * Mestre da Defesa *
			else if (strcmp(Buffer, "TC8S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Expansion_Size[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC8S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Expansion_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC8S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Expansion_UseMana[i] = MeuInt;
				}
			} // * Expansão *
			// [Pike - 2ª Tier Fim]

			// [Pike - 3ª Tier início]
			else if (strcmp(Buffer, "TC9S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					VenomSpear_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC9S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					VenomSpear_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC9S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					VenomSpear_Num[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC9S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					VenomSpear_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC9S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					VenomSpear_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC9S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					VenomSpear_UseMana[i] = MeuInt;
				}
			} // * Lanças Venenosas *
			else if (strcmp(Buffer, "TC10S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vanish_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC10S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vanish_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC10S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vanish_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC10S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vanish_UseMana[i] = MeuInt;
				}
			} // * Desaparecer *
			else if (strcmp(Buffer, "TC11S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Critical_Mastery_Critical[i] = MeuInt;
				}
			} // * Mestre de ATQ Crítico * 
			else if (strcmp(Buffer, "TC12S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Chain_Lance_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC12S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Chain_Lance_UseMana[i] = MeuInt;
				}
			} // * Foice de Aço *
			// [Pike - 3ª Tier Fim]

			// [Pike - 4ª Tier início]
			else if (strcmp(Buffer, "TC13S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Assassin_Eye_AddCritical[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC13S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Assassin_Eye_M_SubCritical[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC13S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Assassin_Eye_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC13S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Assassin_Eye_UseMana[i] = MeuInt;
				}
			} // * Olho Assassino *
			else if (strcmp(Buffer, "TC14S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Charging_Strike_DamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC14S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Charging_Strike_CharingDamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC14S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Charging_Strike_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC14S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Charging_Strike_UseMana[i] = MeuInt;
				}
			} // * Golpe Carregado *
			else if (strcmp(Buffer, "TC15S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vague_EvasionPercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC15S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vague_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC15S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vague_UseMana[i] = MeuInt;
				}
			} // * Furtivo *
			else if (strcmp(Buffer, "TC16S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Shadow_Master_DamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC16S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Shadow_Master_ShadowNum[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TC16S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Shadow_Master_UseMana[i] = MeuInt;
				}
			} // * Mestre das Sombras *
			// [Pike - 4ª Tier Fim]
		}
		fclose(fp);
	}
}
void Skills::readArcher()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Server\\Skills\\Arqueira.ini", "r"); // Abre o arquivo Arqueira.ini para leitura
	if (fp == NULL) // Se falhar
		MessageBox(0, "Arqueira.ini não foi carregado!", "Erro!", 0x30); // Exibir mensagem de erro
	else
	{
		while (fscanf_s(fp, "%s", Buffer, sizeof(Buffer)) != EOF) // Lê o arquivo Arqueira.ini até o final ( EndOfFile )
		{
			// [Arqueira - 1ª Tier início]
			if (strcmp(Buffer, "TD1S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Scout_Hawk_Attack_Rate[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD1S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Scout_Hawk_UseTime[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD1S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Scout_Hawk_UseMana[i] = MeuInt;
				}
			} // * Falcão Vigia *
			else if (strcmp(Buffer, "TD2S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					S_Mastery_DamagePercent[i] = MeuInt;
				}
			} // * Mestra do Tiro *
			else if (strcmp(Buffer, "TD3S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Wind_Arrow_Speed[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD3S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Wind_Arrow_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD3S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Wind_Arrow_UseMana[i] = MeuInt;
				}
			} // * Flecha de Vento *
			else if (strcmp(Buffer, "TD4S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Perfect_Aim_Attack_Rate[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD4S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Perfect_Aim_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD4S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Perfect_Aim_Damage_LV[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD4S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Perfect_Aim_UseMana[i] = MeuInt;
				}
			} // * Mira Perfeita *
			// [Arqueira - 1ª Tier Fim]

			// [Arqueira - 2ª Tier início]
			else if (strcmp(Buffer, "TD5S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					D_Eye_Attack_Rate[i] = MeuInt;
				}
			} // * Olho de Dion *
			else if (strcmp(Buffer, "TD6S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Falcon_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD6S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Falcon_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD6S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Falcon_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD6S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Falcon_UseMana[i] = MeuInt;
				}
			}
			// * Falcão *

			else if (strcmp(Buffer, "TD7S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					A_Rage_ArrowNum[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD7S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					A_Rage_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD7S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					A_Rage_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD7S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					A_Rage_UseMana[i] = MeuInt;
				}
			}
			// * Flecha da Fúria *
			else if (strcmp(Buffer, "TD8S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Avalanche_Speed[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD8S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Avalanche_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD8S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Avalanche_ArrowNum[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD8S4A=") == 0)
			{
			for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Avalanche_UseMana[i] = MeuInt;
				}
			}
			// 	* Avalanche *
			// [Arqueira - 2ª Tier Fim]

			// [Arqueira - 3ª Tier início]
			else if (strcmp(Buffer, "TD9S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Elemental_Shot_Fire[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD9S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Elemental_Shot_Fire[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD9S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Elemental_Shot_Lightning[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD9S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Elemental_Shot_Lightning[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD9S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Elemental_Shot_UseMana[i] = MeuInt;
				}
			} // * Tiro Elemental *
			else if (strcmp(Buffer, "TD10S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Golden_Falcon_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD10S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuFloat = (float)atof(Buffer);
					Golden_Falcon_LifeRegen[i] = MeuFloat;
				}
			}
			else if (strcmp(Buffer, "TD10S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Golden_Falcon_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD10S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Golden_Falcon_UseMana[i] = MeuInt;
				}
			} // * Falcão Dourado  *
			else if (strcmp(Buffer, "TD11S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					BombShot_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD11S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					BombShot_DemonDamage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD11S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					BombShot_AddDamage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD11S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					BombShot_AddDamage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD11S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					BombShot_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD11S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					BombShot_UseMana[i] = MeuInt;
				}
			} // * Tiro Explosivo *
			else if (strcmp(Buffer, "TD12S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Perforation_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD12S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Perforation_Critical[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD12S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Perforation_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD12S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Perforation_Attack_Range[i] = MeuInt;
				}
			} // * Perfuração *
			// [Arqueira - 3ª Tier Fim]

			// [Arqueira - 4ª Tier início]
			else if (strcmp(Buffer, "TD13S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Recall_Wolverin_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD13S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Recall_Wolverin_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD13S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Recall_Wolverin_Hit[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD13S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Recall_Wolverin_Defense[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD13S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Recall_Wolverin_Life[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD13S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Recall_Wolverin_UseMana[i] = MeuInt;
				}
			} // * Invocar Wolverine *
			else if (strcmp(Buffer, "TD14S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Evasion_Mastery_AddPercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD14S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuFloat = (float)atof(Buffer);
					Evasion_Mastery_AddSpeed[i] = MeuFloat;
				}
			}
			else if (strcmp(Buffer, "TD14S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Evasion_Mastery_AddReduce[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD14S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Evasion_Mastery_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD14S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Evasion_Mastery_UseMana[i] = MeuInt;
				}
			}// * Mestra da Esquiva *
			else if (strcmp(Buffer, "TD15S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Phoenix_Shot_DamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD15S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Phoenix_Shot_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD15S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Phoenix_Shot_UseMana[i] = MeuInt;
				}
			} // * Tiro Fênix *
			else if (strcmp(Buffer, "TD16S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Force_Of_Nature_AddDamage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD16S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Force_Of_Nature_AddHit[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD16S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Force_Of_Nature_FalconAddDamage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD16S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Force_Of_Nature_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "TD16S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Force_Of_Nature_UseMana[i] = MeuInt;
				}
			} // * Força da Natureza *
			// [Arqueira - 4ª Tier Fim]
		}
		fclose(fp);
	}
}
void Skills::readKnight()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Server\\Skills\\Cavaleiro.ini", "r"); // Abre o arquivo Cavaleiro.ini para leitura
	if (fp == NULL) // Se falhar
		MessageBox(0, "Cavaleiro.ini não foi carregado!", "Erro!", 0x30); // Exibir mensagem de erro
	else
	{
		while (fscanf_s(fp, "%s", Buffer, sizeof(Buffer)) != EOF) // Lê o arquivo Cavaleiro.ini até o final ( End Of File )
		{
			// [Cavaleiro - 1ª Tier início]
			if (strcmp(Buffer, "MA1S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Sword_Blast_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA1S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Sword_Blast_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA1S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Sword_Blast_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA1S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Sword_Blast_ShootingRange[i] = MeuInt;
				}
			} // * Rajada Sagrada *
			else if (strcmp(Buffer, "MA2S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					HolyBody_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA2S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					HolyBody_Absorb[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA2S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					HolyBody_Time[i] = MeuInt;
				}
			} // * Corpo Sagrado *
			else if (strcmp(Buffer, "MA3S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					P_Traning_Stamina[i] = MeuInt;
				}
			} // * Treinamento Físico *
			else if (strcmp(Buffer, "MA4S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					D_Crash_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA4S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					D_Crash_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA4S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					D_Crash_Critical[i] = MeuInt;
				}
			} // * Golpe Duplo *
			// [Cavaleiro - 1ª Tier Fim]

			// [Cavaleiro - 2ª Tier início]
			else if (strcmp(Buffer, "MA5S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Holy_Valor_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA5S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Holy_Valor_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA5S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Holy_Valor_Mana[i] = MeuInt;
				}
			} // * Valor Sagrado *
			else if (strcmp(Buffer, "MA6S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Brandish_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA6S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Brandish_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA6S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Brandish_Mana[i] = MeuInt;
				}
			} // * Brandish  *
			else if (strcmp(Buffer, "MA7S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Piercing_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA7S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Piercing_Attack_Rating[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA7S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Piercing_Mana[i] = MeuInt;
				}
			} // * Piercing *
			else if (strcmp(Buffer, "MA8S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Drastic_Spirit_Defense[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA8S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Drastic_Spirit_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA8S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Drastic_Spirit_Mana[i] = MeuInt;
				}
			} // * Espírito Drástico *
			// [Cavaleiro - 2ª Tier Fim]

			// [Cavaleiro - 3ª Tier início]
			else if (strcmp(Buffer, "MA9S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Sword_Mastery_DamagePercent[i] = MeuInt;
				}
			} // * Mestre da Espada *
			else if (strcmp(Buffer, "MA10S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					D_Inhalation_Life[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA10S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					D_Inhalation_Block[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA10S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					D_Inhalation_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA10S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					D_Inhalation_UseMana[i] = MeuInt;
				}
			} // * Escudo Divino *
			else if (strcmp(Buffer, "MA11S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					H_Incantation_Success[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA11S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					H_Incantation_AddLife[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA11S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					H_Incantation_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA11S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					H_Incantation_UseMana[i] = MeuInt;
				}
			} // * Encantamento Sagrado *
			else if (strcmp(Buffer, "MA12S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Cross_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA12S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Cross_AttackRate[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA12S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Cross_UndeadDamage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA12S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Cross_UseMana[i] = MeuInt;
				}
			} // * Grande Cruzada *
			// [Cavaleiro - 3ª Tier Fim]

			// [Cavaleiro - 4ª Tier início]
			else if (strcmp(Buffer, "MA13S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Divine_Piercing_DamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA13S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Divine_Piercing_AddHit[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA13S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Divine_Piercing_AttackNum[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA13S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Divine_Piercing_UseMana[i] = MeuInt;
				}
			} // * Piercing Divino *
			else if (strcmp(Buffer, "MA14S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Godly_Shield_AbsorbPercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA14S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Godly_Shield_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA14S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Godly_Shield_UseMana[i] = MeuInt;
				}
			} // * Escudo dos Deuses *
			else if (strcmp(Buffer, "MA15S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					God_Bless_AddDamage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA15S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					God_Bless_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA15S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					God_Bless_UseMana[i] = MeuInt;
				}
			} // * Bênção Divina *
			else if (strcmp(Buffer, "MA16S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Sword_Of_Justice_DamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA16S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Sword_Of_Justice_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MA16S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Sword_Of_Justice_UseMana[i] = MeuInt;
				}
			} // * Espada Justiceira *
			// [Cavaleiro - 4ª Tier Fim]
		}
		fclose(fp);
	}
}
void Skills::readAtalanta()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Server\\Skills\\Atalanta.ini", "r"); // Abre o arquivo Atalanta.ini para leitura
	if (fp == NULL) // Se falhar
		MessageBox(0, "Atalanta.ini não foi carregado!", "Erro!", 0x30); // Exibir mensagem de erro
	else
	{
		while (fscanf_s(fp, "%s", Buffer, sizeof(Buffer)) != EOF) // Lê o arquivo Atalanta.ini até o final ( End Of File )
		{
			// [Atalanta - 1ª Tier início]
			if (strcmp(Buffer, "MB1S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					S_Strike_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB1S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					S_Strike_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB1S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					S_Strike_Damage[i][1] = MeuInt;
				}
			}
			// * Golpe do Escudo *
			else if (strcmp(Buffer, "MB2S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Farina_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB2S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Farina_Speed[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB2S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Farina_AttackRate[i] = MeuInt;
				}
			} // * Farina *
			else if (strcmp(Buffer, "MB3S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					T_Mastery_Damage[i] = MeuInt;
				}
			} // * Treinamento Físico *
			else if (strcmp(Buffer, "MB4S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					V_Spear_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB4S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					V_Spear_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB4S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					V_Spear_Damage[i][1] = MeuInt;
				}
			} // * Lança Gigante *
			// [Atalanta - 1ª Tier Fim]

			// [Atalanta - 2ª Tier início]
			else if (strcmp(Buffer, "MB5S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Windy_Attack_Rating[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB5S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Windy_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB5S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Windy_Mana[i] = MeuInt;
				}
			} // * Windy *
			else if (strcmp(Buffer, "MB6S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Twist_Javelin_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB6S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Twist_Javelin_Attack_Rating[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB6S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Twist_Javelin_Mana[i] = MeuInt;
				}
			} // * Lança Giratória *
			else if (strcmp(Buffer, "MB7S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Soul_Sucker_Absorb[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB7S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Soul_Sucker_Mana[i] = MeuInt;
				}
			} // * Ladrão de Almas *
			else if (strcmp(Buffer, "MB8S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Fire_Javelin_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB8S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Fire_Javelin_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB8S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Fire_Javelin_Mana[i] = MeuInt;
				}
			} // * Lança de Fogo *
			// [Atalanta - 2ª Tier Fim]

			// [Atalanta - 3ª Tier início]
			else if (strcmp(Buffer, "MB9S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Split_Javelin_AttackNum[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB9S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Split_Javelin_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB9S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Split_Javelin_AttackRate[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB9S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Split_Javelin_UseMana[i] = MeuInt;
				}
			} // * Lança Partida *
			else if (strcmp(Buffer, "MB10S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					T_Of_Valhalla_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB10S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					T_Of_Valhalla_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB10S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					T_Of_Valhalla_UseMana[i] = MeuInt;
				}
			} // * Triunfo de Valhalla *
			else if (strcmp(Buffer, "MB11S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Lightning_Javelin_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB11S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Lightning_Javelin_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB11S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Lightning_Javelin_UseMana[i] = MeuInt;
				}
			} // * Lança de Raios *
			else if (strcmp(Buffer, "MB12S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Storm_Javelin_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB12S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Storm_Javelin_UseMana[i] = MeuInt;
				}
			} // * Chuva de Lanças *
			// [Atalanta - 3ª Tier Fim]

			// [Atalanta - 4ª Tier início]
			else if (strcmp(Buffer, "MB13S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Hall_Of_Valhalla_AddEvation[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB13S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Hall_Of_Valhalla_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB13S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Hall_Of_Valhalla_UseMana[i] = MeuInt;
				}
			} // * Inferno de Valhalla *
			else if (strcmp(Buffer, "MB14S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					X_Rage_DamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB14S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					X_Rage_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB14S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					X_Rage_UseMana[i] = MeuInt;
				}
			} // * Fúria-X *
			else if (strcmp(Buffer, "MB15S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Frost_Javelin_IceAddDamage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB15S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Frost_Javelin_IceAddDamage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB15S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Frost_Javelin_SpeedSubPercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB15S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Frost_Javelin_IceTime[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB15S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Frost_Javelin_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB15S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Frost_Javelin_UseMana[i] = MeuInt;
				}
			} // * Lança de Gelo *
			else if (strcmp(Buffer, "MB16S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vengeance_DamagePercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB16S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vengeance_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB16S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vengeance_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MB16S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vengeance_AddCritical[i] = MeuInt;
				}
			}// * Vingança *
			// [Atalanta - 4ª Tier Fim]
		}
		fclose(fp);
	}
}
void Skills::readPriestess()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Server\\Skills\\Sacerdotisa.ini", "r"); // Abre o arquivo Sacerdotisa.ini para leitura
	if (fp == NULL) // Se falhar
		MessageBox(0, "Sacerdotisa.ini não foi carregado!", "Erro!", 0x30); // Exibir mensagem de erro
	else
	{
		while (fscanf_s(fp, "%s", Buffer, sizeof(Buffer)) != EOF) // Lê o arquivo Sacerdotisa.ini até o final ( End Of File )
		{
			// [Sacerdotisa - 1ª Tier início]
			if (strcmp(Buffer, "MC1S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Healing_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC1S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Healing_Heal[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC1S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Healing_Heal[i][1] = MeuInt;
				}
			} // * Cura *
			else if (strcmp(Buffer, "MC2S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					HolyBolt_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC2S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					HolyBolt_Damage[i] = MeuInt;
				}
			}
			// * Fagulha Sagrada *
			else if (strcmp(Buffer, "MC3S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					M_Spark_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC3S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					M_Spark_Damage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MC3S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					M_Spark_Num[i] = MeuInt;
				}
			} // * Espinhos Múltiplos  *
			else if (strcmp(Buffer, "MC4S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					HolyMind_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC4S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					HolyMind_DecDamage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC4S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					HolyMind_Time[i] = MeuInt;
				}
			}			// * Feitiço Divino *
			// [Sacerdotisa - 1ª Tier Fim]

			// [Sacerdotisa - 2ª Tier início]
			else if (strcmp(Buffer, "MC5S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuFloat = (float)atof(Buffer);
					Meditation_Regen[i] = MeuFloat;
				}
			} // * Meditação *
			else if (strcmp(Buffer, "MC6S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Divine_Lightning_Damage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MC6S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Divine_Lightning_Num[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC6S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Divine_Lightning_Mana[i] = MeuInt;
				}
			} // * Raios Divinos *
			else if (strcmp(Buffer, "MC7S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Holy_Reflection_Return_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC7S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Holy_Reflection_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC7S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Holy_Reflection_Mana[i] = MeuInt;
				}
			} // * Reflexão Divina  *
			else if (strcmp(Buffer, "MC8S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Healing[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC8S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Healing[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC8S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Healing_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC8S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Grand_Healing_Mana[i] = MeuInt;
				}
			} // * Cura Máxima *
			// [Sacerdotisa - 2ª Tier Fim]

			// [Sacerdotisa - 3ª Tier início]
			else if (strcmp(Buffer, "MC9S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vigor_Ball_Damage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MC9S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Vigor_Ball_UseMana[i] = MeuInt;
				}
			} // * Esferas de Vigor *
			else if (strcmp(Buffer, "MC10S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Resurrection_Percent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC10S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Resurrection_UseMana[i] = MeuInt;
				}
			} // * Ressurreição *
			else if (strcmp(Buffer, "MC11S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Extinction_Percent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC11S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Extinction_Amount[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC11S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Extinction_UseMana[i] = MeuInt;
				}
			} // * Extinção *
			else if (strcmp(Buffer, "MC12S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Virtual_Life_Percent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC12S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Virtual_Life_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC12S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Virtual_Life_UseMana[i] = MeuInt;
				}
			} // * HP Virtual *
			// [Sacerdotisa - 3ª Tier Fim]

			// [Sacerdotisa - 4ª Tier início]
			else if (strcmp(Buffer, "MC13S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Glacial_Spike_Damage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MC13S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Glacial_Spike_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC13S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Glacial_Spike_UseMana[i] = MeuInt;
				}
			} // * Espinhos Glaciais *
			else if (strcmp(Buffer, "MC14S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuFloat = (float)atof(Buffer);
					Regeneration_Field_LifeRegen[i] = MeuFloat;
				}
			}
			else if (strcmp(Buffer, "MC14S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuFloat = (float)atof(Buffer);
					Regeneration_Field_ManaRegen[i] = MeuFloat;
				}
			}
			else if (strcmp(Buffer, "MC14S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Regeneration_Field_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC14S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Regeneration_Field_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC14S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Regeneration_Field_UseMana[i] = MeuInt;
				}
			} // * Campo de Regen *
			else if (strcmp(Buffer, "MC15S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Chain_Lightning_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC15S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Chain_Lightning_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC15S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Chain_Lightning_UseMana[i] = MeuInt;
				}
			} // * Raios Contínuos *
			else if (strcmp(Buffer, "MC16S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Summon_Muspell_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC16S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Summon_Muspell_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC16S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Summon_Muspell_Attack_Delay[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC16S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Summon_Muspell_UndeadAbsorbPercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC16S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Summon_Muspell_BlockPercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC16S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Summon_Muspell_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MC16S7A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Summon_Muspell_UseMana[i] = MeuInt;
				}
			} // * Invocar Muspell *
			// [Sacerdotisa - 4ª Tier Fim]
		}
		fclose(fp);
	}
}
void Skills::readMagician()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Server\\Skills\\Mago.ini", "r"); // Abre o arquivo Mago.ini para leitura
	if (fp == NULL) // Se falhar
		MessageBox(0, "Mago.ini não foi carregado!", "Erro!", 0x30); // Exibir mensagem de erro
	else
	{
		while (fscanf_s(fp, "%s", Buffer, sizeof(Buffer)) != EOF) // Lê o arquivo Mago.ini até o final ( End Of File )
		{
			// [Mago - 1ª Tier início]
			if (strcmp(Buffer, "MD1S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Agony_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD1S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Agony_ConvLife[i] = MeuInt;
				}
			} // * Agônia *
			else if (strcmp(Buffer, "MD2S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					FireBolt_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD2S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					FireBolt_Damage[i] = MeuInt;
				}
			}
			// * Fagulha *
			else if (strcmp(Buffer, "MD3S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Zenith_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD3S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Zenith_Element[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD3S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Zenith_Time[i] = MeuInt;
				}
			} // * Zenith *
			else if (strcmp(Buffer, "MD4S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					FireBall_UseMana[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD4S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					FireBall_Damage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MD4S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					FireBall_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD4S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					FireBall_Range[i] = MeuInt;
				}
			} // * Bola de Fogo *
			// [Mago - 1ª Tier Fim]

			// [Mago - 2ª Tier início]
			else if (strcmp(Buffer, "MD5S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Mental_Mastery_Mana[i] = MeuInt;
				}
			} // * Mestre da Mente  *
			else if (strcmp(Buffer, "MD6S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Watornado_Damage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MD6S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Watornado_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD6S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Watornado_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD6S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Watornado_Mana[i] = MeuInt;
				}
			} // * Watornado *
			else if (strcmp(Buffer, "MD7S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Enchant_Weapon_Damage_Ice[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD7S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Enchant_Weapon_Damage_Ice[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD7S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Enchant_Weapon_Damage_Fire[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD7S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Enchant_Weapon_Damage_Fire[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD7S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Enchant_Weapon_Damage_Lightning[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD7S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Enchant_Weapon_Damage_Lightning[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD7S7A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Enchant_Weapon_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD7S8A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Enchant_Weapon_Mana[i] = MeuInt;
				}
			} // * Encanto *
			else if (strcmp(Buffer, "MD8S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dead_Ray_Damage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MD8S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dead_Ray_Range[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD8S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dead_Ray_Mana[i] = MeuInt;
				}
			} // * Raio da Morte *
			// [Mago - 2ª Tier Fim]

			// [Mago - 3ª Tier início]
			else if (strcmp(Buffer, "MD9S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Energy_Shield_DecDamage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD9S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Energy_Shield_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD9S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Energy_Shield_UseMana[i] = MeuInt;
				}
			} // * Escudo Energético *
			else if (strcmp(Buffer, "MD10S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Diastrophism_Damage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MD10S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Diastrophism_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD10S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Diastrophism_UseMana[i] = MeuInt;
				}
			} // * Terremoto *
			else if (strcmp(Buffer, "MD11S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuFloat = (float)atof(Buffer);
					Spirit_Elemental_RegenMana[i] = MeuFloat;
				}
			}
			else if (strcmp(Buffer, "MD11S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spirit_Elemental_Damage[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD11S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spirit_Elemental_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD11S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Spirit_Elemental_UseMana[i] = MeuInt;
				}
			} // * Espírito Elemental *
			else if (strcmp(Buffer, "MD12S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dancing_Sword_IceDamage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD12S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dancing_Sword_IceDamage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD12S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dancing_Sword_FireDamage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD12S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dancing_Sword_FireDamage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD12S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dancing_Sword_AttackDelay[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD12S6A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dancing_Sword_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD12S7A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Dancing_Sword_UseMana[i] = MeuInt;
				}
			} // * Espada Dançante *
			// [Mago - 3ª Tier Fim]

			// [Mago - 4ª Tier início]
			else if (strcmp(Buffer, "MD13S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Fire_Elemental_Damage[i][0] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD13S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Fire_Elemental_Damage[i][1] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD13S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Fire_Elemental_Hit[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD13S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Fire_Elemental_Life[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD13S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Fire_Elemental_UseMana[i] = MeuInt;
				}
			} // * Chama Elemental *
			else if (strcmp(Buffer, "MD14S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Flame_Wave_FireDamage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MD14S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Flame_Wave_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD14S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Flame_Wave_UseMana[i] = MeuInt;
				}
			} // * Ondas de Fogo *
			else if (strcmp(Buffer, "MD15S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Distortion_SpeedSubPercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD15S2A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Distortion_DamageSubPercent[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD15S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Distortion_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD15S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Distortion_Time[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD15S5A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Distortion_UseMana[i] = MeuInt;
				}
			} // * Distorção *
			else if (strcmp(Buffer, "MD16S1A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Meteo_Damage[i] = MeuInt;
				}
			}

			else if (strcmp(Buffer, "MD16S3A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Meteo_Area[i] = MeuInt;
				}
			}
			else if (strcmp(Buffer, "MD16S4A=") == 0)
			{
				for (int i = 0; i < 10; i++)
				{
					fscanf_s(fp, "%s", Buffer, sizeof(Buffer));
					MeuInt = atoi(Buffer);
					Meteo_UseMana[i] = MeuInt;
				}
			} // * Meteóro *
			// [Mago - 4ª Tier Fim]
		}
		fclose(fp);
	}
}

// Leitura executada quando skills forem recarregadas e quando o servidor for aberto
void Skills::readAll()
{
	readMechanician();
	readFighter();
	readPikeman();
	readArcher();
	readKnight();
	readAtalanta();
	readPriestess();
	readMagician();
}


extern rsPLAYINFO* rsPlayInfo;

void Skills::send(rsPLAYINFO* player, skillPacket* packet) const
{
	if (player && packet)
	{
		if (player->lpsmSock)
		{
			player->lpsmSock->Send((char*)packet, packet->size, TRUE);
		}
	}
	else
	{
		for (int cnt = 0; cnt < CONNECTMAX; cnt++)
		{
			if (rsPlayInfo[cnt].lpsmSock)
			{
				rsPlayInfo[cnt].lpsmSock->Send((char*)packet, packet->size, TRUE);
			}
		}
	}
}

void Skills::sendMechanician(rsPLAYINFO* player) const
{
	int classId = GROUP_MECHANICIAN;

	E_Shield e_shield;

	e_shield.classId = classId;
	e_shield.skillId = SKILL_EXTREME_SHIELD;
	e_shield.size = sizeof(E_Shield);
	CopyMemory(&e_shield.E_Shield_BlockRate, E_Shield_BlockRate, sizeof(e_shield.E_Shield_BlockRate));
	CopyMemory(&e_shield.E_Shield_UseMana, E_Shield_UseMana, sizeof(e_shield.E_Shield_UseMana));
	CopyMemory(&e_shield.E_Shield_UseTime, E_Shield_UseTime, sizeof(e_shield.E_Shield_UseTime));

	this->send(player, &e_shield);

	Mechanic_Bomb mechanic_Bomb;

	mechanic_Bomb.classId = classId;
	mechanic_Bomb.skillId = SKILL_MECHANIC_BOMB;
	mechanic_Bomb.size = sizeof(Mechanic_Bomb);

	CopyMemory(&mechanic_Bomb.Mechanic_Bomb_Damage, &Mechanic_Bomb_Damage, sizeof(mechanic_Bomb.Mechanic_Bomb_Damage));
	CopyMemory(&mechanic_Bomb.Mechanic_Bomb_Attack_Range, &Mechanic_Bomb_Attack_Range, sizeof(mechanic_Bomb.Mechanic_Bomb_Attack_Range));
	CopyMemory(&mechanic_Bomb.Mechanic_Bomb_UseMana, &Mechanic_Bomb_UseMana, sizeof(mechanic_Bomb.Mechanic_Bomb_UseMana));

	this->send(player, &mechanic_Bomb);

	Physical_Absorb physical_Absorb;

	physical_Absorb.classId = classId;
	physical_Absorb.skillId = SKILL_PHYSICAL_ABSORB;
	physical_Absorb.size = sizeof(Physical_Absorb);

	CopyMemory(&physical_Absorb.P_Absorb, &P_Absorb, sizeof(physical_Absorb.P_Absorb));
	CopyMemory(&physical_Absorb.P_Absorb_UseTime, &P_Absorb_UseTime, sizeof(physical_Absorb.P_Absorb_UseTime));
	CopyMemory(&physical_Absorb.P_Absor_bUseMana, &P_Absor_bUseMana, sizeof(physical_Absorb.P_Absor_bUseMana));

	this->send(player, &physical_Absorb);

	Poison_Resistence poison_Resistence;

	poison_Resistence.classId = classId;
	poison_Resistence.skillId = SKILL_POISON_ATTRIBUTE;
	poison_Resistence.size = sizeof(Poison_Resistence);

	CopyMemory(&poison_Resistence.PlusPoison, &PlusPoison, sizeof(poison_Resistence.PlusPoison));

	this->send(player, &poison_Resistence);

	G_Smash g_Smash;

	g_Smash.classId = classId;
	g_Smash.skillId = SKILL_GREAT_SMASH;
	g_Smash.size = sizeof(G_Smash);

	CopyMemory(&g_Smash.G_Smash_Attack_Rate, &G_Smash_Attack_Rate, sizeof(g_Smash.G_Smash_Attack_Rate));
	CopyMemory(&g_Smash.G_Smash_Attack_Rage, &G_Smash_Attack_Rage, sizeof(g_Smash.G_Smash_Attack_Rage));
	CopyMemory(&g_Smash.G_Smash_UseMana, &G_Smash_UseMana, sizeof(g_Smash.G_Smash_UseMana));

	this->send(player, &g_Smash);

	Maximize maximize;

	maximize.classId = classId;
	maximize.skillId = SKILL_MAXIMIZE;
	maximize.size = sizeof(Maximize);

	CopyMemory(&maximize.Maximize_Damage, &Maximize_Damage, sizeof(maximize.Maximize_Damage));
	CopyMemory(&maximize.Maximize_Time, &Maximize_Time, sizeof(maximize.Maximize_Time));
	CopyMemory(&maximize.Maximize_UseMana, &Maximize_UseMana, sizeof(maximize.Maximize_UseMana));

	this->send(player, &maximize);

	Automation automation;

	automation.classId = classId;
	automation.skillId = SKILL_AUTOMATION;
	automation.size = sizeof(Automation);

	CopyMemory(&automation.Automation_Damage, &Automation_Damage, sizeof(automation.Automation_Damage));
	CopyMemory(&automation.Automation_Speed, &Automation_Speed, sizeof(automation.Automation_Speed));
	CopyMemory(&automation.Automation_Time, &Automation_Time, sizeof(automation.Automation_Time));
	CopyMemory(&automation.Automation_UseMana, &Automation_UseMana, sizeof(automation.Automation_UseMana));

	this->send(player, &automation);

	Spark spark;

	spark.classId = classId;
	spark.skillId = SKILL_SPARK;
	spark.size = sizeof(Spark);

	CopyMemory(&spark.Spark_Damage, &Spark_Damage, sizeof(spark.Spark_Damage));
	CopyMemory(&spark.Spark_Area, &Spark_Area, sizeof(spark.Spark_Area));
	CopyMemory(&spark.Spark_UseMana, &Spark_UseMana, sizeof(spark.Spark_UseMana));

	this->send(player, &spark);

	Metal_Armor metal_Armor;

	metal_Armor.classId = classId;
	metal_Armor.skillId = SKILL_METAL_ARMOR;
	metal_Armor.size = sizeof(Metal_Armor);

	CopyMemory(&metal_Armor.Metal_Armor_Defense, &Metal_Armor_Defense, sizeof(metal_Armor.Metal_Armor_Defense));
	CopyMemory(&metal_Armor.Metal_Armor_Time, &Metal_Armor_Time, sizeof(metal_Armor.Metal_Armor_Time));
	CopyMemory(&metal_Armor.Metal_Armor_UseMana, &Metal_Armor_UseMana, sizeof(metal_Armor.Metal_Armor_UseMana));

	this->send(player, &metal_Armor);

	Grand_Smash grand_Smash;

	grand_Smash.classId = classId;
	grand_Smash.skillId = SKILL_GRAND_SMASH;
	grand_Smash.size = sizeof(Grand_Smash);

	CopyMemory(&grand_Smash.Grand_Smash_AttackRate, &Grand_Smash_AttackRate, sizeof(grand_Smash.Grand_Smash_AttackRate));
	CopyMemory(&grand_Smash.Grand_Smash_Damage, &Grand_Smash_Damage, sizeof(grand_Smash.Grand_Smash_Damage));
	CopyMemory(&grand_Smash.Grand_Smash_UseMana, &Grand_Smash_UseMana, sizeof(grand_Smash.Grand_Smash_UseMana));

	this->send(player, &grand_Smash);

	Mechanician_Weapon_Mastery mechanician_Weapon_Mastery;

	mechanician_Weapon_Mastery.classId = classId;
	mechanician_Weapon_Mastery.skillId = SKILL_MECHANIC_WEAPON;
	mechanician_Weapon_Mastery.size = sizeof(Mechanician_Weapon_Mastery);

	CopyMemory(&mechanician_Weapon_Mastery.M_Weapon_Mastey, &M_Weapon_Mastey, sizeof(mechanician_Weapon_Mastery.M_Weapon_Mastey));


	this->send(player, &mechanician_Weapon_Mastery);

	Spark_Shield spark_Shield;

	spark_Shield.classId = classId;
	spark_Shield.skillId = SKILL_SPARK_SHIELD;
	spark_Shield.size = sizeof(Spark_Shield);

	CopyMemory(&spark_Shield.Spark_Shield_Damage, &Spark_Shield_Damage, sizeof(spark_Shield.Spark_Shield_Damage));
	CopyMemory(&spark_Shield.Spark_Shield_Defense, &Spark_Shield_Defense, sizeof(spark_Shield.Spark_Shield_Defense));
	CopyMemory(&spark_Shield.Spark_Shield_Time, &Spark_Shield_Time, sizeof(spark_Shield.Spark_Shield_Time));
	CopyMemory(&spark_Shield.Spark_Shield_UseMana, &Spark_Shield_UseMana, sizeof(spark_Shield.Spark_Shield_UseMana));

	this->send(player, &spark_Shield);

	Impulsion impulsion;

	impulsion.classId = classId;
	impulsion.skillId = SKILL_IMPULSION;
	impulsion.size = sizeof(Impulsion);

	CopyMemory(&impulsion.Impulsion_LightingDamage, &Impulsion_LightingDamage, sizeof(impulsion.Impulsion_LightingDamage));
	CopyMemory(&impulsion.Impulsion_Range, &Impulsion_Range, sizeof(impulsion.Impulsion_Range));
	CopyMemory(&impulsion.Impulsion_LightingNum, &Impulsion_LightingNum, sizeof(impulsion.Impulsion_LightingNum));
	CopyMemory(&impulsion.Impulsion_UseMana, &Impulsion_UseMana, sizeof(impulsion.Impulsion_UseMana));

	this->send(player, &impulsion);

	Compulsion compulsion;

	compulsion.classId = classId;
	compulsion.skillId = SKILL_COMPULSION;
	compulsion.size = sizeof(Compulsion);

	CopyMemory(&compulsion.Compulsion_AddAbsorb, &Compulsion_AddAbsorb, sizeof(compulsion.Compulsion_AddAbsorb));
	CopyMemory(&compulsion.Compulsion_Area, &Compulsion_Area, sizeof(compulsion.Compulsion_Area));
	CopyMemory(&compulsion.Compulsion_Time, &Compulsion_Time, sizeof(compulsion.Compulsion_Time));
	CopyMemory(&compulsion.Compulsion_UseMana, &Compulsion_UseMana, sizeof(compulsion.Compulsion_UseMana));

	this->send(player, &compulsion);

	Magnetic_Sphere magnetic_Sphere;

	magnetic_Sphere.classId = classId;
	magnetic_Sphere.skillId = SKILL_MAGNETIC_SPHERE;
	magnetic_Sphere.size = sizeof(Magnetic_Sphere);

	CopyMemory(&magnetic_Sphere.Magnetic_Sphere_AddDamage, &Magnetic_Sphere_AddDamage, sizeof(magnetic_Sphere.Magnetic_Sphere_AddDamage));
	CopyMemory(&magnetic_Sphere.Magnetic_Sphere_Area, &Magnetic_Sphere_Area, sizeof(magnetic_Sphere.Magnetic_Sphere_Area));
	CopyMemory(&magnetic_Sphere.Magnetic_Sphere_AttackDelay, &Magnetic_Sphere_AttackDelay, sizeof(magnetic_Sphere.Magnetic_Sphere_AttackDelay));
	CopyMemory(&magnetic_Sphere.Magnetic_Sphere_Time, &Magnetic_Sphere_Time, sizeof(magnetic_Sphere.Magnetic_Sphere_Time));
	CopyMemory(&magnetic_Sphere.Magnetic_Sphere_UseMana, &Magnetic_Sphere_UseMana, sizeof(magnetic_Sphere.Magnetic_Sphere_UseMana));

	this->send(player, &magnetic_Sphere);

	Metal_Golem metal_Golem;

	metal_Golem.classId = classId;
	metal_Golem.skillId = SKILL_METAL_GOLEM;
	metal_Golem.size = sizeof(Metal_Golem);

	CopyMemory(&metal_Golem.Metal_Golem_Damage, &Metal_Golem_Damage, sizeof(metal_Golem.Metal_Golem_Damage));
	CopyMemory(&metal_Golem.Metal_Golem_Hit, &Metal_Golem_Hit, sizeof(metal_Golem.Metal_Golem_Hit));
	CopyMemory(&metal_Golem.Metal_Golem_Defense, &Metal_Golem_Defense, sizeof(metal_Golem.Metal_Golem_Defense));
	CopyMemory(&metal_Golem.Metal_Golem_Life, &Metal_Golem_Life, sizeof(metal_Golem.Metal_Golem_Life));
	CopyMemory(&metal_Golem.Metal_Golem_UseMana, &Metal_Golem_UseMana, sizeof(metal_Golem.Metal_Golem_UseMana));

	this->send(player, &metal_Golem);


}
void Skills::sendFighter(rsPLAYINFO* player) const
{
	int classId = GROUP_FIGHTER;

	Melee_Mastery melee_mastery;

	melee_mastery.classId = classId;
	melee_mastery.skillId = SKILL_MELEE_MASTERY;
	melee_mastery.size = sizeof(Melee_Mastery);
	CopyMemory(&melee_mastery.Melee_Mastery_DamagePercent, Melee_Mastery_DamagePercent, sizeof(melee_mastery.Melee_Mastery_DamagePercent));

	this->send(player, &melee_mastery);

	Plus_Fire plus_fire;

	plus_fire.classId = classId;
	plus_fire.skillId = SKILL_FIRE_ATTRIBUTE;
	plus_fire.size = sizeof(Plus_Fire);
	CopyMemory(&plus_fire.PlusFire, PlusFire, sizeof(plus_fire.PlusFire));

	this->send(player, &plus_fire);

	Raving raving;

	raving.classId = classId;
	raving.skillId = SKILL_RAVING;
	raving.size = sizeof(Raving);
	CopyMemory(&raving.Raving_Damage, Raving_Damage, sizeof(raving.Raving_Damage));
	CopyMemory(&raving.Ravind_Speed, Ravind_Speed, sizeof(raving.Ravind_Speed));
	CopyMemory(&raving.Raving_UseLife, Raving_UseLife, sizeof(raving.Raving_UseLife));
	CopyMemory(&raving.Raving_UseMana, Raving_UseMana, sizeof(raving.Raving_UseMana));

	this->send(player, &raving);

	Impact impact;

	impact.classId = classId;
	impact.skillId = SKILL_IMPACT;
	impact.size = sizeof(Impact);
	CopyMemory(&impact.Impact_Attack_Rating, Impact_Attack_Rating, sizeof(impact.Impact_Attack_Rating));
	CopyMemory(&impact.Impact_Damage, Impact_Damage, sizeof(impact.Impact_Damage));
	CopyMemory(&impact.Impact_UseMana, Impact_UseMana, sizeof(impact.Impact_UseMana));

	this->send(player, &impact);

	T_Impact t_impact;

	t_impact.classId = classId;
	t_impact.skillId = SKILL_TRIPLE_IMPACT;
	t_impact.size = sizeof(T_Impact);
	CopyMemory(&t_impact.T_Impact_Damage, T_Impact_Damage, sizeof(t_impact.T_Impact_Damage));
	CopyMemory(&t_impact.T_Impact_Hit, T_Impact_Hit, sizeof(t_impact.T_Impact_Hit));
	CopyMemory(&t_impact.T_Impact_UseMana, T_Impact_UseMana, sizeof(t_impact.T_Impact_UseMana));

	this->send(player, &t_impact);

	B_Swing b_swing;

	b_swing.classId = classId;
	b_swing.skillId = SKILL_BRUTAL_SWING;
	b_swing.size = sizeof(B_Swing);
	CopyMemory(&b_swing.B_Swing_Damage, B_Swing_Damage, sizeof(b_swing.B_Swing_Damage));
	CopyMemory(&b_swing.B_Swing_Critical, B_Swing_Critical, sizeof(b_swing.B_Swing_Critical));
	CopyMemory(&b_swing.B_Swing_UseMana, B_Swing_UseMana, sizeof(b_swing.B_Swing_UseMana));

	this->send(player, &b_swing);

	Roar roar;

	roar.classId = classId;
	roar.skillId = SKILL_ROAR;
	roar.size = sizeof(Roar);
	CopyMemory(&roar.Roar_Range, Roar_Range, sizeof(roar.Roar_Range));
	CopyMemory(&roar.Roar_Time, Roar_Time, sizeof(roar.Roar_Time));
	CopyMemory(&roar.Roar_UseMana, Roar_UseMana, sizeof(roar.Roar_UseMana));

	this->send(player, &roar);

	R_Zecram r_zecram;

	r_zecram.classId = classId;
	r_zecram.skillId = SKILL_RAGE_OF_ZECRAM;
	r_zecram.size = sizeof(R_Zecram);
	CopyMemory(&r_zecram.R_Zecram_Damage, R_Zecram_Damage, sizeof(r_zecram.R_Zecram_Damage));
	CopyMemory(&r_zecram.R_Zecram_UseMana, R_Zecram_UseMana, sizeof(r_zecram.R_Zecram_UseMana));

	this->send(player, &r_zecram);

	Concentration concentration;

	concentration.classId = classId;
	concentration.skillId = SKILL_CONCENTRATION;
	concentration.size = sizeof(Concentration);
	CopyMemory(&concentration.Concentration_AttackRate, Concentration_AttackRate, sizeof(concentration.Concentration_AttackRate));
	CopyMemory(&concentration.Concentration_Time, Concentration_Time, sizeof(concentration.Concentration_Time));
	CopyMemory(&concentration.Concentration_UseMana, Concentration_UseMana, sizeof(concentration.Concentration_UseMana));

	this->send(player, &concentration);

	A_Crash a_crash;

	a_crash.classId = classId;
	a_crash.skillId = SKILL_AVANGING_CRASH;
	a_crash.size = sizeof(A_Crash);
	CopyMemory(&a_crash.A_Crash_Damage, A_Crash_Damage, sizeof(a_crash.A_Crash_Damage));
	CopyMemory(&a_crash.A_Crash_AttackRate, A_Crash_AttackRate, sizeof(a_crash.A_Crash_AttackRate));
	CopyMemory(&a_crash.A_Crash_UseMana, A_Crash_UseMana, sizeof(a_crash.A_Crash_UseMana));

	this->send(player, &a_crash);

	Swift_Axe swift_axe;

	swift_axe.classId = classId;
	swift_axe.skillId = SKILL_SWIFT_AXE;
	swift_axe.size = sizeof(Swift_Axe);
	CopyMemory(&swift_axe.Swift_Axe_Speed, Swift_Axe_Speed, sizeof(swift_axe.Swift_Axe_Speed));
	CopyMemory(&swift_axe.Swift_Axe_Time, Swift_Axe_Time, sizeof(swift_axe.Swift_Axe_Time));
	CopyMemory(&swift_axe.Swift_Axe_UseMana, Swift_Axe_UseMana, sizeof(swift_axe.Swift_Axe_UseMana));

	this->send(player, &swift_axe);

	B_Crash b_crash;

	b_crash.classId = classId;
	b_crash.skillId = SKILL_BONE_CRASH;
	b_crash.size = sizeof(B_Crash);
	CopyMemory(&b_crash.B_Crash_Damage, B_Crash_Damage, sizeof(b_crash.B_Crash_Damage));
	CopyMemory(&b_crash.B_Crash_DemonDamage, B_Crash_DemonDamage, sizeof(b_crash.B_Crash_DemonDamage));
	CopyMemory(&b_crash.B_Crash_UseMana, B_Crash_UseMana, sizeof(b_crash.B_Crash_UseMana));

	this->send(player, &b_crash);

	Destroyer destroyer;

	destroyer.classId = classId;
	destroyer.skillId = SKILL_DETORYER;
	destroyer.size = sizeof(Destroyer);
	CopyMemory(&destroyer.Destoryer_DamagePercent, Destoryer_DamagePercent, sizeof(destroyer.Destoryer_DamagePercent));
	CopyMemory(&destroyer.Destoryer_AddCritical, Destoryer_AddCritical, sizeof(destroyer.Destoryer_AddCritical));
	CopyMemory(&destroyer.Destoryer_UseMana, Destoryer_UseMana, sizeof(destroyer.Destoryer_UseMana));

	this->send(player, &destroyer);

	Berserker berserker;

	berserker.classId = classId;
	berserker.skillId = SKILL_BERSERKER;
	berserker.size = sizeof(Berserker);
	CopyMemory(&berserker.Berserker_AddAttack, Berserker_AddAttack, sizeof(berserker.Berserker_AddAttack));
	CopyMemory(&berserker.Berserker_SubAbsorb, Berserker_SubAbsorb, sizeof(berserker.Berserker_SubAbsorb));
	CopyMemory(&berserker.Berserker_Time, Berserker_Time, sizeof(berserker.Berserker_Time));
	CopyMemory(&berserker.Berserker_UseMana, Berserker_UseMana, sizeof(berserker.Berserker_UseMana));

	this->send(player, &berserker);

	Cyclone_Strike cyclone_strike;

	cyclone_strike.classId = classId;
	cyclone_strike.skillId = SKILL_CYCLONE_STRIKE;
	cyclone_strike.size = sizeof(Cyclone_Strike);
	CopyMemory(&cyclone_strike.Cyclone_Strike_DamagePercent, Cyclone_Strike_DamagePercent, sizeof(cyclone_strike.Cyclone_Strike_DamagePercent));
	CopyMemory(&cyclone_strike.Cyclone_Strike_AreaDamage, Cyclone_Strike_AreaDamage, sizeof(cyclone_strike.Cyclone_Strike_AreaDamage));
	CopyMemory(&cyclone_strike.Cyclone_Strike_AttackNum, Cyclone_Strike_AttackNum, sizeof(cyclone_strike.Cyclone_Strike_AttackNum));
	CopyMemory(&cyclone_strike.Cyclone_Strike_Area, Cyclone_Strike_Area, sizeof(cyclone_strike.Cyclone_Strike_Area));
	CopyMemory(&cyclone_strike.Cyclone_Strike_UseMana, Cyclone_Strike_UseMana, sizeof(cyclone_strike.Cyclone_Strike_UseMana));

	this->send(player, &cyclone_strike);

	Boost_Health boost_health;

	boost_health.classId = classId;
	boost_health.skillId = SKILL_BOOST_HEALTH;
	boost_health.size = sizeof(Boost_Health);
	CopyMemory(&boost_health.Boost_Health_Life, Boost_Health_Life, sizeof(boost_health.Boost_Health_Life));

	this->send(player, &boost_health);
}
void Skills::sendPikeman(rsPLAYINFO* player) const
{
	int classId = GROUP_PIKEMAN;

	Pike_Wind pike_wind;

	pike_wind.classId = classId;
	pike_wind.skillId = SKILL_PIKE_WIND;
	pike_wind.size = sizeof(Pike_Wind);

	CopyMemory(&pike_wind.Pike_Wind_Damage, Pike_Wind_Damage, sizeof(pike_wind.Pike_Wind_Damage));
	CopyMemory(&pike_wind.Pike_Wind_Push_Lenght, Pike_Wind_Push_Lenght, sizeof(pike_wind.Pike_Wind_Push_Lenght));
	CopyMemory(&pike_wind.Pike_Wind_UseMana, Pike_Wind_UseMana, sizeof(pike_wind.Pike_Wind_UseMana));

	this->send(player, &pike_wind);

	Plus_Ice plus_ice;

	plus_ice.classId = classId;
	plus_ice.skillId = SKILL_ICE_ATTRIBUTE;
	plus_ice.size = sizeof(Plus_Ice);
	CopyMemory(&plus_ice.PlusIce, PlusIce, sizeof(plus_ice.PlusIce));

	this->send(player, &plus_ice);

	Critical_Hit critical_hit;

	critical_hit.classId = classId;
	critical_hit.skillId = SKILL_CRITICAL_HIT;
	critical_hit.size = sizeof(Critical_Hit);
	CopyMemory(&critical_hit.Critical_Hit_Critical, Critical_Hit_Critical, sizeof(critical_hit.Critical_Hit_Critical));
	CopyMemory(&critical_hit.Critical_Hit_UseMana, Critical_Hit_UseMana, sizeof(critical_hit.Critical_Hit_UseMana));
	this->send(player, &critical_hit);

	Jumping_Crash jumping_crash;

	jumping_crash.classId = classId;
	jumping_crash.skillId = SKILL_JUMPING_CRASH;
	jumping_crash.size = sizeof(Jumping_Crash);
	CopyMemory(&jumping_crash.Jumping_Crash_UseMana, Jumping_Crash_UseMana, sizeof(jumping_crash.Jumping_Crash_UseMana));
	CopyMemory(&jumping_crash.Jumping_Crash_Attack_Rating, Jumping_Crash_Attack_Rating, sizeof(jumping_crash.Jumping_Crash_Attack_Rating));
	CopyMemory(&jumping_crash.Jumping_Crash_Damage, Jumping_Crash_Damage, sizeof(jumping_crash.Jumping_Crash_Damage));

	this->send(player, &jumping_crash);

	G_Pike g_pike;

	g_pike.classId = classId;
	g_pike.skillId = SKILL_GROUND_PIKE;
	g_pike.size = sizeof(G_Pike);
	CopyMemory(&g_pike.G_Pike_Damage, G_Pike_Damage, sizeof(g_pike.G_Pike_Damage));
	CopyMemory(&g_pike.G_Pike_Range, G_Pike_Range, sizeof(g_pike.G_Pike_Range));
	CopyMemory(&g_pike.G_Pike_UseMana, G_Pike_UseMana, sizeof(g_pike.G_Pike_UseMana));
	CopyMemory(&g_pike.G_Pike_Time, G_Pike_Time, sizeof(g_pike.G_Pike_Time));

	this->send(player, &g_pike);

	Tornado tornado;

	tornado.classId = classId;
	tornado.skillId = SKILL_TORNADO;
	tornado.size = sizeof(Tornado);
	CopyMemory(&tornado.Tornado_Damage, Tornado_Damage, sizeof(tornado.Tornado_Damage));
	CopyMemory(&tornado.Tornado_Stun, Tornado_Stun, sizeof(tornado.Tornado_Stun));
	CopyMemory(&tornado.Tornado_Range, Tornado_Range, sizeof(tornado.Tornado_Range));
	CopyMemory(&tornado.Tornado_UseMana, Tornado_UseMana, sizeof(tornado.Tornado_UseMana));

	this->send(player, &tornado);

	W_D_Mastery w_d_mastery;

	w_d_mastery.classId = classId;
	w_d_mastery.skillId = SKILL_WEAPONE_DEFENCE_MASTERY;
	w_d_mastery.size = sizeof(W_D_Mastery);
	CopyMemory(&w_d_mastery.W_D_Mastery_Block, W_D_Mastery_Block, sizeof(w_d_mastery.W_D_Mastery_Block));

	this->send(player, &w_d_mastery);

	Expansion expansion;

	expansion.classId = classId;
	expansion.skillId = SKILL_EXPANSION;
	expansion.size = sizeof(Expansion);
	CopyMemory(&expansion.Expansion_Size, Expansion_Size, sizeof(expansion.Expansion_Size));
	CopyMemory(&expansion.Expansion_Damage, Expansion_Damage, sizeof(expansion.Expansion_Damage));
	CopyMemory(&expansion.Expansion_UseMana, Expansion_UseMana, sizeof(expansion.Expansion_UseMana));

	this->send(player, &expansion);

	Venom_Spear venom_spear;

	venom_spear.classId = classId;
	venom_spear.skillId = SKILL_VENOM_SPEAR;
	venom_spear.size = sizeof(Venom_Spear);
	CopyMemory(&venom_spear.VenomSpear_Damage, VenomSpear_Damage, sizeof(venom_spear.VenomSpear_Damage));
	CopyMemory(&venom_spear.VenomSpear_Num, VenomSpear_Num, sizeof(venom_spear.VenomSpear_Num));
	CopyMemory(&venom_spear.VenomSpear_Time, VenomSpear_Time, sizeof(venom_spear.VenomSpear_Time));
	CopyMemory(&venom_spear.VenomSpear_Range, VenomSpear_Range, sizeof(venom_spear.VenomSpear_Range));
	CopyMemory(&venom_spear.VenomSpear_UseMana, VenomSpear_UseMana, sizeof(venom_spear.VenomSpear_UseMana));

	this->send(player, &venom_spear);

	Vanish vanish;

	vanish.classId = classId;
	vanish.skillId = SKILL_VANISH;
	vanish.size = sizeof(Vanish);
	CopyMemory(&vanish.Vanish_Damage, Vanish_Damage, sizeof(vanish.Vanish_Damage));
	CopyMemory(&vanish.Vanish_Range, Vanish_Range, sizeof(vanish.Vanish_Range));
	CopyMemory(&vanish.Vanish_Time, Vanish_Time, sizeof(vanish.Vanish_Time));
	CopyMemory(&vanish.Vanish_UseMana, Vanish_UseMana, sizeof(vanish.Vanish_UseMana));

	this->send(player, &vanish);

	Critical_Mastery critical_mastery;

	critical_mastery.classId = classId;
	critical_mastery.skillId = SKILL_CRITICAL_MASTERY;
	critical_mastery.size = sizeof(Critical_Mastery);
	CopyMemory(&critical_mastery.Critical_Mastery_Critical, Critical_Mastery_Critical, sizeof(critical_mastery.Critical_Mastery_Critical));

	this->send(player, &critical_mastery);

	Chain_Lance chain_lance;

	chain_lance.classId = classId;
	chain_lance.skillId = SKILL_CHAIN_LANCE;
	chain_lance.size = sizeof(Chain_Lance);
	CopyMemory(&chain_lance.Chain_Lance_Damage, Chain_Lance_Damage, sizeof(chain_lance.Chain_Lance_Damage));
	CopyMemory(&chain_lance.Chain_Lance_UseMana, Chain_Lance_UseMana, sizeof(chain_lance.Chain_Lance_UseMana));

	this->send(player, &chain_lance);

	Assassin_Eye assassin_eye;

	assassin_eye.classId = classId;
	assassin_eye.skillId = SKILL_ASSASSIN_EYE;
	assassin_eye.size = sizeof(Assassin_Eye);
	CopyMemory(&assassin_eye.Assassin_Eye_AddCritical, Assassin_Eye_AddCritical, sizeof(assassin_eye.Assassin_Eye_AddCritical));
	CopyMemory(&assassin_eye.Assassin_Eye_M_SubCritical, Assassin_Eye_M_SubCritical, sizeof(assassin_eye.Assassin_Eye_M_SubCritical));
	CopyMemory(&assassin_eye.Assassin_Eye_Time, Assassin_Eye_Time, sizeof(assassin_eye.Assassin_Eye_Time));
	CopyMemory(&assassin_eye.Assassin_Eye_UseMana, Assassin_Eye_UseMana, sizeof(assassin_eye.Assassin_Eye_UseMana));

	this->send(player, &assassin_eye);

	Charging_Strike charging_strike;

	charging_strike.classId = classId;
	charging_strike.skillId = SKILL_CHARGING_STRIKE;
	charging_strike.size = sizeof(Charging_Strike);
	CopyMemory(&charging_strike.Charging_Strike_DamagePercent, Charging_Strike_DamagePercent, sizeof(charging_strike.Charging_Strike_DamagePercent));
	CopyMemory(&charging_strike.Charging_Strike_CharingDamagePercent, Charging_Strike_CharingDamagePercent, sizeof(charging_strike.Charging_Strike_CharingDamagePercent));
	CopyMemory(&charging_strike.Charging_Strike_Time, Charging_Strike_Time, sizeof(charging_strike.Charging_Strike_Time));
	CopyMemory(&charging_strike.Charging_Strike_UseMana, Charging_Strike_UseMana, sizeof(charging_strike.Charging_Strike_UseMana));

	this->send(player, &charging_strike);

	Vague vague;

	vague.classId = classId;
	vague.skillId = SKILL_VAGUE;
	vague.size = sizeof(Vague);
	CopyMemory(&vague.Vague_EvasionPercent, Vague_EvasionPercent, sizeof(vague.Vague_EvasionPercent));
	CopyMemory(&vague.Vague_Time, Vague_Time, sizeof(vague.Vague_Time));
	CopyMemory(&vague.Vague_UseMana, Vague_UseMana, sizeof(vague.Vague_UseMana));

	this->send(player, &vague);

	Shadow_Master shadow_master;

	shadow_master.classId = classId;
	shadow_master.skillId = SKILL_SHADOW_MASTER;
	shadow_master.size = sizeof(Shadow_Master);
	CopyMemory(&shadow_master.Shadow_Master_DamagePercent, Shadow_Master_DamagePercent, sizeof(shadow_master.Shadow_Master_DamagePercent));
	CopyMemory(&shadow_master.Shadow_Master_ShadowNum, Shadow_Master_ShadowNum, sizeof(shadow_master.Shadow_Master_ShadowNum));
	CopyMemory(&shadow_master.Shadow_Master_UseMana, Shadow_Master_UseMana, sizeof(shadow_master.Shadow_Master_UseMana));

	this->send(player, &shadow_master);
}
void Skills::sendArcher(rsPLAYINFO* player) const
{
	int classId = GROUP_ARCHER;

	Scout_Hawk scout_hawk;

	scout_hawk.classId = classId;
	scout_hawk.skillId = SKILL_SCOUT_HAWK;
	scout_hawk.size = sizeof(Scout_Hawk);
	CopyMemory(&scout_hawk.Scout_Hawk_Attack_Rate, Scout_Hawk_Attack_Rate, sizeof(scout_hawk.Scout_Hawk_Attack_Rate));
	CopyMemory(&scout_hawk.Scout_Hawk_UseTime, Scout_Hawk_UseTime, sizeof(scout_hawk.Scout_Hawk_UseTime));
	CopyMemory(&scout_hawk.Scout_Hawk_UseMana, Scout_Hawk_UseMana, sizeof(scout_hawk.Scout_Hawk_UseMana));

	this->send(player, &scout_hawk);

	S_Mastery s_mastery;

	s_mastery.classId = classId;
	s_mastery.skillId = SKILL_SHOOTING_MASTERY;
	s_mastery.size = sizeof(S_Mastery);
	CopyMemory(&s_mastery.S_Mastery_DamagePercent, S_Mastery_DamagePercent, sizeof(s_mastery.S_Mastery_DamagePercent));

	this->send(player, &s_mastery);

	Wind_Arrow wind_arrow;

	wind_arrow.classId = classId;
	wind_arrow.skillId = SKILL_WIND_ARROW;
	wind_arrow.size = sizeof(Wind_Arrow);
	CopyMemory(&wind_arrow.Wind_Arrow_Speed, Wind_Arrow_Speed, sizeof(wind_arrow.Wind_Arrow_Speed));
	CopyMemory(&wind_arrow.Wind_Arrow_Damage, Wind_Arrow_Damage, sizeof(wind_arrow.Wind_Arrow_Damage));
	CopyMemory(&wind_arrow.Wind_Arrow_UseMana, Wind_Arrow_UseMana, sizeof(wind_arrow.Wind_Arrow_UseMana));

	this->send(player, &wind_arrow);

	Perfect_Aim perfect_aim;

	perfect_aim.classId = classId;
	perfect_aim.skillId = SKILL_PERFECT_AIM;
	perfect_aim.size = sizeof(Perfect_Aim);
	CopyMemory(&perfect_aim.Perfect_Aim_Attack_Rate, Perfect_Aim_Attack_Rate, sizeof(perfect_aim.Perfect_Aim_Attack_Rate));
	CopyMemory(&perfect_aim.Perfect_Aim_Damage, Perfect_Aim_Damage, sizeof(perfect_aim.Perfect_Aim_Damage));
	CopyMemory(&perfect_aim.Perfect_Aim_Damage_LV, Perfect_Aim_Damage_LV, sizeof(perfect_aim.Perfect_Aim_Damage_LV));
	CopyMemory(&perfect_aim.Perfect_Aim_UseMana, Perfect_Aim_UseMana, sizeof(perfect_aim.Perfect_Aim_UseMana));

	this->send(player, &perfect_aim);

	D_Eye d_eye;

	d_eye.classId = classId;
	d_eye.skillId = SKILL_DIONS_EYE;
	d_eye.size = sizeof(D_Eye);
	CopyMemory(&d_eye.D_Eye_Attack_Rate, D_Eye_Attack_Rate, sizeof(d_eye.D_Eye_Attack_Rate));

	this->send(player, &d_eye);

	Falcon falcon;

	falcon.classId = classId;
	falcon.skillId = SKILL_FALCON;
	falcon.size = sizeof(Falcon);
	CopyMemory(&falcon.Falcon_Damage, Falcon_Damage, sizeof(falcon.Falcon_Damage));
	CopyMemory(&falcon.Falcon_Time, Falcon_Time, sizeof(falcon.Falcon_Time));
	CopyMemory(&falcon.Falcon_UseMana, Falcon_UseMana, sizeof(falcon.Falcon_UseMana));

	this->send(player, &falcon);

	A_Rage a_rage;

	a_rage.classId = classId;
	a_rage.skillId = SKILL_ARROW_OF_RAGE;
	a_rage.size = sizeof(A_Rage);
	CopyMemory(&a_rage.A_Rage_ArrowNum, A_Rage_ArrowNum, sizeof(a_rage.A_Rage_ArrowNum));
	CopyMemory(&a_rage.A_Rage_Damage, A_Rage_Damage, sizeof(a_rage.A_Rage_Damage));
	CopyMemory(&a_rage.A_Rage_Area, A_Rage_Area, sizeof(a_rage.A_Rage_Area));
	CopyMemory(&a_rage.A_Rage_UseMana, A_Rage_UseMana, sizeof(a_rage.A_Rage_UseMana));

	this->send(player, &a_rage);

	Avalanche avalanche;

	avalanche.classId = classId;
	avalanche.skillId = SKILL_AVALANCHE;
	avalanche.size = sizeof(Avalanche);
	CopyMemory(&avalanche.Avalanche_Speed, Avalanche_Speed, sizeof(avalanche.Avalanche_Speed));
	CopyMemory(&avalanche.Avalanche_Damage, Avalanche_Damage, sizeof(avalanche.Avalanche_Damage));
	CopyMemory(&avalanche.Avalanche_ArrowNum, Avalanche_ArrowNum, sizeof(avalanche.Avalanche_ArrowNum));
	CopyMemory(&avalanche.Avalanche_UseMana, Avalanche_UseMana, sizeof(avalanche.Avalanche_UseMana));

	this->send(player, &avalanche);

	Elemental_Shot elemental_shot;

	elemental_shot.classId = classId;
	elemental_shot.skillId = SKILL_ELEMENTAL_SHOT;
	elemental_shot.size = sizeof(Elemental_Shot);
	CopyMemory(&elemental_shot.Elemental_Shot_Fire, Elemental_Shot_Fire, sizeof(elemental_shot.Elemental_Shot_Fire));
	CopyMemory(&elemental_shot.Elemental_Shot_Lightning, Elemental_Shot_Lightning, sizeof(elemental_shot.Elemental_Shot_Lightning));
	CopyMemory(&elemental_shot.Elemental_Shot_UseMana, Elemental_Shot_UseMana, sizeof(elemental_shot.Elemental_Shot_UseMana));

	this->send(player, &elemental_shot);

	Golden_Falcon golden_falcon;

	golden_falcon.classId = classId;
	golden_falcon.skillId = SKILL_GOLDEN_FALCON;
	golden_falcon.size = sizeof(Golden_Falcon);
	CopyMemory(&golden_falcon.Golden_Falcon_Damage, Golden_Falcon_Damage, sizeof(golden_falcon.Golden_Falcon_Damage));
	CopyMemory(&golden_falcon.Golden_Falcon_LifeRegen, Golden_Falcon_LifeRegen, sizeof(golden_falcon.Golden_Falcon_LifeRegen));
	CopyMemory(&golden_falcon.Golden_Falcon_Time, Golden_Falcon_Time, sizeof(golden_falcon.Golden_Falcon_Time));
	CopyMemory(&golden_falcon.Golden_Falcon_UseMana, Golden_Falcon_UseMana, sizeof(golden_falcon.Golden_Falcon_UseMana));

	this->send(player, &golden_falcon);

	Bomb_Shot bomb_shot;

	bomb_shot.classId = classId;
	bomb_shot.skillId = SKILL_BOMB_SHOT;
	bomb_shot.size = sizeof(Bomb_Shot);
	CopyMemory(&bomb_shot.BombShot_Damage, BombShot_Damage, sizeof(bomb_shot.BombShot_Damage));
	CopyMemory(&bomb_shot.BombShot_DemonDamage, BombShot_DemonDamage, sizeof(bomb_shot.BombShot_DemonDamage));
	CopyMemory(&bomb_shot.BombShot_AddDamage, BombShot_AddDamage, sizeof(bomb_shot.BombShot_AddDamage));
	CopyMemory(&bomb_shot.BombShot_Area, BombShot_Area, sizeof(bomb_shot.BombShot_Area));
	CopyMemory(&bomb_shot.BombShot_UseMana, BombShot_UseMana, sizeof(bomb_shot.BombShot_UseMana));

	this->send(player, &bomb_shot);

	Perforation perforation;

	perforation.classId = classId;
	perforation.skillId = SKILL_PERFORATION;
	perforation.size = sizeof(Perforation);
	CopyMemory(&perforation.Perforation_Damage, Perforation_Damage, sizeof(perforation.Perforation_Damage));
	CopyMemory(&perforation.Perforation_Critical, Perforation_Critical, sizeof(perforation.Perforation_Critical));
	CopyMemory(&perforation.Perforation_UseMana, Perforation_UseMana, sizeof(perforation.Perforation_UseMana));
	CopyMemory(&perforation.Perforation_Attack_Range, Perforation_Attack_Range, sizeof(perforation.Perforation_Attack_Range));

	this->send(player, &perforation);

	Recall_Wolverin recall_wolverin;

	recall_wolverin.classId = classId;
	recall_wolverin.skillId = SKILL_RECALL_WOLVERIN;
	recall_wolverin.size = sizeof(Recall_Wolverin);
	CopyMemory(&recall_wolverin.Recall_Wolverin_Damage, Recall_Wolverin_Damage, sizeof(recall_wolverin.Recall_Wolverin_Damage));
	CopyMemory(&recall_wolverin.Recall_Wolverin_Hit, Recall_Wolverin_Hit, sizeof(recall_wolverin.Recall_Wolverin_Hit));
	CopyMemory(&recall_wolverin.Recall_Wolverin_Defense, Recall_Wolverin_Defense, sizeof(recall_wolverin.Recall_Wolverin_Defense));
	CopyMemory(&recall_wolverin.Recall_Wolverin_Life, Recall_Wolverin_Life, sizeof(recall_wolverin.Recall_Wolverin_Life));
	CopyMemory(&recall_wolverin.Recall_Wolverin_UseMana, Recall_Wolverin_UseMana, sizeof(recall_wolverin.Recall_Wolverin_UseMana));

	this->send(player, &recall_wolverin);

	Evasion_Mastery evasion_mastery;

	evasion_mastery.classId = classId;
	evasion_mastery.skillId = SKILL_EVASION_MASTERY;
	evasion_mastery.size = sizeof(Evasion_Mastery);
	CopyMemory(&evasion_mastery.Evasion_Mastery_AddPercent, Evasion_Mastery_AddPercent, sizeof(evasion_mastery.Evasion_Mastery_AddPercent));
	CopyMemory(&evasion_mastery.Evasion_Mastery_AddSpeed, Evasion_Mastery_AddSpeed, sizeof(evasion_mastery.Evasion_Mastery_AddSpeed));
	CopyMemory(&evasion_mastery.Evasion_Mastery_AddReduce, Evasion_Mastery_AddReduce, sizeof(evasion_mastery.Evasion_Mastery_AddReduce));
	CopyMemory(&evasion_mastery.Evasion_Mastery_Time, Evasion_Mastery_Time, sizeof(evasion_mastery.Evasion_Mastery_Time));
	CopyMemory(&evasion_mastery.Evasion_Mastery_UseMana, Evasion_Mastery_UseMana, sizeof(evasion_mastery.Evasion_Mastery_UseMana));

	this->send(player, &evasion_mastery);

	Phoenix_Shot phoenix_shot;

	phoenix_shot.classId = classId;
	phoenix_shot.skillId = SKILL_PHOENIX_SHOT;
	phoenix_shot.size = sizeof(Phoenix_Shot);
	CopyMemory(&phoenix_shot.Phoenix_Shot_DamagePercent, Phoenix_Shot_DamagePercent, sizeof(phoenix_shot.Phoenix_Shot_DamagePercent));
	CopyMemory(&phoenix_shot.Phoenix_Shot_Time, Phoenix_Shot_Time, sizeof(phoenix_shot.Phoenix_Shot_Time));
	CopyMemory(&phoenix_shot.Phoenix_Shot_UseMana, Phoenix_Shot_UseMana, sizeof(phoenix_shot.Phoenix_Shot_UseMana));

	this->send(player, &phoenix_shot);

	Force_Of_Nature force_of_nature;

	force_of_nature.classId = classId;
	force_of_nature.skillId = SKILL_FORCE_OF_NATURE;
	force_of_nature.size = sizeof(Force_Of_Nature);
	CopyMemory(&force_of_nature.Force_Of_Nature_AddDamage, Force_Of_Nature_AddDamage, sizeof(force_of_nature.Force_Of_Nature_AddDamage));
	CopyMemory(&force_of_nature.Force_Of_Nature_AddHit, Force_Of_Nature_AddHit, sizeof(force_of_nature.Force_Of_Nature_AddHit));
	CopyMemory(&force_of_nature.Force_Of_Nature_FalconAddDamage, Force_Of_Nature_FalconAddDamage, sizeof(force_of_nature.Force_Of_Nature_FalconAddDamage));
	CopyMemory(&force_of_nature.Force_Of_Nature_Time, Force_Of_Nature_Time, sizeof(force_of_nature.Force_Of_Nature_Time));
	CopyMemory(&force_of_nature.Force_Of_Nature_UseMana, Force_Of_Nature_UseMana, sizeof(force_of_nature.Force_Of_Nature_UseMana));

	this->send(player, &force_of_nature);
}
void Skills::sendKnight(rsPLAYINFO* player) const
{
	int classId = GROUP_KNIGHT;

	Sword_Blast sword_blast;

	sword_blast.classId = classId;
	sword_blast.skillId = SKILL_SWORD_BLAST;
	sword_blast.size = sizeof(Sword_Blast);
	CopyMemory(&sword_blast.Sword_Blast_UseMana, Sword_Blast_UseMana, sizeof(sword_blast.Sword_Blast_UseMana));
	CopyMemory(&sword_blast.Sword_Blast_Damage, Sword_Blast_Damage, sizeof(sword_blast.Sword_Blast_Damage));
	CopyMemory(&sword_blast.Sword_Blast_ShootingRange, Sword_Blast_ShootingRange, sizeof(sword_blast.Sword_Blast_ShootingRange));

	this->send(player, &sword_blast);

	Holy_Body holy_body;

	holy_body.classId = classId;
	holy_body.skillId = SKILL_HOLY_BODY;
	holy_body.size = sizeof(Holy_Body);
	CopyMemory(&holy_body.HolyBody_UseMana, HolyBody_UseMana, sizeof(holy_body.HolyBody_UseMana));
	CopyMemory(&holy_body.HolyBody_Absorb, HolyBody_Absorb, sizeof(holy_body.HolyBody_Absorb));
	CopyMemory(&holy_body.HolyBody_Time, HolyBody_Time, sizeof(holy_body.HolyBody_Time));

	this->send(player, &holy_body);

	P_Traning p_traning;

	p_traning.classId = classId;
	p_traning.skillId = SKILL_PHYSICAL_TRANING;
	p_traning.size = sizeof(P_Traning);
	CopyMemory(&p_traning.P_Traning_Stamina, P_Traning_Stamina, sizeof(p_traning.P_Traning_Stamina));

	this->send(player, &p_traning);

	D_Crash d_crash;

	d_crash.classId = classId;
	d_crash.skillId = SKILL_DOUBLE_CRASH;
	d_crash.size = sizeof(D_Crash);
	CopyMemory(&d_crash.D_Crash_UseMana, D_Crash_UseMana, sizeof(d_crash.D_Crash_UseMana));
	CopyMemory(&d_crash.D_Crash_Damage, D_Crash_Damage, sizeof(d_crash.D_Crash_Damage));
	CopyMemory(&d_crash.D_Crash_Critical, D_Crash_Critical, sizeof(d_crash.D_Crash_Critical));

	this->send(player, &d_crash);

	Holy_Valor holy_valor;

	holy_valor.classId = classId;
	holy_valor.skillId = SKILL_HOLY_VALOR;
	holy_valor.size = sizeof(Holy_Valor);
	CopyMemory(&holy_valor.Holy_Valor_Damage, Holy_Valor_Damage, sizeof(holy_valor.Holy_Valor_Damage));
	CopyMemory(&holy_valor.Holy_Valor_Time, Holy_Valor_Time, sizeof(holy_valor.Holy_Valor_Time));
	CopyMemory(&holy_valor.Holy_Valor_Mana, Holy_Valor_Mana, sizeof(holy_valor.Holy_Valor_Mana));

	this->send(player, &holy_valor);

	Brandish brandish;

	brandish.classId = classId;
	brandish.skillId = SKILL_BRANDISH;
	brandish.size = sizeof(Brandish);
	CopyMemory(&brandish.Brandish_Range, Brandish_Range, sizeof(brandish.Brandish_Range));
	CopyMemory(&brandish.Brandish_Damage, Brandish_Damage, sizeof(brandish.Brandish_Damage));
	CopyMemory(&brandish.Brandish_Mana, Brandish_Mana, sizeof(brandish.Brandish_Mana));

	this->send(player, &brandish);

	Piercing piercing;

	piercing.classId = classId;
	piercing.skillId = SKILL_PIERCING;
	piercing.size = sizeof(Piercing);
	CopyMemory(&piercing.Piercing_Damage, Piercing_Damage, sizeof(piercing.Piercing_Damage));
	CopyMemory(&piercing.Piercing_Attack_Rating, Piercing_Attack_Rating, sizeof(piercing.Piercing_Attack_Rating));
	CopyMemory(&piercing.Piercing_Mana, Piercing_Mana, sizeof(piercing.Piercing_Mana));

	this->send(player, &piercing);

	Drastic_Spirit drastic_spirit;

	drastic_spirit.classId = classId;
	drastic_spirit.skillId = SKILL_DRASTIC_SPIRIT;
	drastic_spirit.size = sizeof(Drastic_Spirit);
	CopyMemory(&drastic_spirit.Drastic_Spirit_Defense, Drastic_Spirit_Defense, sizeof(drastic_spirit.Drastic_Spirit_Defense));
	CopyMemory(&drastic_spirit.Drastic_Spirit_Time, Drastic_Spirit_Time, sizeof(drastic_spirit.Drastic_Spirit_Time));
	CopyMemory(&drastic_spirit.Drastic_Spirit_Mana, Drastic_Spirit_Mana, sizeof(drastic_spirit.Drastic_Spirit_Mana));

	this->send(player, &drastic_spirit);

	Sword_Mastery sword_mastery;

	sword_mastery.classId = classId;
	sword_mastery.skillId = SKILL_SWORD_MASTERY;
	sword_mastery.size = sizeof(Sword_Mastery);
	CopyMemory(&sword_mastery.Sword_Mastery_DamagePercent, Sword_Mastery_DamagePercent, sizeof(sword_mastery.Sword_Mastery_DamagePercent));

	this->send(player, &sword_mastery);

	D_Inhalation d_inhalation;

	d_inhalation.classId = classId;
	d_inhalation.skillId = SKILL_DIVINE_INHALATION;
	d_inhalation.size = sizeof(D_Inhalation);
	CopyMemory(&d_inhalation.D_Inhalation_Life, D_Inhalation_Life, sizeof(d_inhalation.D_Inhalation_Life));
	CopyMemory(&d_inhalation.D_Inhalation_Block, D_Inhalation_Block, sizeof(d_inhalation.D_Inhalation_Block));
	CopyMemory(&d_inhalation.D_Inhalation_Time, D_Inhalation_Time, sizeof(d_inhalation.D_Inhalation_Time));
	CopyMemory(&d_inhalation.D_Inhalation_UseMana, D_Inhalation_UseMana, sizeof(d_inhalation.D_Inhalation_UseMana));

	this->send(player, &d_inhalation);

	H_Incantation h_incantation;

	h_incantation.classId = classId;
	h_incantation.skillId = SKILL_HOLY_INCANTATION;
	h_incantation.size = sizeof(H_Incantation);
	CopyMemory(&h_incantation.H_Incantation_Success, H_Incantation_Success, sizeof(h_incantation.H_Incantation_Success));
	CopyMemory(&h_incantation.H_Incantation_AddLife, H_Incantation_AddLife, sizeof(h_incantation.H_Incantation_AddLife));
	CopyMemory(&h_incantation.H_Incantation_Time, H_Incantation_Time, sizeof(h_incantation.H_Incantation_Time));
	CopyMemory(&h_incantation.H_Incantation_UseMana, H_Incantation_UseMana, sizeof(h_incantation.H_Incantation_UseMana));

	this->send(player, &h_incantation);

	Grand_Cross grand_cross;

	grand_cross.classId = classId;
	grand_cross.skillId = SKILL_GRAND_CROSS;
	grand_cross.size = sizeof(Grand_Cross);
	CopyMemory(&grand_cross.Grand_Cross_Damage, Grand_Cross_Damage, sizeof(grand_cross.Grand_Cross_Damage));
	CopyMemory(&grand_cross.Grand_Cross_AttackRate, Grand_Cross_AttackRate, sizeof(grand_cross.Grand_Cross_AttackRate));
	CopyMemory(&grand_cross.Grand_Cross_UndeadDamage, Grand_Cross_UndeadDamage, sizeof(grand_cross.Grand_Cross_UndeadDamage));
	CopyMemory(&grand_cross.Grand_Cross_UseMana, Grand_Cross_UseMana, sizeof(grand_cross.Grand_Cross_UseMana));

	this->send(player, &grand_cross);

	Sword_Of_Justice sword_of_justice;

	sword_of_justice.classId = classId;
	sword_of_justice.skillId = SKILL_SWORD_OF_JUSTICE;
	sword_of_justice.size = sizeof(Sword_Of_Justice);
	CopyMemory(&sword_of_justice.Sword_Of_Justice_DamagePercent, Sword_Of_Justice_DamagePercent, sizeof(sword_of_justice.Sword_Of_Justice_DamagePercent));
	CopyMemory(&sword_of_justice.Sword_Of_Justice_Area, Sword_Of_Justice_Area, sizeof(sword_of_justice.Sword_Of_Justice_Area));
	CopyMemory(&sword_of_justice.Sword_Of_Justice_UseMana, Sword_Of_Justice_UseMana, sizeof(sword_of_justice.Sword_Of_Justice_UseMana));

	this->send(player, &sword_of_justice);

	Godly_Shield godly_shield;

	godly_shield.classId = classId;
	godly_shield.skillId = SKILL_GODLY_SHIELD;
	godly_shield.size = sizeof(Godly_Shield);
	CopyMemory(&godly_shield.Godly_Shield_AbsorbPercent, Godly_Shield_AbsorbPercent, sizeof(godly_shield.Godly_Shield_AbsorbPercent));
	CopyMemory(&godly_shield.Godly_Shield_Time, Godly_Shield_Time, sizeof(godly_shield.Godly_Shield_Time));
	CopyMemory(&godly_shield.Godly_Shield_UseMana, Godly_Shield_UseMana, sizeof(godly_shield.Godly_Shield_UseMana));

	this->send(player, &godly_shield);

	God_Bless god_bless;

	god_bless.classId = classId;
	god_bless.skillId = SKILL_GOD_BLESS;
	god_bless.size = sizeof(God_Bless);
	CopyMemory(&god_bless.God_Bless_AddDamage, God_Bless_AddDamage, sizeof(god_bless.God_Bless_AddDamage));
	CopyMemory(&god_bless.God_Bless_Time, God_Bless_Time, sizeof(god_bless.God_Bless_Time));
	CopyMemory(&god_bless.God_Bless_UseMana, God_Bless_UseMana, sizeof(god_bless.God_Bless_UseMana));

	this->send(player, &god_bless);

	Divine_Piercing divine_piercing;

	divine_piercing.classId = classId;
	divine_piercing.skillId = SKILL_DIVINE_PIERCING;
	divine_piercing.size = sizeof(Divine_Piercing);
	CopyMemory(&divine_piercing.Divine_Piercing_DamagePercent, Divine_Piercing_DamagePercent, sizeof(divine_piercing.Divine_Piercing_DamagePercent));
	CopyMemory(&divine_piercing.Divine_Piercing_AddHit, Divine_Piercing_AddHit, sizeof(divine_piercing.Divine_Piercing_AddHit));
	CopyMemory(&divine_piercing.Divine_Piercing_AttackNum, Divine_Piercing_AttackNum, sizeof(divine_piercing.Divine_Piercing_AttackNum));
	CopyMemory(&divine_piercing.Divine_Piercing_UseMana, Divine_Piercing_UseMana, sizeof(divine_piercing.Divine_Piercing_UseMana));

	this->send(player, &divine_piercing);
}
void Skills::sendAtalanta(rsPLAYINFO* player) const
{
	int classId = GROUP_ATALANTA;

	S_Strike s_strike;

	s_strike.classId = classId;
	s_strike.skillId = SKILL_SHIELD_STRIKE;
	s_strike.size = sizeof(S_Strike);
	CopyMemory(&s_strike.S_Strike_UseMana, S_Strike_UseMana, sizeof(s_strike.S_Strike_UseMana));
	CopyMemory(&s_strike.S_Strike_Damage, S_Strike_Damage, sizeof(s_strike.S_Strike_Damage));

	this->send(player, &s_strike);

	Farina farina;

	farina.classId = classId;
	farina.skillId = SKILL_FARINA;
	farina.size = sizeof(Farina);
	CopyMemory(&farina.Farina_UseMana, Farina_UseMana, sizeof(farina.Farina_UseMana));
	CopyMemory(&farina.Farina_Speed, Farina_Speed, sizeof(farina.Farina_Speed));
	CopyMemory(&farina.Farina_AttackRate, Farina_AttackRate, sizeof(farina.Farina_AttackRate));

	this->send(player, &farina);

	T_Mastery t_mastery;

	t_mastery.classId = classId;
	t_mastery.skillId = SKILL_THROWING_MASTERY;
	t_mastery.size = sizeof(T_Mastery);
	CopyMemory(&t_mastery.T_Mastery_Damage, T_Mastery_Damage, sizeof(t_mastery.T_Mastery_Damage));

	this->send(player, &t_mastery);

	V_Spear v_spear;

	v_spear.classId = classId;
	v_spear.skillId = SKILL_VIGOR_SPEAR;
	v_spear.size = sizeof(V_Spear);
	CopyMemory(&v_spear.V_Spear_UseMana, V_Spear_UseMana, sizeof(v_spear.V_Spear_UseMana));
	CopyMemory(&v_spear.V_Spear_Damage, V_Spear_Damage, sizeof(v_spear.V_Spear_Damage));

	this->send(player, &v_spear);

	Wind wind;

	wind.classId = classId;
	wind.skillId = SKILL_WINDY;
	wind.size = sizeof(Wind);
	CopyMemory(&wind.Windy_Attack_Rating, Windy_Attack_Rating, sizeof(wind.Windy_Attack_Rating));
	CopyMemory(&wind.Windy_Time, Windy_Time, sizeof(wind.Windy_Time));
	CopyMemory(&wind.Windy_Mana, Windy_Mana, sizeof(wind.Windy_Mana));

	this->send(player, &wind);

	Twist_Javelin twist_javelin;

	twist_javelin.classId = classId;
	twist_javelin.skillId = SKILL_TWIST_JAVELIN;
	twist_javelin.size = sizeof(Twist_Javelin);
	CopyMemory(&twist_javelin.Twist_Javelin_Damage, Twist_Javelin_Damage, sizeof(twist_javelin.Twist_Javelin_Damage));
	CopyMemory(&twist_javelin.Twist_Javelin_Attack_Rating, Twist_Javelin_Attack_Rating, sizeof(twist_javelin.Twist_Javelin_Attack_Rating));
	CopyMemory(&twist_javelin.Twist_Javelin_Mana, Twist_Javelin_Mana, sizeof(twist_javelin.Twist_Javelin_Mana));

	this->send(player, &twist_javelin);

	Soul_Sucker soul_sucker;

	soul_sucker.classId = classId;
	soul_sucker.skillId = SKILL_SOUL_SUCKER;
	soul_sucker.size = sizeof(Soul_Sucker);
	CopyMemory(&soul_sucker.Soul_Sucker_Absorb, Soul_Sucker_Absorb, sizeof(soul_sucker.Soul_Sucker_Absorb));
	CopyMemory(&soul_sucker.Soul_Sucker_Mana, Soul_Sucker_Mana, sizeof(soul_sucker.Soul_Sucker_Mana));

	this->send(player, &soul_sucker);

	Fire_Javelin fire_javelin;

	fire_javelin.classId = classId;
	fire_javelin.skillId = SKILL_FIRE_JAVELIN;
	fire_javelin.size = sizeof(Fire_Javelin);
	CopyMemory(&fire_javelin.Fire_Javelin_Damage, Fire_Javelin_Damage, sizeof(fire_javelin.Fire_Javelin_Damage));
	CopyMemory(&fire_javelin.Fire_Javelin_Mana, Fire_Javelin_Mana, sizeof(fire_javelin.Fire_Javelin_Mana));

	this->send(player, &fire_javelin);

	Split_Javelin split_javelin;

	split_javelin.classId = classId;
	split_javelin.skillId = SKILL_SPLIT_JAVELIN;
	split_javelin.size = sizeof(Split_Javelin);
	CopyMemory(&split_javelin.Split_Javelin_AttackNum, Split_Javelin_AttackNum, sizeof(split_javelin.Split_Javelin_AttackNum));
	CopyMemory(&split_javelin.Split_Javelin_Damage, Split_Javelin_Damage, sizeof(split_javelin.Split_Javelin_Damage));
	CopyMemory(&split_javelin.Split_Javelin_AttackRate, Split_Javelin_AttackRate, sizeof(split_javelin.Split_Javelin_AttackRate));
	CopyMemory(&split_javelin.Split_Javelin_UseMana, Split_Javelin_UseMana, sizeof(split_javelin.Split_Javelin_UseMana));

	this->send(player, &split_javelin);

	T_Of_Valhalla t_of_valhalla;

	t_of_valhalla.classId = classId;
	t_of_valhalla.skillId = SKILL_TRIUMPH_OF_VALHALLA;
	t_of_valhalla.size = sizeof(T_Of_Valhalla);
	CopyMemory(&t_of_valhalla.T_Of_Valhalla_Damage, T_Of_Valhalla_Damage, sizeof(t_of_valhalla.T_Of_Valhalla_Damage));
	CopyMemory(&t_of_valhalla.T_Of_Valhalla_Time, T_Of_Valhalla_Time, sizeof(t_of_valhalla.T_Of_Valhalla_Time));
	CopyMemory(&t_of_valhalla.T_Of_Valhalla_UseMana, T_Of_Valhalla_UseMana, sizeof(t_of_valhalla.T_Of_Valhalla_UseMana));

	this->send(player, &t_of_valhalla);

	Lightning_Javelin lightning_javelin;

	lightning_javelin.classId = classId;
	lightning_javelin.skillId = SKILL_LIGHTNING_JAVELIN;
	lightning_javelin.size = sizeof(Lightning_Javelin);
	CopyMemory(&lightning_javelin.Lightning_Javelin_Damage, Lightning_Javelin_Damage, sizeof(lightning_javelin.Lightning_Javelin_Damage));
	CopyMemory(&lightning_javelin.Lightning_Javelin_UseMana, Lightning_Javelin_UseMana, sizeof(lightning_javelin.Lightning_Javelin_UseMana));

	this->send(player, &lightning_javelin);

	Storm_Javelin storm_javelin;

	storm_javelin.classId = classId;
	storm_javelin.skillId = SKILL_STORM_JAVELIN;
	storm_javelin.size = sizeof(Storm_Javelin);
	CopyMemory(&storm_javelin.Storm_Javelin_Damage, Storm_Javelin_Damage, sizeof(storm_javelin.Storm_Javelin_Damage));
	CopyMemory(&storm_javelin.Storm_Javelin_UseMana, Storm_Javelin_UseMana, sizeof(storm_javelin.Storm_Javelin_UseMana));

	this->send(player, &storm_javelin);

	Hall_Of_Valhalla hall_of_valhalla;

	hall_of_valhalla.classId = classId;
	hall_of_valhalla.skillId = SKILL_HALL_OF_VALHALLA;
	hall_of_valhalla.size = sizeof(Hall_Of_Valhalla);
	CopyMemory(&hall_of_valhalla.Hall_Of_Valhalla_AddEvation, Hall_Of_Valhalla_AddEvation, sizeof(hall_of_valhalla.Hall_Of_Valhalla_AddEvation));
	CopyMemory(&hall_of_valhalla.Hall_Of_Valhalla_Time, Hall_Of_Valhalla_Time, sizeof(hall_of_valhalla.Hall_Of_Valhalla_Time));
	CopyMemory(&hall_of_valhalla.Hall_Of_Valhalla_UseMana, Hall_Of_Valhalla_UseMana, sizeof(hall_of_valhalla.Hall_Of_Valhalla_UseMana));

	this->send(player, &hall_of_valhalla);

	X_Rage x_rage;

	x_rage.classId = classId;
	x_rage.skillId = SKILL_X_RAGE;
	x_rage.size = sizeof(X_Rage);
	CopyMemory(&x_rage.X_Rage_DamagePercent, X_Rage_DamagePercent, sizeof(x_rage.X_Rage_DamagePercent));
	CopyMemory(&x_rage.X_Rage_Area, X_Rage_Area, sizeof(x_rage.X_Rage_Area));
	CopyMemory(&x_rage.X_Rage_UseMana, X_Rage_UseMana, sizeof(x_rage.X_Rage_UseMana));

	this->send(player, &x_rage);

	Frost_Javelin frost_javelin;

	frost_javelin.classId = classId;
	frost_javelin.skillId = SKILL_FROST_JAVELIN;
	frost_javelin.size = sizeof(Frost_Javelin);
	CopyMemory(&frost_javelin.Frost_Javelin_IceAddDamage, Frost_Javelin_IceAddDamage, sizeof(frost_javelin.Frost_Javelin_IceAddDamage));
	CopyMemory(&frost_javelin.Frost_Javelin_SpeedSubPercent, Frost_Javelin_SpeedSubPercent, sizeof(frost_javelin.Frost_Javelin_SpeedSubPercent));
	CopyMemory(&frost_javelin.Frost_Javelin_IceTime, Frost_Javelin_IceTime, sizeof(frost_javelin.Frost_Javelin_IceTime));
	CopyMemory(&frost_javelin.Frost_Javelin_Time, Frost_Javelin_Time, sizeof(frost_javelin.Frost_Javelin_Time));
	CopyMemory(&frost_javelin.Frost_Javelin_UseMana, Frost_Javelin_UseMana, sizeof(frost_javelin.Frost_Javelin_UseMana));

	this->send(player, &frost_javelin);

	Vengeance vengeance;

	vengeance.classId = classId;
	vengeance.skillId = SKILL_VENGEANCE;
	vengeance.size = sizeof(Vengeance);
	CopyMemory(&vengeance.Vengeance_DamagePercent, Vengeance_DamagePercent, sizeof(vengeance.Vengeance_DamagePercent));
	CopyMemory(&vengeance.Vengeance_Range, Vengeance_Range, sizeof(vengeance.Vengeance_Range));
	CopyMemory(&vengeance.Vengeance_UseMana, Vengeance_UseMana, sizeof(vengeance.Vengeance_UseMana));
	CopyMemory(&vengeance.Vengeance_AddCritical, Vengeance_AddCritical, sizeof(vengeance.Vengeance_AddCritical));

	this->send(player, &vengeance);
}
void Skills::sendPriestess(rsPLAYINFO* player) const
{
	int classId = GROUP_PRIESTESS;

	Healing healing;

	healing.classId = classId;
	healing.skillId = SKILL_HEALING;
	healing.size = sizeof(Healing);
	CopyMemory(&healing.Healing_UseMana, Healing_UseMana, sizeof(healing.Healing_UseMana));
	CopyMemory(&healing.Healing_Heal, Healing_Heal, sizeof(healing.Healing_Heal));

	this->send(player, &healing);

	HolyBolt holybolt;

	holybolt.classId = classId;
	holybolt.skillId = SKILL_HOLY_BOLT;
	holybolt.size = sizeof(HolyBolt);
	CopyMemory(&holybolt.HolyBolt_UseMana, HolyBolt_UseMana, sizeof(holybolt.HolyBolt_UseMana));
	CopyMemory(&holybolt.HolyBolt_Damage, HolyBolt_Damage, sizeof(holybolt.HolyBolt_Damage));

	this->send(player, &holybolt);

	M_Spark m_spark;

	m_spark.classId = classId;
	m_spark.skillId = SKILL_MULTISPARK;
	m_spark.size = sizeof(M_Spark);
	CopyMemory(&m_spark.M_Spark_Damage, M_Spark_Damage, sizeof(m_spark.M_Spark_Damage));
	CopyMemory(&m_spark.M_Spark_Num, M_Spark_Num, sizeof(m_spark.M_Spark_Num));

	this->send(player, &m_spark);

	HolyMind holymind;

	holymind.classId = classId;
	holymind.skillId = SKILL_HOLY_MIND;
	holymind.size = sizeof(HolyMind);
	CopyMemory(&holymind.HolyMind_UseMana, HolyMind_UseMana, sizeof(holymind.HolyMind_UseMana));
	CopyMemory(&holymind.HolyMind_DecDamage, HolyMind_DecDamage, sizeof(holymind.HolyMind_DecDamage));
	CopyMemory(&holymind.HolyMind_Time, HolyMind_Time, sizeof(holymind.HolyMind_Time));

	this->send(player, &holymind);

	Meditation meditation;

	meditation.classId = classId;
	meditation.skillId = SKILL_MEDITATION;
	meditation.size = sizeof(Meditation);
	CopyMemory(&meditation.Meditation_Regen, Meditation_Regen, sizeof(meditation.Meditation_Regen));

	this->send(player, &meditation);

	Divine_Lightning divine_lightning;

	divine_lightning.classId = classId;
	divine_lightning.skillId = SKILL_DIVINE_LIGHTNING;
	divine_lightning.size = sizeof(Divine_Lightning);
	CopyMemory(&divine_lightning.Divine_Lightning_Damage, Divine_Lightning_Damage, sizeof(divine_lightning.Divine_Lightning_Damage));
	CopyMemory(&divine_lightning.Divine_Lightning_Num, Divine_Lightning_Num, sizeof(divine_lightning.Divine_Lightning_Num));
	CopyMemory(&divine_lightning.Divine_Lightning_Mana, Divine_Lightning_Mana, sizeof(divine_lightning.Divine_Lightning_Mana));

	this->send(player, &divine_lightning);

	Holy_Reflection holy_reflection;

	holy_reflection.classId = classId;
	holy_reflection.skillId = SKILL_HOLY_REFLECTION;
	holy_reflection.size = sizeof(Holy_Reflection);
	CopyMemory(&holy_reflection.Holy_Reflection_Return_Damage, Holy_Reflection_Return_Damage, sizeof(holy_reflection.Holy_Reflection_Return_Damage));
	CopyMemory(&holy_reflection.Holy_Reflection_Time, Holy_Reflection_Time, sizeof(holy_reflection.Holy_Reflection_Time));
	CopyMemory(&holy_reflection.Holy_Reflection_Mana, Holy_Reflection_Mana, sizeof(holy_reflection.Holy_Reflection_Mana));

	this->send(player, &holy_reflection);

	GrandHealing grandhealing;

	grandhealing.classId = classId;
	grandhealing.skillId = SKILL_GRAND_HEALING;
	grandhealing.size = sizeof(GrandHealing);
	CopyMemory(&grandhealing.Grand_Healing, Grand_Healing, sizeof(grandhealing.Grand_Healing));
	CopyMemory(&grandhealing.Grand_Healing_Range, Grand_Healing_Range, sizeof(grandhealing.Grand_Healing_Range));
	CopyMemory(&grandhealing.Grand_Healing_Mana, Grand_Healing_Mana, sizeof(grandhealing.Grand_Healing_Mana));

	this->send(player, &grandhealing);

	Vigor_Ball vigor_ball;

	vigor_ball.classId = classId;
	vigor_ball.skillId = SKILL_VIGOR_BALL;
	vigor_ball.size = sizeof(Vigor_Ball);
	CopyMemory(&vigor_ball.Vigor_Ball_Damage, Vigor_Ball_Damage, sizeof(vigor_ball.Vigor_Ball_Damage));
	CopyMemory(&vigor_ball.Vigor_Ball_UseMana, Vigor_Ball_UseMana, sizeof(vigor_ball.Vigor_Ball_UseMana));

	this->send(player, &vigor_ball);

	Resurrection resurrection;

	resurrection.classId = classId;
	resurrection.skillId = SKILL_RESURRECTION;
	resurrection.size = sizeof(Resurrection);
	CopyMemory(&resurrection.Resurrection_Percent, Resurrection_Percent, sizeof(resurrection.Resurrection_Percent));
	CopyMemory(&resurrection.Resurrection_UseMana, Resurrection_UseMana, sizeof(resurrection.Resurrection_UseMana));

	this->send(player, &resurrection);

	Extinction extinction;

	extinction.classId = classId;
	extinction.skillId = SKILL_EXTINCTION;
	extinction.size = sizeof(Extinction);
	CopyMemory(&extinction.Extinction_Percent, Extinction_Percent, sizeof(extinction.Extinction_Percent));
	CopyMemory(&extinction.Extinction_Amount, Extinction_Amount, sizeof(extinction.Extinction_Amount));
	CopyMemory(&extinction.Extinction_UseMana, Extinction_UseMana, sizeof(extinction.Extinction_UseMana));

	this->send(player, &extinction);

	Virtual_Life virtual_life;

	virtual_life.classId = classId;
	virtual_life.skillId = SKILL_VIRTUAL_LIFE;
	virtual_life.size = sizeof(Virtual_Life);
	CopyMemory(&virtual_life.Virtual_Life_Percent, Virtual_Life_Percent, sizeof(virtual_life.Virtual_Life_Percent));
	CopyMemory(&virtual_life.Virtual_Life_Time, Virtual_Life_Time, sizeof(virtual_life.Virtual_Life_Time));
	CopyMemory(&virtual_life.Virtual_Life_UseMana, Virtual_Life_UseMana, sizeof(virtual_life.Virtual_Life_UseMana));

	this->send(player, &virtual_life);

	Glacial_Spike glacial_spike;

	glacial_spike.classId = classId;
	glacial_spike.skillId = SKILL_GLACIAL_SPIKE;
	glacial_spike.size = sizeof(Glacial_Spike);
	CopyMemory(&glacial_spike.Glacial_Spike_Damage, Glacial_Spike_Damage, sizeof(glacial_spike.Glacial_Spike_Damage));
	CopyMemory(&glacial_spike.Glacial_Spike_Area, Glacial_Spike_Area, sizeof(glacial_spike.Glacial_Spike_Area));
	CopyMemory(&glacial_spike.Glacial_Spike_UseMana, Glacial_Spike_UseMana, sizeof(glacial_spike.Glacial_Spike_UseMana));

	this->send(player, &glacial_spike);

	Regeneration_Field regeneration_field;

	regeneration_field.classId = classId;
	regeneration_field.skillId = SKILL_REGENERATION_FIELD;
	regeneration_field.size = sizeof(Regeneration_Field);
	CopyMemory(&regeneration_field.Regeneration_Field_LifeRegen, Regeneration_Field_LifeRegen, sizeof(regeneration_field.Regeneration_Field_LifeRegen));
	CopyMemory(&regeneration_field.Regeneration_Field_ManaRegen, Regeneration_Field_ManaRegen, sizeof(regeneration_field.Regeneration_Field_ManaRegen));
	CopyMemory(&regeneration_field.Regeneration_Field_Time, Regeneration_Field_Time, sizeof(regeneration_field.Regeneration_Field_Time));
	CopyMemory(&regeneration_field.Regeneration_Field_Area, Regeneration_Field_Area, sizeof(regeneration_field.Regeneration_Field_Area));
	CopyMemory(&regeneration_field.Regeneration_Field_UseMana, Regeneration_Field_UseMana, sizeof(regeneration_field.Regeneration_Field_UseMana));

	this->send(player, &regeneration_field);

	Chain_Lightning chain_lightning;

	chain_lightning.classId = classId;
	chain_lightning.skillId = SKILL_CHAIN_LIGHTNING;
	chain_lightning.size = sizeof(Chain_Lightning);
	CopyMemory(&chain_lightning.Chain_Lightning_Damage, Chain_Lightning_Damage, sizeof(chain_lightning.Chain_Lightning_Damage));
	/*CopyMemory(&chain_lightning.Chain_Lightning_Num, Chain_Lightning_Num, sizeof(chain_lightning.Chain_Lightning_Num));*/
	CopyMemory(&chain_lightning.Chain_Lightning_Range, Chain_Lightning_Range, sizeof(chain_lightning.Chain_Lightning_Range));
	CopyMemory(&chain_lightning.Chain_Lightning_UseMana, Chain_Lightning_UseMana, sizeof(chain_lightning.Chain_Lightning_UseMana));

	this->send(player, &chain_lightning);

	Summon_Muspell summon_muspell;

	summon_muspell.classId = classId;
	summon_muspell.skillId = SKILL_SUMMON_MUSPELL;
	summon_muspell.size = sizeof(Summon_Muspell);
	CopyMemory(&summon_muspell.Summon_Muspell_Damage, Summon_Muspell_Damage, sizeof(summon_muspell.Summon_Muspell_Damage));
	CopyMemory(&summon_muspell.Summon_Muspell_Attack_Delay, Summon_Muspell_Attack_Delay, sizeof(summon_muspell.Summon_Muspell_Attack_Delay));
	CopyMemory(&summon_muspell.Summon_Muspell_UndeadAbsorbPercent, Summon_Muspell_UndeadAbsorbPercent, sizeof(summon_muspell.Summon_Muspell_UndeadAbsorbPercent));
	CopyMemory(&summon_muspell.Summon_Muspell_BlockPercent, Summon_Muspell_BlockPercent, sizeof(summon_muspell.Summon_Muspell_BlockPercent));
	CopyMemory(&summon_muspell.Summon_Muspell_Time, Summon_Muspell_Time, sizeof(summon_muspell.Summon_Muspell_Time));
	CopyMemory(&summon_muspell.Summon_Muspell_UseMana, Summon_Muspell_UseMana, sizeof(summon_muspell.Summon_Muspell_UseMana));

	this->send(player, &summon_muspell);
}
void Skills::sendMagician(rsPLAYINFO* player) const
{
	int classId = GROUP_MAGICIAN;

	Agony agony;

	agony.classId = classId;
	agony.skillId = SKILL_AGONY;
	agony.size = sizeof(Agony);
	CopyMemory(&agony.Agony_UseMana, Agony_UseMana, sizeof(agony.Agony_UseMana));
	CopyMemory(&agony.Agony_ConvLife, Agony_ConvLife, sizeof(agony.Agony_ConvLife));

	this->send(player, &agony);

	FireBolt firebolt;

	firebolt.classId = classId;
	firebolt.skillId = SKILL_FIRE_BOLT;
	firebolt.size = sizeof(FireBolt);
	CopyMemory(&firebolt.FireBolt_UseMana, FireBolt_UseMana, sizeof(firebolt.FireBolt_UseMana));
	CopyMemory(&firebolt.FireBolt_Damage, FireBolt_Damage, sizeof(firebolt.FireBolt_Damage));

	this->send(player, &firebolt);

	Zenith zenith;

	zenith.classId = classId;
	zenith.skillId = SKILL_ZENITH;
	zenith.size = sizeof(Zenith);
	CopyMemory(&zenith.Zenith_UseMana, Zenith_UseMana, sizeof(zenith.Zenith_UseMana));
	CopyMemory(&zenith.Zenith_Element, Zenith_Element, sizeof(zenith.Zenith_Element));
	CopyMemory(&zenith.Zenith_Time, Zenith_Time, sizeof(zenith.Zenith_Time));

	this->send(player, &zenith);

	FireBall fireball;

	fireball.classId = classId;
	fireball.skillId = SKILL_FIRE_BALL;
	fireball.size = sizeof(FireBall);
	CopyMemory(&fireball.FireBall_UseMana, FireBall_UseMana, sizeof(fireball.FireBall_UseMana));
	CopyMemory(&fireball.FireBall_Damage, FireBall_Damage, sizeof(fireball.FireBall_Damage));
	CopyMemory(&fireball.FireBall_Area, FireBall_Area, sizeof(fireball.FireBall_Area));
	CopyMemory(&fireball.FireBall_Range, FireBall_Range, sizeof(fireball.FireBall_Range));

	this->send(player, &fireball);

	Mental_Mastery mental_mastery;

	mental_mastery.classId = classId;
	mental_mastery.skillId = SKILL_MENTAL_MASTERY;
	mental_mastery.size = sizeof(Mental_Mastery);
	CopyMemory(&mental_mastery.Mental_Mastery_Mana, Mental_Mastery_Mana, sizeof(mental_mastery.Mental_Mastery_Mana));

	this->send(player, &mental_mastery);

	Watornado watornado;

	watornado.classId = classId;
	watornado.skillId = SKILL_WATORNADO;
	watornado.size = sizeof(Watornado);
	CopyMemory(&watornado.Watornado_Damage, Watornado_Damage, sizeof(watornado.Watornado_Damage));
	CopyMemory(&watornado.Watornado_Area, Watornado_Area, sizeof(watornado.Watornado_Area));
	CopyMemory(&watornado.Watornado_Range, Watornado_Range, sizeof(watornado.Watornado_Range));
	CopyMemory(&watornado.Watornado_Mana, Watornado_Mana, sizeof(watornado.Watornado_Mana));

	this->send(player, &watornado);

	Enchant_Weapon enchant_weapon;

	enchant_weapon.classId = classId;
	enchant_weapon.skillId = SKILL_ENCHANT_WEAPON;
	enchant_weapon.size = sizeof(Enchant_Weapon);
	CopyMemory(&enchant_weapon.Enchant_Weapon_Damage_Ice, Enchant_Weapon_Damage_Ice, sizeof(enchant_weapon.Enchant_Weapon_Damage_Ice));
	CopyMemory(&enchant_weapon.Enchant_Weapon_Damage_Fire, Enchant_Weapon_Damage_Fire, sizeof(enchant_weapon.Enchant_Weapon_Damage_Fire));
	CopyMemory(&enchant_weapon.Enchant_Weapon_Damage_Lightning, Enchant_Weapon_Damage_Lightning, sizeof(enchant_weapon.Enchant_Weapon_Damage_Lightning));
	CopyMemory(&enchant_weapon.Enchant_Weapon_Time, Enchant_Weapon_Time, sizeof(enchant_weapon.Enchant_Weapon_Time));
	CopyMemory(&enchant_weapon.Enchant_Weapon_Mana, Enchant_Weapon_Mana, sizeof(enchant_weapon.Enchant_Weapon_Mana));

	this->send(player, &enchant_weapon);

	Dead_Ray dead_ray;

	dead_ray.classId = classId;
	dead_ray.skillId = SKILL_DEAD_RAY;
	dead_ray.size = sizeof(Dead_Ray);
	CopyMemory(&dead_ray.Dead_Ray_Damage, Dead_Ray_Damage, sizeof(dead_ray.Dead_Ray_Damage));
	CopyMemory(&dead_ray.Dead_Ray_Range, Dead_Ray_Range, sizeof(dead_ray.Dead_Ray_Range));
	CopyMemory(&dead_ray.Dead_Ray_Mana, Dead_Ray_Mana, sizeof(dead_ray.Dead_Ray_Mana));

	this->send(player, &dead_ray);

	Energy_Shield energy_shield;

	energy_shield.classId = classId;
	energy_shield.skillId = SKILL_ENERGY_SHIELD;
	energy_shield.size = sizeof(Energy_Shield);
	CopyMemory(&energy_shield.Energy_Shield_DecDamage, Energy_Shield_DecDamage, sizeof(energy_shield.Energy_Shield_DecDamage));
	CopyMemory(&energy_shield.Energy_Shield_Time, Energy_Shield_Time, sizeof(energy_shield.Energy_Shield_Time));
	CopyMemory(&energy_shield.Energy_Shield_UseMana, Energy_Shield_UseMana, sizeof(energy_shield.Energy_Shield_UseMana));

	this->send(player, &energy_shield);

	Diastrophism diastrophism;

	diastrophism.classId = classId;
	diastrophism.skillId = SKILL_DIASTROPHISM;
	diastrophism.size = sizeof(Diastrophism);
	CopyMemory(&diastrophism.Diastrophism_Damage, Diastrophism_Damage, sizeof(diastrophism.Diastrophism_Damage));
	CopyMemory(&diastrophism.Diastrophism_Area, Diastrophism_Area, sizeof(diastrophism.Diastrophism_Area));
	CopyMemory(&diastrophism.Diastrophism_UseMana, Diastrophism_UseMana, sizeof(diastrophism.Diastrophism_UseMana));

	this->send(player, &diastrophism);

	Spirit_Elemental spirit_elemental;

	spirit_elemental.classId = classId;
	spirit_elemental.skillId = SKILL_SPIRIT_ELEMENTAL;
	spirit_elemental.size = sizeof(Spirit_Elemental);
	CopyMemory(&spirit_elemental.Spirit_Elemental_RegenMana, Spirit_Elemental_RegenMana, sizeof(spirit_elemental.Spirit_Elemental_RegenMana));
	CopyMemory(&spirit_elemental.Spirit_Elemental_Damage, Spirit_Elemental_Damage, sizeof(spirit_elemental.Spirit_Elemental_Damage));
	CopyMemory(&spirit_elemental.Spirit_Elemental_Time, Spirit_Elemental_Time, sizeof(spirit_elemental.Spirit_Elemental_Time));
	CopyMemory(&spirit_elemental.Spirit_Elemental_UseMana, Spirit_Elemental_UseMana, sizeof(spirit_elemental.Spirit_Elemental_UseMana));

	this->send(player, &spirit_elemental);

	Dancing_Sword dancing_sword;

	dancing_sword.classId = classId;
	dancing_sword.skillId = SKILL_DANCING_SWORD;
	dancing_sword.size = sizeof(Dancing_Sword);
	CopyMemory(&dancing_sword.Dancing_Sword_IceDamage, Dancing_Sword_IceDamage, sizeof(dancing_sword.Dancing_Sword_IceDamage));
	CopyMemory(&dancing_sword.Dancing_Sword_FireDamage, Dancing_Sword_FireDamage, sizeof(dancing_sword.Dancing_Sword_FireDamage));
	CopyMemory(&dancing_sword.Dancing_Sword_AttackDelay, Dancing_Sword_AttackDelay, sizeof(dancing_sword.Dancing_Sword_AttackDelay));
	CopyMemory(&dancing_sword.Dancing_Sword_Time, Dancing_Sword_Time, sizeof(dancing_sword.Dancing_Sword_Time));
	CopyMemory(&dancing_sword.Dancing_Sword_UseMana, Dancing_Sword_UseMana, sizeof(dancing_sword.Dancing_Sword_UseMana));

	this->send(player, &dancing_sword);

	Fire_Elemental fire_elemental;

	fire_elemental.classId = classId;
	fire_elemental.skillId = SKILL_FIRE_ELEMENTAL;
	fire_elemental.size = sizeof(Fire_Elemental);
	CopyMemory(&fire_elemental.Fire_Elemental_Damage, Fire_Elemental_Damage, sizeof(fire_elemental.Fire_Elemental_Damage));
	CopyMemory(&fire_elemental.Fire_Elemental_Hit, Fire_Elemental_Hit, sizeof(fire_elemental.Fire_Elemental_Hit));
	CopyMemory(&fire_elemental.Fire_Elemental_Life, Fire_Elemental_Life, sizeof(fire_elemental.Fire_Elemental_Life));
	CopyMemory(&fire_elemental.Fire_Elemental_UseMana, Fire_Elemental_UseMana, sizeof(fire_elemental.Fire_Elemental_UseMana));

	this->send(player, &fire_elemental);

	Flame_Wave flame_wave;

	flame_wave.classId = classId;
	flame_wave.skillId = SKILL_FLAME_WAVE;
	flame_wave.size = sizeof(Flame_Wave);
	CopyMemory(&flame_wave.Flame_Wave_FireDamage, Flame_Wave_FireDamage, sizeof(flame_wave.Flame_Wave_FireDamage));
	CopyMemory(&flame_wave.Flame_Wave_Area, Flame_Wave_Area, sizeof(flame_wave.Flame_Wave_Area));
	CopyMemory(&flame_wave.Flame_Wave_UseMana, Flame_Wave_UseMana, sizeof(flame_wave.Flame_Wave_UseMana));

	this->send(player, &flame_wave);

	Distortion distortion;

	distortion.classId = classId;
	distortion.skillId = SKILL_DISTORTION;
	distortion.size = sizeof(Distortion);
	CopyMemory(&distortion.Distortion_SpeedSubPercent, Distortion_SpeedSubPercent, sizeof(distortion.Distortion_SpeedSubPercent));
	CopyMemory(&distortion.Distortion_DamageSubPercent, Distortion_DamageSubPercent, sizeof(distortion.Distortion_DamageSubPercent));
	CopyMemory(&distortion.Distortion_Area, Distortion_Area, sizeof(distortion.Distortion_Area));
	CopyMemory(&distortion.Distortion_Time, Distortion_Time, sizeof(distortion.Distortion_Time));
	CopyMemory(&distortion.Distortion_UseMana, Distortion_UseMana, sizeof(distortion.Distortion_UseMana));

	this->send(player, &distortion);

	Meteo meteo;

	meteo.classId = classId;
	meteo.skillId = SKILL_M_METEO;
	meteo.size = sizeof(Meteo);
	CopyMemory(&meteo.Meteo_Damage, Meteo_Damage, sizeof(meteo.Meteo_Damage));
	CopyMemory(&meteo.Meteo_Area, Meteo_Area, sizeof(meteo.Meteo_Area));
	CopyMemory(&meteo.Meteo_UseMana, Meteo_UseMana, sizeof(meteo.Meteo_UseMana));

	this->send(player, &meteo);
}

// Mensagem enviada ao recarregar skills
void Skills::sendAll() const
{
	this->sendPlayer(nullptr);
}

// Mensagem enviada ao logar
void Skills::sendPlayer(rsPLAYINFO* player) const
{
	if (player)
	{
		sendMechanician(player);
		sendFighter(player);
		sendPikeman(player);
		sendArcher(player);
		sendKnight(player);
		sendAtalanta(player);
		sendPriestess(player);
		sendMagician(player);
	}
	else
	{
		sendMechanician(nullptr);
		sendFighter(nullptr);
		sendPikeman(nullptr);
		sendArcher(nullptr);
		sendKnight(nullptr);
		sendAtalanta(nullptr);
		sendPriestess(nullptr);
		sendMagician(nullptr);
	}

}

void openSkills()
{
	cout << "Lendo arquivos de magias" << endl;
	skill.readAll();
}