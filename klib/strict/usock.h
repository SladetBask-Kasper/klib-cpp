#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SOCKETS true
#define UNIX_SOCKETS true

//#define BUFF_SIZE 256 // <--- Not used currently

// Common Ports
#define DEFAULT_PORT 80
#define HTTP_PORT 80
#define HTTPS_PORT 443
#define POP3_PORT 110
#define FTP_PORT 21
#define SMTP_PORT 25

class usock
{
private:
    static void error(const char *msg)
    {
        perror(msg);
        printf("ERROR: %s\n", msg);
    }
public:



    // -------------- STRING VERSIONS -------------- //

    /// String edition
    static bool connect_send(std::string ip_a, int portno = 80, std::string msg_tosend = "DEFAULT_MESSAGE")
    {

        int sockfd, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;

        // reads message in to buffer
        char *buffer = (char*)msg_tosend.c_str();
        const char *ipaddr = ip_a.c_str();

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            error("ERROR opening socket");
            return false;
        }

        // Resolves hostname (So it can deal with url and ip)
        server = gethostbyname(ipaddr);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            return false;
        }

        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;

        bcopy((char *)server->h_addr,
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);
        serv_addr.sin_port = htons(portno);

        // Connecting to server
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
            error("ERROR connecting");
            return false;
        }


        // Write To Socket
        /*n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) {
            error("ERROR writing to socket");
            return false;
        }*/

        // Sends to target instead of writeing
        int sendLen = strlen(buffer);
        send(sockfd, &sendLen, sizeof(sendLen), 0);
        send(sockfd, buffer, sendLen, 0);


        // Will read data aswell
        /*n = read(sockfd,buffer,255);
        if (n < 0)
             error("ERROR reading from socket"); return false;
        printf("%s\n",buffer);*/

        close(sockfd);
        return true;
    }
    /// Connects A socket and returns it to you.
    /// Yes. Sockets in linux is type int!
    /// String edition
    static int connect_socket(std::string ip_a, int portno = 80)
    {

        int sockfd, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;

        const char *ipaddr = ip_a.c_str();

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            error("ERROR opening socket");
            return 0;
        }

        // Resolves hostname (So it can deal with url and ip)
        server = gethostbyname(ipaddr);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            return 0;
        }

        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;

        bcopy((char *)server->h_addr,
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);
        serv_addr.sin_port = htons(portno);

        // Connecting to server
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
            error("ERROR connecting");
            return 0;
        }

        return sockfd;
    }
    /// Usend // Uninx Send
    /// Sends packet with socket
    /// String edition
    static void usend(int sockfd, std::string msg_tosend = "DEFAULT_MESSAGE")
    {



        // reads message in to buffer
        char *buffer = (char*)msg_tosend.c_str();

        // Sends to target instead of writeing
        int sendLen = strlen(buffer);
        send(sockfd, &sendLen, sizeof(sendLen), 0);
        send(sockfd, buffer, sendLen, 0);
    }
};
/*
/// Connects socket and sends packet and then closees socket
static bool connect_send(char const *ipaddr, int portno = 80, char *msg_tosend = (char*)"DEFAULT_MESSAGE")
{

    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // reads message in to buffer
    char *buffer = (char*)msg_tosend;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
        return false;
    }

    // Resolves hostname (So it can deal with url and ip)
    server = gethostbyname(ipaddr);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return false;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connecting to server
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
        return false;
    }


    // Write To Socket
    /*n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) {
        error("ERROR writing to socket");
        return false;
    }*//*

    // Sends to target instead of writeing
    int sendLen = strlen(buffer);
    send(sockfd, &sendLen, sizeof(sendLen), 0);
    send(sockfd, buffer, sendLen, 0);


    // Will read data aswell
    /*n = read(sockfd,buffer,255);
    if (n < 0)
         error("ERROR reading from socket"); return false;
    printf("%s\n",buffer);*//*

    close(sockfd);
    return true;
}
/// Connects A socket and returns it to you.
/// Yes. Sockets in linux is type int!
static int connect_socket(char const *ipaddr, int portno = 80)
{

    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
        return 0;
    }

    // Resolves hostname (So it can deal with url and ip)
    server = gethostbyname(ipaddr);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return 0;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connecting to server
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
        return 0;
    }

    return sockfd;
}
/// Usend // Uninx Send
/// Sends packet with socket
static void usend(int sockfd, char *msg_tosend = (char*)"DEFAULT_MESSAGE")
{

    // reads message in to buffer
    char *buffer = (char*)msg_tosend;

    // Sends to target instead of writeing
    int sendLen = strlen(buffer);
    send(sockfd, &sendLen, sizeof(sendLen), 0);
    send(sockfd, buffer, sendLen, 0);
}
*/
