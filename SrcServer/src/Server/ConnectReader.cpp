#include "ConnectReader.h"
#include "smwsock.h"
#include <windows.h>
#include <iostream>

// Definição da variável global TCP_SERVPORT
int TCP_SERVPORT = 32299; // Valor padrão

// Função para inicializar o TCP_SERVPORT com o valor do Connect.ini
void InicializarTCPPort()
{
    char szPort[255] = { 0 };
    char Pasta[600] = { 0 };
    char PastaEmu[512] = { 0 };
    GetCurrentDirectoryA(512, PastaEmu);
    wsprintfA(Pasta, "%s\\%s", (char*)PastaEmu, "Server\\Config\\Connect.ini");

    // Log do caminho completo
  //  std::cout << "Tentando ler o arquivo INI em: " << Pasta << std::endl;

    // VERIFICAÇÃO: Testa se consegue ler a seção [Server] que sabemos que funciona
    char szTest[255] = { 0 };
    LeIniStr("Server", "Maintenance", "Server\\Config\\Server.ini", szTest);
  //  std::cout << "Teste de leitura do Server.ini - Maintenance: [" << szTest << "]" << std::endl;

    // VERIFICAÇÃO: Testa se consegue ler a seção [ConnectServer] do Connect.ini
    LeIniStr("ConnectServer", "IP", "Server\\Config\\Connect.ini", szTest);
   // std::cout << "Teste de leitura do Connect.ini - IP: [" << szTest << "]" << std::endl;

    // Lê a porta do arquivo Connect.ini
    LeIniStr("ConnectServer", "Port", "Server\\Config\\Connect.ini", szPort);
   // std::cout << "Valor lido da porta: [" << szPort << "]" << std::endl;

    // Se conseguiu ler a porta, converte para inteiro
    if (strlen(szPort) > 0) {
        TCP_SERVPORT = atoi(szPort);
        std::cout << "TCP_SERVPORT inicializado com: " << TCP_SERVPORT << std::endl;
    }
    else {
      //  std::cout << "Não foi possível ler a porta do Connect.ini. Usando valor padrão: " << TCP_SERVPORT << std::endl;
    }
}

// Função para ler configurações do Connect.ini
ConnectConfig LerConfiguracaoConnect()
{
    ConnectConfig config;
    char szIP[255] = { 0 };
    char szPort[255] = { 0 };
    
    // Lê o IP do arquivo Connect.ini
    LeIniStr("ConnectServer", "IP", "Server\\Config\\Connect.ini", szIP);
    
    // Lê a Porta do arquivo Connect.ini
    LeIniStr("ConnectServer", "Port", "Server\\Config\\Connect.ini", szPort);
    
    // Armazena os valores na estrutura
    config.strIP = szIP;
    config.nPort = atoi(szPort);
    
    return config;
}

// Função para ler apenas o IP
std::string LerIPConnect()
{
    char szIP[255] = { 0 };
    LeIniStr("ConnectServer", "IP", "Server\\Config\\Connect.ini", szIP);
    return std::string(szIP);
}

// Função para ler apenas a Porta
int LerPortaConnect()
{
    char szPort[255] = { 0 };
    LeIniStr("ConnectServer", "Port", "Server\\Config\\Connect.ini", szPort);
    return atoi(szPort);
}

// Função para ler a configuração do Clan
std::string LerClanConnect()
{
    char szClan[255] = { 0 };
    LeIniStr("ConnectServer", "Clan", "Server\\Config\\Connect.ini", szClan);
    return std::string(szClan);
} 