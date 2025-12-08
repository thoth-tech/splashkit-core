//
//  skNetwork.cpp
//  sk
//
//  Created by Andrew Cain on 16/11/2014.
//  Copyright (c) 2014 Andrew Cain. All rights reserved.
//

#include "core_driver.h"
#include "network_driver.h"
#include "easylogging++.h"

#include <stdio.h>

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#else
#include <SDL.h>
#include <SDL_net.h>
#endif

#include <string.h>
#include <stdlib.h>

// Platform-specific includes for non-blocking socket operations
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define SOCKET_ERRNO WSAGetLastError()
    #define SOCKET_WOULDBLOCK WSAEWOULDBLOCK
    #define SOCKET_INPROGRESS WSAEINPROGRESS
#else
    #include <fcntl.h>
    #include <sys/socket.h>
    #include <sys/select.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>
    #define SOCKET_ERRNO errno
    #define SOCKET_WOULDBLOCK EWOULDBLOCK
    #define SOCKET_INPROGRESS EINPROGRESS
#endif

// Default connection timeout in seconds
#define SK_CONNECTION_TIMEOUT_SECONDS 5

namespace splashkit_lib
{
    // This set keeps track of all of the sockets to see if there is activity
    SDLNet_SocketSet _sockets; // allocate on setup of functions.

    void sk_network_init()
    {
        SDLNet_Init();
        _sockets = SDLNet_AllocSocketSet(1024);
        if(!_sockets)
        {
            printf("Error allocating network resources\n");
            exit(1);
        }
    }

    sk_network_connection sk_open_udp_connection(unsigned short port)
    {
        internal_sk_init();

        UDPsocket svr;

        sk_network_connection result;
        result.id = NETWORK_CONNECTION_PTR;
        result.kind = UNKNOWN;
        result._socket = nullptr;

        svr = SDLNet_UDP_Open(port);

        if (svr)
        {
            result.kind = UDP;
            result._socket = svr;
            SDLNet_UDP_AddSocket(_sockets, svr);
        }
        else
        {
            result.kind = UNKNOWN;
            result._socket = nullptr;
            LOG(ERROR) << "Failed to open UDP server on port " << port << ": " << SDLNet_GetError();
        }

        return result;
    }

    /**
     * Helper function to connect with timeout using native sockets.
     * Returns the connected socket fd on success, -1 on failure.
     */
    static int _connect_with_timeout(const char *host, unsigned short port, int timeout_seconds)
    {
        struct sockaddr_in server_addr;
        int sock_fd;
        
        // Create socket
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd < 0)
        {
            LOG(ERROR) << "Failed to create socket: " << SOCKET_ERRNO;
            return -1;
        }
        
        // Set up server address
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        
        // Convert host to IP address
        if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0)
        {
            // Try to resolve hostname using SDLNet
            IPaddress addr;
            if (SDLNet_ResolveHost(&addr, host, port) < 0)
            {
                LOG(ERROR) << "Failed to resolve host: " << host;
#ifdef _WIN32
                closesocket(sock_fd);
#else
                close(sock_fd);
#endif
                return -1;
            }
            server_addr.sin_addr.s_addr = addr.host;
        }
        
        // Set socket to non-blocking mode
#ifdef _WIN32
        unsigned long mode = 1;
        ioctlsocket(sock_fd, FIONBIO, &mode);
#else
        int flags = fcntl(sock_fd, F_GETFL, 0);
        fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
