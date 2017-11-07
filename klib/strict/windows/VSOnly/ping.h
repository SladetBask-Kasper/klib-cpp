#ifndef PING_H
#define PING_H
#include <Ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <string>

#define ERROR_VALUE -1
#define ZERO_ERROR -10
#define MEM_ERROR -100
#define ADDRESS_ERROR -1000
#define AV_ERROR -11
#define ERROR_DONE -11111
#define STD_MSG_HEADER "MS_VS"
#define MAX_HEADER_SIZE 255

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

class w_pingClass {
    static int __cdecl w_ping(string cip = "www.google.com") {

        // Declare and initialize variables
        HANDLE hIcmpFile;
        unsigned long ipaddr = INADDR_NONE;
        DWORD dwRetVal = 0;
        char SendData[MAX_HEADER_SIZE] = STD_MSG_HEADER;
        LPVOID ReplyBuffer = NULL;
        DWORD ReplySize = 0;

        // Removing Variables
        {
            const char * ipBOI = cip.c_str();
            ipaddr = inet_addr(ipBOI);
            if (ipaddr == INADDR_NONE) {
               return ADDRESS_ERROR;
           }
       }
       hIcmpFile = IcmpCreateFile();
       if (hIcmpFile == INVALID_HANDLE_VALUE) {
           cout << "Unable to open handle. IcmpCreatefile returned error: " << GetLastError() << endl;
           return ERROR_DONE;
       }

       ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
       ReplyBuffer = (VOID*)malloc(ReplySize);
       if (ReplyBuffer == NULL) {
           return MEM_ERROR;
       }


       dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
       if (dwRetVal != 0) {
           PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
           struct in_addr ReplyAddr;
           ReplyAddr.S_un.S_addr = pEchoReply->Address;
           return pEchoReply->RoundTripTime;
       } else {
           return ZERO_ERROR;
       }
       return ERROR_VALUE;
    }
    static int __cdecl w_ping(char *ipBOI = "www.google.com") {

        // Declare and initialize variables
        HANDLE hIcmpFile;
        unsigned long ipaddr = INADDR_NONE;
        DWORD dwRetVal = 0;
        char SendData[MAX_HEADER_SIZE] = STD_MSG_HEADER;
        LPVOID ReplyBuffer = NULL;
        DWORD ReplySize = 0;

        ipaddr = inet_addr(ipBOI);

        if (ipaddr == INADDR_NONE) {
               return ADDRESS_ERROR;
        }

        hIcmpFile = IcmpCreateFile();
        if (hIcmpFile == INVALID_HANDLE_VALUE) {
            cout << "Unable to open handle. IcmpCreatefile returned error: " << GetLastError() << endl;
            return ERROR_DONE;
        }

        ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
        ReplyBuffer = (VOID*)malloc(ReplySize);
        if (ReplyBuffer == NULL) {
            return MEM_ERROR;
        }


        dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
        if (dwRetVal != 0) {
            PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
            struct in_addr ReplyAddr;
            ReplyAddr.S_un.S_addr = pEchoReply->Address;
            return pEchoReply->RoundTripTime;
        } else {
            return ZERO_ERROR;
        }
        return ERROR_VALUE;
    }
};
class w_ping {
private:
    std::string cip;
public:
    w_ping(std::string ipaddress) { cip = ipaddress; }
    w_ping() { continue; }
    void set(std::string ipaddress) { cip = ipaddress; }
    std::string get() { return cip; }
    virtual ~w_ping() {}
    int __cdecl w_ping() {

        // Declare and initialize variables
        HANDLE hIcmpFile;
        unsigned long ipaddr = INADDR_NONE;
        DWORD dwRetVal = 0;
        char SendData[MAX_HEADER_SIZE] = STD_MSG_HEADER;
        LPVOID ReplyBuffer = NULL;
        DWORD ReplySize = 0;

        {
            const char * ipBOI = cip.c_str();
            ipaddr = inet_addr(ipBOI);
            if (ipaddr == INADDR_NONE) {
               return ADDRESS_ERROR;
           }
       }
       hIcmpFile = IcmpCreateFile();
       if (hIcmpFile == INVALID_HANDLE_VALUE) {
           cout << "Unable to open handle. IcmpCreatefile returned error: " << GetLastError() << endl;
           return ERROR_DONE;
       }

       ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
       ReplyBuffer = (VOID*)malloc(ReplySize);
       if (ReplyBuffer == NULL) {
           return MEM_ERROR;
       }


       dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
       if (dwRetVal != 0) {
           PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
           struct in_addr ReplyAddr;
           ReplyAddr.S_un.S_addr = pEchoReply->Address;
           return pEchoReply->RoundTripTime;
       } else {
           return ZERO_ERROR;
       }
       return ERROR_VALUE;
    }
};
#endif // PING_H
