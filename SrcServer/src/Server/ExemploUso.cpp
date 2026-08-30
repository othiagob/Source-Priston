#include "ConnectReader.h"
#include "smwsock.h"
#include <iostream>

int main()
{
    // Inicializa o TCP_SERVPORT com o valor do Connect.ini
    InicializarTCPPort();
    
    // Agora o TCP_SERVPORT contém o valor lido do arquivo Connect.ini (31620)
    std::cout << "TCP_SERVPORT: " << TCP_SERVPORT << std::endl;
    
    // Você pode usar o TCP_SERVPORT normalmente em qualquer lugar do código
    // Por exemplo:
    // InitBindSock(TCP_SERVPORT);
    // smConnectSock3(szIP, TCP_SERVPORT);
    
    return 0;
} 