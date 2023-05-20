#include<WinSock2.h>
#include<chrono>
#include<stdio.h>
#include<iostream>
#include<string>
#include<sstream>
using namespace std;
double reference=0;
string translate_note(BYTE note,short transpose=0)
{
	static string table[128]=
	{
		"C-1","C#-1","D-1","D#-1","E-1","F-1","F#-1","G-1","G#-1","A-1","A#-1","B-1",
		"C0","C#0","D0","D#0","E0","F0","F#0","G0","G#0","A0","A#0","B0",
		"C1","C#1","D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1",
		"C2","C#2","D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2",
		"C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3",
		"C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4",
		"C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5",
		"C6","C#6","D6","D#6","E6","F6","F#6","G6","G#6","A6","A#6","B6",
		"C7","C#7","D7","D#7","E7","F7","F#7","G7","G#7","A7","A#7","B7",
		"C8","C#8","D8","D#8","E8","F8","F#8","G8","G#8","A8","A#8","B8",
		"C9","C#9","D9","D#9","E9","F9","F#9","G9"
	};
	return ((note+transpose<=127&&note+transpose>=0)?table[note+transpose]:"invalid");
}
inline double precise_clock()
{
	return chrono::high_resolution_clock::now().time_since_epoch().count()/1000000000.0-reference;
}
inline short hex_string_to_value(string hex_string)
{
	return ((hex_string[0]<='9')?(hex_string[0]-'0'):(hex_string[0]-'A'+10))<<4|((hex_string[1]<='9')?(hex_string[1]-'0'):(hex_string[1]-'A'+10));
}
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
	string file,eventbyte1,eventbyte2,eventbyte3;
	FILE* fp;
	stringstream ss;
	double time;
	string line;
	while(1)
	{
		cout<<"play MIDI file:";
		cin>>file;
		if((fp=fopen(file.c_str(),"r"))==NULL)
		{
			cout<<"nonexistent file!";
			Sleep(1000);
			system("cls");
			continue;
		}
		else
			fclose(fp);
		sendto(client,"reset",5,0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
		fp=popen((string("echo ")+file+"|list_events.exe").c_str(),"r");
		fgets(buf,1024,fp);
		reference=0;
		reference=precise_clock();
		do
		{
			ss.clear();
			ss<<buf;
			eventbyte1=eventbyte2=eventbyte3="00";
			ss>>time>>eventbyte1>>eventbyte2>>eventbyte3;
			while(precise_clock()<time)
				if(GetAsyncKeyState(VK_ESCAPE)&0x8000)
					goto stop;
			if((hex_string_to_value(eventbyte1)&0xf0)==0x90)
				cout<<translate_note(hex_string_to_value(eventbyte2))<<" ";
			line=eventbyte1+" "+eventbyte2+" "+eventbyte3;
			sendto(client,line.c_str(),line.length(),0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
			fgets(buf,1024,fp);
		}
		while(!feof(fp));
stop:
		pclose(fp);
		system("cls");	
	}
	closesocket(client);
	WSACleanup();
	return 0;
}
