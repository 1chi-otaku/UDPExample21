#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <string>
using namespace std;

int main()
{
    WSADATA wsadata;

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

    sockaddr_in addrTo;
    addrTo.sin_family = AF_INET;
    addrTo.sin_port = htons(23000);
    inet_pton(AF_INET, "127.0.0.1", &addrTo.sin_addr.s_addr);
    cout << "-----Menu-----" << endl;
    cout << "1\tCola\t\t5sec\n2\tHamburger\t20sec\n3\tIce Cream\t8sec\n";
    string order;
    cout << "Enter your order:";
    getline(cin, order);

    cout << "Sending data..." << endl;
    int sendResult = sendto(udpSocket, order.c_str(), order.length(), 0, (SOCKADDR*)&addrTo, sizeof(addrTo));
    if (sendResult == SOCKET_ERROR)
    {
        cout << "sendto failed with error " << WSAGetLastError() << endl;
        return 4;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    sockaddr_in addrFrom;
    int addrFromSize = sizeof(addrFrom);

    cout << "Please wait. You're in the queue" << endl;
    int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&addrFrom, &addrFromSize);


    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "recvfrom failed with error " << WSAGetLastError() << endl;
        return 4;
    }

    receiveBuf[bytesReceived] = '\0';
    cout << "Data: " << receiveBuf << endl;

    cout << "Waiting...." << endl;
    bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&addrFrom, &addrFromSize);
    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "recvfrom failed with error " << WSAGetLastError() << endl;
        return 4;
    }
    receiveBuf[bytesReceived] = '\0';
    cout << "Data: " << receiveBuf << endl;

    closesocket(udpSocket);
    WSACleanup();
}

