#include <iostream>  
#include "windows.h" // �� �������� �������� ������������ ����
#include "process.h"
#include <string>
using namespace std;
#pragma comment(lib, "WS2_32.lib")  // ����������� �������� ws2_32.dll ws2_32.dll - ��������� ������ ������

void Receive(void *param)		// ������� ������, ����������� ������
{
    while (true)
    {
        // ������ ��������� ������
        SOCKET revClientSocket = *(SOCKET*)(param);
        char recvbuf[2048] = {};		// �������� �����
        if (recv(revClientSocket, recvbuf, 2048, 0) == SOCKET_ERROR)
        {
            cout << "�� ������� ������� ������!" << endl;
        }
        else
            cout << "[������] ������:" << recvbuf << endl;
    }
}

void Send(void *param)		// ������� ������ ��� �������� ������
{
    while (1)
    {
        // ������ ���������� ������
        SOCKET revClientSocket = *(SOCKET*)(param);
        char sendbuf[2048] = {};		// ���������� �����
        cin.getline(sendbuf, 2048);
        if (send(revClientSocket, sendbuf, strlen(sendbuf), 0) == SOCKET_ERROR)
        {
            cout << "�� ������� ��������� ���������!" << endl;
        }
        else
            cout << "[� ������:" << sendbuf << endl;
    }
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
    cout << "----------- ������ -----------" << endl;

	// ������������� ������
	WSADATA wsaData;	// ��� ��������� ������������ ��� �������� ������ Windows Sockets, ������������ ����� ������ ������� WSAStartup.
	WORD sockVersion = MAKEWORD(2, 2);	// ���������� � ������ ������ ���������� �������� ���������������� Windows
	if (WSAStartup(sockVersion, &wsaData) != 0) // ������� WSAStartup ������������� ��� ������������� � �������� ���� Windows � ���������
	{
		cout << "������ ������������� ������!" << endl;
		return 0;
	}

	// ������� ��������� �����
	SOCKET SeverSocket;
	if ((SeverSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "�� ������� ������� �����!" << endl;
		return 0;
	}
	struct sockaddr_in SeverAddress;		// ����� ��������: ���� IP-�����, ����� ����� � ��������� ����������
	memset(&SeverAddress, 0, sizeof(sockaddr_in)); // �������������� ���������
	SeverAddress.sin_family = AF_INET;
	SeverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);// ��������� ��������� IP-����� 
	SeverAddress.sin_port = htons(60000);// ������������� ����� �����

	// ��������� ����� ������� ����������� IP-����� � ����� �����
	if (bind(SeverSocket, (sockaddr*)&SeverAddress, sizeof(SeverAddress)) == SOCKET_ERROR)
	{
		cout << "�� ������� ��������� �����!"<<endl;
		return 0;
	}

	// ���������� �������	
	if (listen(SeverSocket, SOMAXCONN) == SOCKET_ERROR) // ������ �������� �����������: ������� ���������� �������� ����� ���������, ��� ����� ������� ��� ������������� ����������������.
	{
		cout << "���� �����������!"<<endl;
		return 0;
	}
	else
		cout << "������ �������:"<<endl;

	// ������ ��������� ������ �� �����������
	sockaddr_in revClientAddress;	// ����� � ���� ������
	SOCKET revClientSocket = INVALID_SOCKET;		// ������������ ��� ��������� ���������� �����������
	//memset(&revClientAddress, 0, sizeof(revClientAddress));
	int addlen = sizeof(revClientAddress);
	if ((revClientSocket = accept(SeverSocket, (sockaddr*)&revClientAddress, &addlen)) == INVALID_SOCKET)
	{
		cout << "�� ������� ������� ���������� ����������!" << endl;
		return 0;
	}
	else
		cout << "������� ������� ���������� ����������!"<<endl;

	// ������� ��� �������� ������
	_beginthread(Receive, 0, &revClientSocket);
	_beginthread(Send, 0, &revClientSocket);

	while (true) {}  // ����� ������������� ����� �� ��������� ������, �������� ����� �������� �������, � �������� ����� ������ � ����

	// ��������� �����
	closesocket(revClientSocket);
	closesocket(SeverSocket);

	// ����������
	WSACleanup();
	cout << "������ ����������!" << endl;
    
    return 0;
}
