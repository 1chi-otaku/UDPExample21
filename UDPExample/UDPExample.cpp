#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#include <queue>
using namespace std;


DWORD WINAPI QueuePush(LPVOID lp) {

    cout << "Receiving data...." << endl;
    int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);
}

int main()
{
    WSADATA wsadata;
    queue <sockaddr> addressqueue;

    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(23000);   
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }

    while (true) {
        
        int timer = 0;
        const size_t receiveBufSize = 1024;
        char receiveBuf[receiveBufSize];

        sockaddr_in senderAddr;
        int senderAddrSize = sizeof(senderAddr);

        cout << "Receiving data...." << endl;
        int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "recvfrom failed with error " << WSAGetLastError() << endl;
            return 4;
        }

        receiveBuf[bytesReceived] = '\0';
        if (strstr(receiveBuf, "cola")) timer += 1;
        if (strstr(receiveBuf, "hamburger")) timer += 10;
        if (strstr(receiveBuf, "ice cream")) timer += 7;

   
        cout << "Data: " << receiveBuf << endl;
        cout << timer << " seconds to wait..";
        string waitMessage = "Thank you for order. Your order will be ready soon..";

        int sendResult = sendto(udpSocket, waitMessage.c_str(), waitMessage.length(), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
        Sleep(timer * 1000);


        const size_t sendBufSize = 1024;
        char sendBuf[sendBufSize] = "Hello from UDPExample";


        sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
        if (sendResult == SOCKET_ERROR)
        {
            cout << "sendto failed with error " << WSAGetLastError() << endl;
            return 5;
        }
    }
    


    closesocket(udpSocket);
    WSACleanup();
}

