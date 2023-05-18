#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>
#include <string>

#include "Container.h"
#include "Str.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main() 
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		cout << "Init failed" << endl;
		return 1;
	}

	struct addrinfo* result = nullptr, * ptr = nullptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

#define DEFAULT_PORT "27015"

	cout << "Please enter the server's ip: ";
	string IPAddress;
	getline(cin, IPAddress);

	iResult = getaddrinfo(IPAddress.c_str(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		cout << "Address not found." << endl;
		WSACleanup();
		return 1;
	}

	ptr = result;

	SOCKET ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "Could not create socket." << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "Connection failed." << endl;
		WSACleanup();
		return 1;
	}

#define DEFAULT_BUFLEN 512

	int recvbuflen = DEFAULT_BUFLEN;
	char* recvbuf = new char[DEFAULT_BUFLEN];

	// Receive data until the server closes the connection
	do 
	{
		ZeroMemory(recvbuf, DEFAULT_BUFLEN);

		char* Buffer = new char[512];
		while (Buffer && Buffer[0] != '<' && Buffer[1] != '>')
		{
			int Len = 0;
			if ((Len = recv(ConnectSocket, Buffer, 512, 0)) == SOCKET_ERROR)
			{
				cout << "Recive Failed" << endl;
				break;
			}

			if (Len == 2 && Buffer[0] == '<' && Buffer[1] == '>')
				break;

			for (int i = 0; i < Len; i++)
				cout << Buffer[i];
		}

		cout << endl << endl << "End file." << endl;

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			cout << "[Server]: ";
			for (int i = 0; i < iResult; i++)
				cout << recvbuf[i];

			cout << endl << "[Client]: ";

			String Line;
			getline(cin, Line);

			if (Line == "exit")
				break;

			StringList Parts = Line.Split(' ');
			if (Parts[0] == "retrive")
			{
				if (send(ConnectSocket, "retrive", 8, 0) == SOCKET_ERROR)
				{
					cout << "Send Failed" << endl;
					break;
				}

				
			}

			int iSendResult = send(ConnectSocket, Line.ConstWchar(), (int)Line.Length(), 0);
			if (iSendResult == SOCKET_ERROR)
			{
				cout << "Send Failed: " << WSAGetLastError() << endl;
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
		}
		else if (iResult == 0)
			cout << "Connection closed." << endl;
		else
			cout << "Recive failed: " << WSAGetLastError() << endl;
	} while (iResult > 0);

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}