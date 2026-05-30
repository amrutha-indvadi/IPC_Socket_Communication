#define _CRT_SECURE_NO_WARNINGS
#define _CTR_SECURE_NO_DEPRICATE
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // link library to executable built

int main() {
    char buffer[512];
    char message[90];
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
    {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }
    printf("Winsock initialized.\n");

    // Create a socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
    {
        printf("Could not create socket\n");
        WSACleanup();
        return 1;
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("IP_port"); // Connect to localhost
    server_addr.sin_port = htons(8888); // Port to connect to

    // Connect to remote server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Connect failed with error code.\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    printf("Connected to server.\n");

    while (message)
    {
        // Send data to server
        printf("Client message: ");
        fgets(message, sizeof(message), stdin);
        if (strcmp(message, "end")==0)
            exit(0);
        else
            send(client_socket, message, strlen(message), 0);

        // Receive data from server
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) 
        {
            buffer[bytes_received] = '\0';
            printf("Server reply: %s\n", buffer);
        }
    }

    // Clean up
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