#endif
        
        // Initiate connection (returns immediately for non-blocking socket)
        int connect_result = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        
        if (connect_result < 0)
        {
            int err = SOCKET_ERRNO;
            // Check if connection is in progress (expected for non-blocking)
            if (err != SOCKET_INPROGRESS && err != SOCKET_WOULDBLOCK)
            {
                LOG(ERROR) << "Connect failed immediately: " << err;
#ifdef _WIN32
                closesocket(sock_fd);
#else
                close(sock_fd);
#endif
                return -1;
            }
            
            // Wait for connection with timeout using select()
            fd_set write_fds, except_fds;
            struct timeval timeout;
            
            FD_ZERO(&write_fds);
            FD_ZERO(&except_fds);
            FD_SET(sock_fd, &write_fds);
            FD_SET(sock_fd, &except_fds);
            
            timeout.tv_sec = timeout_seconds;
            timeout.tv_usec = 0;
            
            int select_result = select(sock_fd + 1, NULL, &write_fds, &except_fds, &timeout);
            
            if (select_result <= 0)
            {
                if (select_result == 0)
                {
                    LOG(ERROR) << "Connection timeout after " << timeout_seconds << " seconds";
                }
                else
                {
                    LOG(ERROR) << "Select failed: " << SOCKET_ERRNO;
                }
#ifdef _WIN32
                closesocket(sock_fd);
#else
                close(sock_fd);
#endif
                return -1;
            }
            
            // Check if connection was successful
            int so_error = 0;
            socklen_t len = sizeof(so_error);
            if (getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, (char*)&so_error, &len) < 0 || so_error != 0)
            {
                LOG(ERROR) << "Connection failed: " << (so_error ? so_error : SOCKET_ERRNO);
#ifdef _WIN32
                closesocket(sock_fd);
#else
                close(sock_fd);
#endif
                return -1;
            }
        }
        
        // Set socket back to blocking mode for normal operation
#ifdef _WIN32
        mode = 0;
        ioctlsocket(sock_fd, FIONBIO, &mode);
#else
        int flags_blocking = fcntl(sock_fd, F_GETFL, 0);
        fcntl(sock_fd, F_SETFL, flags_blocking & ~O_NONBLOCK);
