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
	SERVER_ADDRESS=c;
	SOCKET client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int flag=1;
	setsockopt(client,SOL_SOCKET,SO_BROADCAST,(const char*)&flag,sizeof(flag));
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT);
	char buf[MSGSIZE];
	cout<<"ready to broadcast on port "<<PORT<<endl;
	string tmp;
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
