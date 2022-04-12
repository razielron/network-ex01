#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>

#define TIME_PORT	27015

void printMenu() {
	cout << "MENU:" << endl;
	cout << "press 0 to EXIT" << endl;
	cout << "press 1 for GetTime" << endl;
	cout << "press 2 for GetTimeWithoutDate" << endl;
	cout << "press 3 for GetTimeSinceEpoch" << endl;
	cout << "press 4 for GetTime" << endl;
	cout << "press 5 for GetTime" << endl;
	cout << "press 6 for GetTimeWithoutDateOrSeconds" << endl;
	cout << "press 7 for GetYear" << endl;
	cout << "press 8 for GetMonthAndDay" << endl;
	cout << "press 9 for GetTime" << endl;
	cout << "press 10 for GetTime" << endl;
	cout << "press 11 for GetTime" << endl;
	cout << "press 12 for GetTime" << endl;
	cout << "press 13 for GetTime" << endl;
}

bool validSendBuff(char(&sendBuff)[255]) {
	char firstNum;
	char secondNum;
	char thirdNum;

	firstNum = sendBuff[0] - '0';
	secondNum = sendBuff[1] - '0';
	thirdNum = sendBuff[2] - '0';

	if (secondNum == '\0' - '0') {
		return firstNum >= 0;
	}
	else {
		return firstNum == 1 && secondNum >= 0 && secondNum <= 3 && thirdNum == '\0' - '0';
	}
}

bool getUserInput(char(&sendBuff)[255]) {
	printMenu();
	cin >> sendBuff;

	while (!validSendBuff(sendBuff)) {
		cin >> sendBuff;
	}

	return sendBuff[0] != '0';
}

void main()
{

	// Initialize Winsock (Windows Sockets).

	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Client: Error at WSAStartup()\n";
		return;
	}

	// Client side:
	// Create a socket and connect to an internet address.

	SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connSocket)
	{
		cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// For a client to communicate on a network, it must connect to a server.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called server. 
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(TIME_PORT);

	// Send and receive data.

	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[255] = "0";
	char recvBuff[255];
	bool validInput = true;

	validInput = getUserInput(sendBuff);

	while (validInput) {
		// Asks the server what's the currnet time.
		// The send function sends data on a connected socket.
		// The buffer to be sent and its size are needed.
		// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
		// The two last arguments hold the details of the server to communicate with. 
		// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
		bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
		if (SOCKET_ERROR == bytesSent)
		{
			cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
			closesocket(connSocket);
			WSACleanup();
			return;
		}
		cout << "\nTime Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

		// Gets the server's answer using simple recieve (no need to hold the server's address).
		bytesRecv = recv(connSocket, recvBuff, 255, 0);
		if (SOCKET_ERROR == bytesRecv)
		{
			cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
			closesocket(connSocket);
			WSACleanup();
			return;
		}

		recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
		cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n\n\n\n";

		validInput = getUserInput(sendBuff);
	}

	// Closing connections and Winsock.
	cout << "Time Client: Closing Connection.\n";
	closesocket(connSocket);

	system("pause");
}