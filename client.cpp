#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	int PORT=442,MSGSIZE=1024;
	string SERVER_ADDRESS;
	char c[128];
	cout<<"send to udp://";
	scanf("%s",c);
	SERVER_ADDRESS=c;
	SOCKET client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS.c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT);
	char buf[MSGSIZE];
	cout<<"ready to send to "<<SERVER_ADDRESS<<":"<<PORT<<endl;
	string tmp;
	getline(cin,tmp);
	while(1)
	{
		printf("send:");
		getline(cin,tmp);
		tmp+="\n";
		strcpy(buf,tmp.c_str());
		sendto(client,buf,strlen(buf),0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	}
	closesocket(client);
	WSACleanup();
	return 0;
}
