#pragma once

#include <string>

// Declaração externa da função LeIniStr que já existe no projeto
extern void LeIniStr(char* Section, char* Key, char* szFileIni, char* Var1);

// Estrutura para armazenar as configurações do Connect Server
struct ConnectConfig
{
    std::string strIP;
    int nPort;
    
    ConnectConfig() : nPort(0) {}
};

// Função para inicializar o TCP_SERVPORT com o valor do Connect.ini
void InicializarTCPPort();
void InicializarDefaultServIP();

// Função para ler configurações do Connect.ini
ConnectConfig LerConfiguracaoConnect();

// Função para ler apenas o IP
std::string LerIPConnect();

// Função para ler apenas a Porta
int LerPortaConnect();

// Função para ler a configuração do Clan
std::string LerClanConnect(); 