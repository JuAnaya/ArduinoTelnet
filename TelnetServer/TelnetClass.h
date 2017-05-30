// TelnetClass.h
#ifndef _TELNETCLASS_h
#define _TELNETCLASS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <UIPEthernet.h>

#define textBuffSize 255 


class MacAddress
{
public:
	MacAddress();
	MacAddress(byte m1, byte m2, byte m3, byte m4, byte m5, byte m6);
	byte mac[6] = { 0,0,0,0,0,0 };
};
struct EstacionConfig
{
	int estacionId = 0;
	MacAddress mac;
	const char* ip;
	const char* servidor_dns;
	const char* puerta_enlace;
	const char* mascara_red;
	const char* server;
	int updPort = 0;
	unsigned long int interval = 900000; //10 mins
	float ver = 0;
};



class TelnetClass
{
public:
	TelnetClass();
	~TelnetClass();
	void init(EthernetServer* server, EstacionConfig* config);
	void Update();
private:
	EthernetServer* _server;
	EstacionConfig* _config;
	IPAddress ip;
	IPAddress servidor_dns;
	IPAddress puerta_enlace;
	IPAddress mascara_red;

	//REMOTE COMMANDS

	char textBuff[textBuffSize];
	int charsReceived = 0;
	boolean connectFlag = 0;
	unsigned long timeOfLastActivity;
	unsigned long allowedConnectTime = 300000;
	EthernetClient client;

	void checkConnectionTimeout();

	void executeServerLogic();
	void getReceivedText();
	void parseReceivedText();
	void checkCloseConnection();

	void closeConnection();
	void printPrompt();
	void printErrorMessage();
	void printHelpMessage();
	void printGreetingMessage();


	bool CommunicationProcess();
	void printInfo();
	void executeLogic();

};

#endif

