#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#define SOCKETS true
#define WINDOWS_SOCKETS true

#define DEFAULT_BUFLEN 512

// Common Ports
#define DEFAULT_PORT 80
#define HTTP_PORT 80
#define HTTPS_PORT 443
#define POP3_PORT 110
#define FTP_PORT 21
#define SMTP_PORT 25


class tasks_winsocket {
private:
    static const char *intToArray(int x) {
        std::string s = std::to_string(x);
        const char *portNumber = s.c_str();
        return portNumber;
    }
public:

    /// Allows you to connect to an ip or url,
    /// Will return true if the connection is up
    /// False if its not
    static bool w_test_connect(std::string IPAddress, int port = 80) {

        const char *portNumber = intToArray(port);
        const char *Server_IP_Address = IPAddress.c_str();


        int iResult;
        WSADATA wsaData;
        struct addrinfo *result = NULL, *ptr = NULL, hints;

        ZeroMemory( &hints, sizeof(hints) );
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed: %d\n", iResult);
            return false;
        }

        // Resolve the server address and port
        iResult = getaddrinfo(Server_IP_Address, portNumber, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed: %d\n", iResult);
            WSACleanup();
            return false;
        }

        SOCKET ConnectSocket = INVALID_SOCKET;//. // found this random dot, don't think it should be here
        ptr=result;
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
        }

        freeaddrinfo(result);

        // Socket Connection error
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return false;
        }

        // Closing Socket
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;

        return true;
    }

    /// Basic Connect and send packet operation with winsock
    /// if operation faild the the function will return false
    /// success will return true
    static bool w_connect_and_send(std::string IPAddress, int port = 80, std::string msg = "DEFAULT_SEND_MESSAGE") {

        const char *portNumber = intToArray(port);
        const char *Server_IP_Address = IPAddress.c_str();
        const char *sendbuf = msg.c_str();


        int iResult;
        WSADATA wsaData;
        struct addrinfo *result = NULL, *ptr = NULL, hints;

        ZeroMemory( &hints, sizeof(hints) );
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed: %d\n", iResult);
            return false;
        }

        // Resolve the server address and port
        iResult = getaddrinfo(Server_IP_Address, portNumber, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed: %d\n", iResult);
            WSACleanup();
            return false;
        }

        SOCKET ConnectSocket = INVALID_SOCKET;
        ptr=result;
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
        }
        freeaddrinfo(result);

        // Socket Connection error
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return false;
        }

        int recvbuflen = DEFAULT_BUFLEN;

        char recvbuf[DEFAULT_BUFLEN];

        // Send an initial buffer
        iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }

        printf("Bytes Sent: %ld\n", iResult);

        // shutdown the connection for sending since no more data will be sent
        // the client can still use the ConnectSocket for receiving data
        iResult = shutdown(ConnectSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return 1;
        }

        // Receive data until the server closes the connection
        do {
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
                printf("Bytes received: %d\n", iResult);
            else if (iResult == 0)
                printf("Connection closed\n"); // only means we got all the data back
            else
                printf("recv failed: %d\n", WSAGetLastError());
        } while (iResult > 0);

        // Closing Socket
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;

        return true;
    }

};
// Will be more interactive with your sockets
class win_sock
{
private:
    static const char *intToArray(int x) {
        std::string s = std::to_string(x);
        const char *portNumber = s.c_str();
        return portNumber;
    }
public:
    static SOCKET w_connect(char *Server_IP_Address, int port = 80) {

        const char *portNumber = intToArray(port);

        int iResult;
        WSADATA wsaData;
        struct addrinfo *result = NULL, *ptr = NULL, hints;

        ZeroMemory( &hints, sizeof(hints) );
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed: %d\n", iResult);
            return INVALID_SOCKET;
        }

        // Resolve the server address and port
        iResult = getaddrinfo(Server_IP_Address, portNumber, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed: %d\n", iResult);
            WSACleanup();
            return INVALID_SOCKET;
        }

