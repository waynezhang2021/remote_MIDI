#include<bits/stdc++.h>
#include<windows.h> 
using namespace std;
double reference;
int port=442;
SOCKET init()
{
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	return sockSrv;
}
inline HMIDIOUT midi_open()
{
	HMIDIOUT h;
	midiOutOpen(&h,0,0,0,0);
	return h;
}
inline void midi_close(HMIDIOUT h)
{
	midiOutClose(h);
	return;
}
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
inline void midi_event(HMIDIOUT device,string byte1,string byte2,string byte3)
{
	midiOutShortMsg(device,hex_string_to_value(byte1)|hex_string_to_value(byte2)<<8|hex_string_to_value(byte3)<<16);
}
void reset_instruments(HMIDIOUT device)
{
	for(int i=0;i<16;i++)
		midiOutShortMsg(device,0x00C0|i);
}
int main()
{
	int MSGSIZE=1048576;
	HMIDIOUT midi_device=midi_open();
	stringstream ss;
	double time;
	string note;
	string eventbyte1,eventbyte2,eventbyte3;
	SOCKET s=init();
	char buf[MSGSIZE];
	int len=sizeof(SOCKADDR),ret;
	SOCKADDR_IN addr;
	cout<<"receiving on port "<<port<<endl;
	while(1)
	{
		ret=recvfrom(s,buf,MSGSIZE,0,(SOCKADDR*)&addr,&len);
	    if(ret<=0)
	    	continue;
	    buf[ret] = '\0';
	    if(strcmp(buf,"reset")==0||strcmp(buf,"reset\n")==0||strcmp(buf,"reset\r")==0||strcmp(buf,"reset\n\r")==0||strcmp(buf,"reset\r\n")==0)
	    {
	    	reset_instruments(midi_device);
	    	cout<<"reset instruments\n";
	    	Sleep(500);
	    	continue;
		}
		ss.clear();
		ss<<string(buf);
		while(ss>>eventbyte1>>eventbyte2>>eventbyte3)
		{
			if((hex_string_to_value(eventbyte1)&0xf0)==0x90)
				cout<<translate_note(hex_string_to_value(eventbyte2))<<" ";
			midi_event(midi_device,eventbyte1,eventbyte2,eventbyte3);
		}
	}
	return 0;
}