#endif
        
        return sock_fd;
    }

    sk_network_connection sk_open_tcp_connection(const char *host, unsigned short port)
    {
        internal_sk_init();

        sk_network_connection result;
        result.id = NETWORK_CONNECTION_PTR;
        result.kind = UNKNOWN;
        result._socket = nullptr;

        // If host is null, we're creating a server socket - use the original blocking method
        if (host == nullptr)
        {
            IPaddress addr;
            if (SDLNet_ResolveHost(&addr, nullptr, port) < 0)
                return result;

            TCPsocket server = SDLNet_TCP_Open(&addr);
            if (server)
            {
                result.kind = TCP;
                result._socket = server;
            }
            else
            {
                LOG(ERROR) << "SDLNet_TCP_Open (server): " << SDLNet_GetError();
            }
            return result;
        }

        // For client connections, use non-blocking connect with timeout
        int sock_fd = _connect_with_timeout(host, port, SK_CONNECTION_TIMEOUT_SECONDS);
        
        if (sock_fd < 0)
        {
            return result;  // Connection failed
        }

        // Successfully connected! Now we need to wrap this in SDL_net's TCPsocket
        // Unfortunately, SDL_net doesn't provide a way to create a TCPsocket from a raw fd.
        // So we close the test connection and use SDL_net for the actual connection.
        // Since we verified the connection works, SDLNet_TCP_Open should succeed quickly.
#ifdef _WIN32
        closesocket(sock_fd);
#else
        close(sock_fd);
#endif

        // Now use SDL_net - the connection should succeed immediately since we verified connectivity
        IPaddress addr;
        if (SDLNet_ResolveHost(&addr, host, port) < 0)
            return result;

        TCPsocket client = SDLNet_TCP_Open(&addr);

        if (client)
        {
            result.kind = TCP;
            result._socket = client;
            SDLNet_TCP_AddSocket(_sockets, client);
        }
        else
        {
            result.kind = UNKNOWN;
            result._socket = nullptr;
            LOG(ERROR) << "SDLNet_TCP_Open: " << SDLNet_GetError();
        }

        return result;
    }

    int sk_send_bytes(sk_network_connection *con, char *buffer, unsigned long size)
    {
        // not entry point...
        //    printf("Sending %d\n", size);
        int sent = 0;
        if ((TCPsocket)con->_socket)
        {
            //printf("here -- %p\n", (TCPsocket)con->_socket);
            sent = SDLNet_TCP_Send((TCPsocket)con->_socket, buffer, static_cast<int>(size));
        }
        //    printf("Sent %d\n", sent);
        return sent;
    }

    int sk_send_udp_message(sk_network_connection *con, const char *host, unsigned short port, const char *buffer, unsigned long size)
    {
        // Not entry point.
        UDPpacket packet;
        SDLNet_ResolveHost(&packet.address, host, port);

        packet.len = static_cast<int>(size);
        packet.data = (Uint8*)buffer;
        return SDLNet_UDP_Send((UDPsocket)con->_socket, -1, &packet);
    }

    void sk_read_udp_message(sk_network_connection *con, unsigned int *host, unsigned short *port, char *buffer, unsigned long *size)
    {
        //    printf("Reading up to %d bytes\n", *size);

        UDPpacket *packet;
        packet = SDLNet_AllocPacket((int)*size);

        *size = 0;
        *host = 0;
        if ( SDLNet_UDP_Recv((UDPsocket)con->_socket, packet) > 0 )
        {
            //        printf("Read %d bytes\n", packet->len);

            *host = SDLNet_Read32(&packet->address.host);
            *port = SDLNet_Read16(&packet->address.port);

            if ( packet->len > 0 )
            {
                unsigned int buf_sz = sizeof(char) * (unsigned int)packet->len;
                memcpy(buffer, packet->data, buf_sz);
                *size = buf_sz;
            }
        }
        SDLNet_FreePacket(packet);
    }

    int sk_read_bytes(sk_network_connection *con, char *buffer, int size)
    {
        // not entry point
        return SDLNet_TCP_Recv((TCPsocket)con->_socket, buffer, size);
    }

    void sk_close_connection(sk_network_connection *con)
    {
        // not entry point
        if ( con->kind == TCP )
        {
            SDLNet_TCP_DelSocket(_sockets, (TCPsocket)con->_socket);
            SDLNet_TCP_Close((TCPsocket)con->_socket);
        }
        else
        {
            SDLNet_UDP_DelSocket(_sockets, (UDPsocket)con->_socket);
            SDLNet_UDP_Close((UDPsocket)con->_socket);
        }

        con->kind = UNKNOWN;
    }

    unsigned int sk_network_address(sk_network_connection *con)
    {
        IPaddress *remote;
        if (con->kind == TCP)
            remote = SDLNet_TCP_GetPeerAddress((TCPsocket)con->_socket);
        else
            remote = SDLNet_UDP_GetPeerAddress((UDPsocket)con->_socket, -1);
        return SDLNet_Read32(&remote->host);
    }

    unsigned int sk_get_network_port(sk_network_connection *con)
    {
        IPaddress *remote;
        if (con->kind == TCP)
            remote = SDLNet_TCP_GetPeerAddress((TCPsocket)con->_socket);
        else
            remote = SDLNet_UDP_GetPeerAddress((UDPsocket)con->_socket, -1);
        return SDLNet_Read16(&remote->port);
    }

    sk_network_connection sk_accept_connection(sk_network_connection &con)
    {
        sk_network_connection result;
        result._socket = NULL;
        result.kind = UNKNOWN;
        
        TCPsocket client;
        if ((client = SDLNet_TCP_Accept((TCPsocket)con._socket)) != NULL)
        {
            //        printf("Adding client %p\n", client);
            SDLNet_TCP_AddSocket(_sockets, client);
            result._socket = client;
            result.kind = TCP;
        }
        return result;
    }
    
    unsigned int sk_network_has_data()
    {
        internal_sk_init();
        if (SDLNet_CheckSockets(_sockets, 0) > 0) return 1;
        else return 0;
    }
    
    unsigned int sk_connection_has_data(sk_network_connection *con)
    {
        int got = SDLNet_SocketReady(con->_socket);
        
        //    printf("Checking %p %d\n", con->_socket, got);
        if (got > 0)
            return 1;
        else
            return 0;
    }
}