        SOCKET ConnectSocket = INVALID_SOCKET;
        ptr=result;
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return INVALID_SOCKET;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
        }

        freeaddrinfo(result);

        // Socket Connection error
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return INVALID_SOCKET;
        }

        return ConnectSocket;
    }
    static SOCKET w_connect(std::string ipadd, int port = 80) {

        const char *portNumber  = intToArray(port);
        const char* Server_IP_Address = ipadd.c_str();

        int iResult;
        WSADATA wsaData;
        struct addrinfo *result = NULL, *ptr = NULL, hints;

        ZeroMemory( &hints, sizeof(hints) );
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed: %d\n", iResult);
            return INVALID_SOCKET;
        }

        // Resolve the server address and port
        iResult = getaddrinfo(Server_IP_Address, portNumber, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed: %d\n", iResult);
            WSACleanup();
            return INVALID_SOCKET;
        }

        SOCKET ConnectSocket = INVALID_SOCKET;
        ptr=result;
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return INVALID_SOCKET;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
        }

        freeaddrinfo(result);

        // Socket Connection error
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return INVALID_SOCKET;
        }

        return ConnectSocket;
    }
    static bool w_send(SOCKET ConnectSocket, char* sendbuf = "DEFAULT_SEND_MESSAGE") {

        if (ConnectSocket == INVALID_SOCKET) {
            cout << "[ERROR][SOCKET IS INVALID]" << endl;
            return false;
        }

        int iResult;
        WSADATA wsaData;
        struct addrinfo *result = NULL, *ptr = NULL, hints;
        int recvbuflen = DEFAULT_BUFLEN;
        char recvbuf[DEFAULT_BUFLEN];

        // Send an initial buffer
        iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return false;
        }

        printf("Bytes Sent: %ld\n", iResult);

        // shutdown the connection for sending since no more data will be sent
        // the client can still use the ConnectSocket for receiving data
        iResult = shutdown(ConnectSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return false;
        }

        // Receive data until the server closes the connection
        do {
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
                printf("Bytes received: %d\n", iResult);
            else if (iResult == 0)
                cout << ""; // Do nothing! //printf("Connection closed\n"); // only means we got all the data back
            else
                printf("recv failed: %d\n", WSAGetLastError());
        } while (iResult > 0);

        return true;
    }
    static bool w_send(SOCKET ConnectSocket, std::string sendString = "DEFAULT_SEND_MESSAGE") {

        const char* sendbuf = sendString.c_str();

        if (ConnectSocket == INVALID_SOCKET) {
            cout << "[ERROR][SOCKET IS INVALID]" << endl;
            return false;
        }

        int iResult;
        WSADATA wsaData;
        struct addrinfo *result = NULL, *ptr = NULL, hints;
        int recvbuflen = DEFAULT_BUFLEN;
        char recvbuf[DEFAULT_BUFLEN];

        // Send an initial buffer
        iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return false;
        }

        printf("Bytes Sent: %ld\n", iResult);

        // shutdown the connection for sending since no more data will be sent
        // the client can still use the ConnectSocket for receiving data
        iResult = shutdown(ConnectSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return false;
        }

        // Receive data until the server closes the connection
        do {
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
                printf("Bytes received: %d\n", iResult);
            else if (iResult == 0)
                cout << ""; // Do nothing! //printf("Connection closed\n"); // only means we got all the data back
            else
                printf("recv failed: %d\n", WSAGetLastError());
        } while (iResult > 0);

        return true;
    }
};

///
/// An accual socket class
///
class a_sock
{
private:
    const char *intToArray(int x) {
        std::string s = std::to_string(x);
        const char *portNumber = s.c_str();
        return portNumber;
    }

    SOCKET userSocket = INVALID_SOCKET;
public:
    /// Connect.
    a_sock(std::string ip_addr, int port = 80) {

        const char *portNumber = intToArray(port);
        const char* Server_IP_Address = ip_addr.c_str();

        int iResult;
        WSADATA wsaData;
        struct addrinfo *result = NULL, *ptr = NULL, hints;

        ZeroMemory( &hints, sizeof(hints) );
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed: %d\n", iResult);
            return;
        }

        // Resolve the server address and port
        iResult = getaddrinfo(Server_IP_Address, portNumber, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed: %d\n", iResult);
            WSACleanup();
            return;
        }

        SOCKET ConnectSocket = INVALID_SOCKET;
        ptr=result;
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
        }

        freeaddrinfo(result);

        // Socket Connection error
        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
            return;
        }

        userSocket = ConnectSocket;
    }
    bool Send(std::string sendString = "DEFAULT_SEND_MESSAGE") {

        const char* sendbuf = sendString.c_str();
        SOCKET ConnectSocket = userSocket;

        if (ConnectSocket == INVALID_SOCKET) {
            cout << "[ERROR][SOCKET IS INVALID]" << endl;
            return false;
        }

        int iResult;
        WSADATA wsaData;
        struct addrinfo *result = NULL, *ptr = NULL, hints;
        int recvbuflen = DEFAULT_BUFLEN;
        char recvbuf[DEFAULT_BUFLEN];

        // Send an initial buffer
        iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return false;
        }

        printf("Bytes Sent: %ld\n", iResult);

        // shutdown the connection for sending since no more data will be sent
        // the client can still use the ConnectSocket for receiving data
        iResult = shutdown(ConnectSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            return false;
        }

        // Receive data until the server closes the connection
        do {
            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
                printf("Bytes received: %d\n", iResult);
            else if (iResult == 0)
                cout << ""; // Do nothing! //printf("Connection closed\n"); // only means we got all the data back
            else
                printf("recv failed: %d\n", WSAGetLastError());
        } while (iResult > 0);

        return true;
    }
    virtual ~a_sock() {};
};
