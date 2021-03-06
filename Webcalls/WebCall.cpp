//This file contains the entry point to the application

#include <iostream>
#include <ws2tcpip.h>
#include<winsock.h>
#include<WinSock2.h>
#include "helperFunctions.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#pragma warning(disable:4996)

int main()
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char buffer[512], fileName[512];
	int bytes;
	char *html = NULL;

	try
	{
		while (1) {

			printf("\nInitialising Winsock...");
			//Initialise Winsock
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			{
				std::cout << "Winsock initialisation failed. Error Code :" << WSAGetLastError();
				return 1;
			}

			std::cout << "Initialised.\n";

			//Create a socket
			if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
			{
				std::cout << "Could not create socket : " << WSAGetLastError();
			}

			std::cout << "Socket created." << std::endl;

			//Setup the server address
			server.sin_addr.s_addr = inet_addr("127.0.0.1");
			server.sin_family = AF_INET;
			server.sin_port = htons(80);

			//Bind to the socket
			if (bind(s, (LPSOCKADDR)&server, sizeof(server)) == SOCKET_ERROR)
			{
				//This will happen if you try to bind to the same socket more than once
				std::cout << "Could not bind with error code:" << WSAGetLastError();
				return false;
			}

			//Listen to the socket
			if (listen(s, SOMAXCONN) == SOCKET_ERROR)
			{
				std::cout << "Listen function failed with error:" << WSAGetLastError();
				return false;
			}


			std::cout << "Waiting for incoming connections..." << std::endl;

			// Create a continuous loop that checks for connections requests. 
			while (1)
			{
				SOCKET AcceptSocket = SOCKET_ERROR;

				while (AcceptSocket == SOCKET_ERROR)
				{
					AcceptSocket = accept(s, NULL, NULL);
				}

				// When the client connection has been accepted, transfer control from the
				// temporary socket to the original socket and stop checking for new connections.
				printf("Server: Client Connected!\n");
				s = AcceptSocket;


				bytes = recv(AcceptSocket, buffer, 512, 0);

				for (int i = 0; i < bytes; ++i) {
					std::cout << buffer[i];
				}

				//Call the function to parse the request and get the file name if any.
				parseRequest(buffer, fileName);

				if (strlen(fileName) > 1)
				{
					if (!readHTMLFile(fileName, &html))
					{
						readHTMLFile("..//pages//error.html", &html);
					}

				}
				else
				{
					readHTMLFile("..//pages//index.html", &html);
				}


				send(AcceptSocket, html, strlen(html) - 1, 0);


				closesocket(AcceptSocket);
				closesocket(s);
				html = NULL;
				break;

			}
			WSACleanup();

		}
	}
	catch (...)
	{
		std::cout << "WSA error" << std::endl;
		getchar();
	}


	return 0;

}





