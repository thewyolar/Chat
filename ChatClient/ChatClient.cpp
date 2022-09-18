#include "windows.h"
#include "iostream"
#include "process.h"
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void Receive(void *param)
{
	while (1)
	{
		// ������ ��������� ������ � �������
		SOCKET clientSocket = *(SOCKET*)(param);
		char  recvbuf[2048] = {};		// �������� �����
		if (recv(clientSocket, recvbuf, 2048, 0) == SOCKET_ERROR)
		{
			cout << "�� ������� ������� ������" << endl;
		}
		else
			cout << "[������] ������:" << recvbuf << endl;
	}
}

void Send(void *param)
{
	while (1)
	{
		// ������ ���������� ������ �� ������
		SOCKET clientSocket = *(SOCKET*)(param);
		char sendbuf[2048] = {};		// ���������� �����
		cin.getline(sendbuf, 2048);
		if (send(clientSocket, sendbuf, strlen(sendbuf), 0) == SOCKET_ERROR)
		{
			cout << "�� ������� ��������� ���������!";
		}
		else
			cout << "[� ������:" << sendbuf << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	cout << "----------- ������ -----------" << endl;
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "������ ������������� ������!"<<endl;
	}
	SOCKET clientSocket;
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "�� ������� ������� �����!"<<endl;
	}
	struct sockaddr_in SeverAddress;		// ����� ������� - ��� ������� ����� ��� �����������
	memset(&SeverAddress, 0, sizeof(sockaddr_in)); 
	SeverAddress.sin_family = AF_INET;
	SeverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //127.0.0.1 ������������ ��������� IP-�����
	SeverAddress.sin_port = htons(60000);// ������������� ����� �����

	// ��������� ����������
	if (connect(clientSocket, (sockaddr*)&SeverAddress, sizeof(SeverAddress)) == SOCKET_ERROR)
	{
		cout << "������: �� ������� ������������ � �������!" << endl;
		return 0;
	}
	else
		cout << "���������� � ������� �������!" << endl;

	// ������� ��� �������� ������
	_beginthread(Receive, 0, &clientSocket);
	_beginthread(Send, 0, &clientSocket);

	Sleep(INFINITE);	// ����� ������������ ������ �����, ����� ������������� ����� �� ��������� ������ ����� ���������� - ��������� ��� ����� �� �������������� ����
	// ��������� �����
	if (clientSocket != INVALID_SOCKET) {
		closesocket(clientSocket);
		clientSocket = INVALID_SOCKET;
	}

	// ���������
	WSACleanup();
	cout << "������ ������!" << endl;
    return 0;
}