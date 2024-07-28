#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <thread>
#include <vector>
#include <set>
#include <algorithm>

#ifdef _WIN32
#include <io.h>
#include <winsock2.h>
#include <sys/types.h>
#include <ws2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

#elif defined(__linux__)
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

using namespace std;

#define MESSAGE_BUFFER 1024 // Maximum siae of buffer for sending and receiving
#define PORT 7777 // Port number which we will use for sending and receiving 

#if defined (__linux__)
#define SOCKET int
#define INVALID_SOCKET -1
#define closesocket close
#define ZeroMemory bzero
#define SOCKET_ERROR -1
#endif

vector<thread> ths;
//vector<SOCKET> connections;
set<SOCKET> connections;

void connection(SOCKET clientSocket)
{
    char buffer[MESSAGE_BUFFER];
	cout << "Thread ID: " << this_thread::get_id() << endl;
    while (true)
    {
        ZeroMemory(buffer, MESSAGE_BUFFER);

        // Wait for client to send data
        #ifdef _WIN32
        int bytesReceived = recv(clientSocket, buffer, MESSAGE_BUFFER, 0);
        #elif defined(__linux__)
        int bytesReceived = read(clientSocket, buffer, MESSAGE_BUFFER);
        #endif
        cout << "buffer " << buffer << " bytes " << bytesReceived << endl;
        if (bytesReceived == SOCKET_ERROR)
            break;

        if (bytesReceived == 0)
        {
            cout << "Client disconnected" << endl;
            close(clientSocket);
            return;
        }

        // Send message to all connections
        for (auto it = connections.begin(); it != connections.end(); it++)
        {
        	#ifdef _WIN32
        	send((*it), buffer, bytesReceived, 0);
        	#elif defined(__linux__)
        	if ((*it) != clientSocket)
        		write((*it), buffer, bytesReceived);
        	#endif
        }
    }
}


void processRequest() {
	#ifdef _WIN32
    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsok = WSAStartup(ver, &wsData);
    if (wsok != 0)
    {
        cerr << "Can't Initialize winsock! Quitig" << endl;
        return;
    }
	#endif
    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Can't create a socket! Quiting" << endl;
        return;
    }
	
    // Bind an ip address and port to the socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    hint.sin_addr.s_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    #ifdef _WIN32
    int clientSize = sizeof(client);
    #elif defined(__linux__)
    socklen_t clientSize = sizeof(client);
    #endif
    while (true)
    {
        SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if (clientSocket == INVALID_SOCKET)
        {
            cout << "Invalid Socket" << endl;
            return;
        }
        connections.insert(clientSocket);
        ths.push_back(thread(connection, clientSocket));
        
        for (auto &th : ths)
        	cout << th.get_id() << endl;
        
        cout << 1 << endl;
    }

    closesocket(listening);
    // Close the socket

    for (auto &th : ths)
        th.join();
	cout << 5 << endl;
    for (auto &c : connections)
        closesocket(c);
	
	#ifdef _WIN32
    // Cleanup winsock
    WSACleanup();
    #endif
}

int main() {
    cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << endl;
    // launching the function of processing messages from clients and responding to these messages
    processRequest();
    return 0;
}
