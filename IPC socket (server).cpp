#define _CRT_SECURE_NO_WARNINGS
#define _CTR_SECURE_NO_DEPRICATE
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib") // link library to executable built

int main() 
{
    char buffer[500];
    char message[50];
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    int client_address_length;

    struct sockaddr_in 
        server_address, 
        client_address;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
    {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }
    printf("Winsock initialized.\n");

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
    {
        printf("Could not create socket.\n");
        WSACleanup();
        return 1;
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // Accept connections on any IP address
    server_address.sin_port = htons(8888); // Port to listen on

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) 
    {
        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Bind done.\n");

    // Listen for incoming connections
    listen(server_socket, 10); 
    printf("Waiting for client.\n");

    // Accept an incoming connection
    client_address_length = sizeof(struct sockaddr_in);
    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length)) == INVALID_SOCKET)
    {
        printf("Accept failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("Connection to client.\n");

    while (message)
    {
        // Receive data from client
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            printf("Client message: %s\n", buffer);
        }

        // Send a reply to the client
        printf("Server message: ");
        fgets(message, sizeof(message), stdin);
        if (strcmp(message, "end")==0)
            exit(0);
        else
            send(client_socket, message, strlen(message), 0);
    }
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}

