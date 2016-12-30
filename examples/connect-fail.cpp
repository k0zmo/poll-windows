#include <stdio.h>
#include <WinSock2.h>

#include "poll.h"

//
// Taken from: https://curl.haxx.se/mail/lib-2012-07/0311.html
//

void TestSocketConnection(const char* aIPAddress, USHORT aPortNumber,
                          INT aTimeout)
{
    SOCKET lSOCKET = socket(AF_INET,     // 2,	// family
                            SOCK_STREAM, // 1,	// type
                            IPPROTO_IP   // 0	// protocol
                            );

    printf("\n"
           "\n"
           "lSOCKET is 0x%08X.\n",
           lSOCKET);

    unsigned long lFlags = 1;

    int lReturnValue = ioctlsocket(lSOCKET, FIONBIO, &lFlags);

    printf("ioctlsocket(FIONBIO, 1) returned %d.\n", lReturnValue);

    printf("connecting to %s, port %u.\n", aIPAddress, aPortNumber);

    sockaddr_in lsockaddr_in;

    lsockaddr_in.sin_family = AF_INET;
    lsockaddr_in.sin_addr.s_addr = inet_addr(aIPAddress);
    lsockaddr_in.sin_port = htons(aPortNumber);

    lReturnValue =
        connect(lSOCKET, (sockaddr*)&lsockaddr_in, sizeof(lsockaddr_in));

    printf("connect() returned %d.\n", lReturnValue);

    int lWSAError = 0;

    if (lReturnValue == SOCKET_ERROR)
    {
        lWSAError = WSAGetLastError();

        printf("WSAGetLastError() returned %d.\n", lWSAError);
    }

    if (lWSAError == WSAEWOULDBLOCK)
    {
        int lError = 0;
        int lErrorSize = sizeof(lError);
        int lPollCount = 0;

        while (lPollCount == 0 && lError == 0)
        {
            WSAPOLLFD lWSAPOLLFD;

            lWSAPOLLFD.fd = lSOCKET;
            lWSAPOLLFD.events = POLLWRNORM;
            lWSAPOLLFD.revents = 0;

            lPollCount = poll(&lWSAPOLLFD, 1, aTimeout);

            printf("\n"
                   "WSAPoll(timeout=%dms) returned %d.\n"
                   "lWSAPOLLFD.revents is 0x%02X\n",
                   aTimeout, lPollCount, lWSAPOLLFD.revents);

            lReturnValue = getsockopt(lSOCKET, SOL_SOCKET, SO_ERROR,
                                      (char*)&lError, &lErrorSize);

            printf("getsockopt() returned %d, lError is %d.\n", lReturnValue,
                   lError);
        }
    }

    lReturnValue = closesocket(lSOCKET);

    printf("\n"
           "closesocket() returned %d.\n",
           lReturnValue);
}

int main(int aArgumentCount, char* aArgumentValue[])
{
    printf("Hello, world!\n");

    WORD lRequestedVersion = MAKEWORD(2, 2);

    WSADATA lWSADATA;

    int lReturnValue = WSAStartup(lRequestedVersion, &lWSADATA);

    printf("WSAStartup returned %d.\n", lReturnValue);

    INT lTimeout = -1;

    if (2 <= aArgumentCount)
    {
        lTimeout = atoi(aArgumentValue[1]);
    }

    TestSocketConnection("213.180.141.140", 443, lTimeout);
    TestSocketConnection("213.180.141.140", 8443, lTimeout);

    lReturnValue = WSACleanup();

    printf("\n"
           "WSACleanup returned %d.\n",
           lReturnValue);

    printf("Bye, world!\n");

    return 0;
}
