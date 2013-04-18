#ifndef SOCKET_HPP
#define SOCKET_HPP

#ifdef _WIN32 //_WIN64

    #include <winsock2.h>

#elif __linux //|| __unix //or __APPLE__ 

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h> /* close */
    #include <netdb.h> /* gethostbyname */
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close(s)
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef struct in_addr IN_ADDR;

#else

    #error not defined for this platform

#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>

#define CRLF		"\r\n"
#define BUF_SIZE	1024
#define PORT 1977

class Socket
{
    public:
        enum Dommaine {IP=AF_INET, LOCAL=AF_UNIX};
        enum Type {TCP=SOCK_STREAM, UDP=SOCK_DGRAM};
        enum Down {EMIT=0,RECIVE=1,BOTH=2};

        Socket(Dommaine dommaine,Type type,int protocole=0);
        ~Socket();

        void Connect(std::string host,int port=PORT);
        Socket Wait(std::string host="",int port=PORT);

        template<typename T>
        inline void Send(const T* data,const size_t len,const int flags=0) const
        {
            if(send(sock,data,sizeof(T)*len,flags) ==  SOCKET_ERROR)
            {
                perror("Sending message fail");
                throw "Sending message fail";
            }
        }

        template<typename T>
        inline void Receive(T* buffer,const size_t len,const int flags=0) const
        {
            recv(sock,buffer,sizeof(T)*len,flags);
        };

        void Shutdown(Down mode=Down::BOTH);


        static int Max_clients;
        static int Buffer_size;

    private:
        Socket(){}// intern use only;
        inline void _close(){closesocket(sock);};
        //socket
        SOCKET sock;
        //configuration
        SOCKADDR_IN sock_cfg;

};

#endif
