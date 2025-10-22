#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Container.h"
#include "Str.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

//https://docs.microsoft.com/en-us/windows/win32/winsock/disconnecting-the-server
//https://docs.microsoft.com/en-us/windows/win32/winsock/creating-a-socket-for-the-client

int main()
{
	WSADATA wsaData = { 0 };

	int iResult = 0;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "WSAStartup failed: " << iResult << endl;
		return 1;
	}


#define DEFAULT_PORT "27015"

	struct addrinfo* result = nullptr, * ptr = nullptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		cout << "getaddrinfo failed: " << iResult << endl;
		WSACleanup();
		return 1;
	}

	SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		cout << "bind failed with error: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Listen failed with error: " << WSAGetLastError() << endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	wcout << endl << L"Looking for connections..." << endl;

	SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "Accept failed: " << WSAGetLastError() << endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	cout << "Connection accepted." << endl;

	cout << endl << "[Server]: Connection is sucess, waiting for commands.";
	String FirstLine = "Connection is sucess, waiting for commands.";
	iResult = send(ClientSocket, FirstLine.ConstWchar(), FirstLine.Length(), 0);
	if (iResult < 0)
	{
		cout << "Send failed." << endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

#define DEFAULT_BUFLEN 512

	char* recvbuf = new char[DEFAULT_BUFLEN];
	int iSendResult;
	iResult = 0;
	int recvbuflen = DEFAULT_BUFLEN;

	do
	{
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			String RawRecived(recvbuf);

			cout << "[Client]: ";
			for (int i = 0; i < iResult; i++)
				cout << recvbuf[i];

			cout << endl << "[Server]: ";

			StringList Parts = RawRecived.Split(' ');
			String Result;
			if (Parts[0] == "retrive")
			{
				fstream File;
				File.open("thisdata.txt", ios::in | ios::out | ios::app);

				char* FileBuffer = new char[512];
				while (File)
				{
					ZeroMemory(FileBuffer, sizeof(char) * 512);
					File.read(FileBuffer, 512);

					int SendResult = send(ClientSocket, FileBuffer, 512, 0);
					if (SendResult == SOCKET_ERROR)
					{
						cout << "Send Failed" << endl;
						break;
					}
				}

				send(ClientSocket, "<>", 3, 0);
				Result = "Retrived Sucess.";
			}
			else if (Parts[0] == "set")
			{
				Result = "Undefined";
			}
			else
				Result = "Unknown command.";

			iSendResult = send(ClientSocket, Result.ConstWchar(), Result.Length(), 0);
			if (iSendResult == SOCKET_ERROR)
			{
				cout << "Send Failed: " << WSAGetLastError() << endl;
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
		}
		else if (iResult == 0)
			cout << "Connection closing" << endl;
		else
		{
			cout << "recv failed: " << WSAGetLastError() << endl;
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);

	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		cout << "Shutdown failed: " << WSAGetLastError() << endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	closesocket(ClientSocket);
	WSACleanup();


	return 0;
}