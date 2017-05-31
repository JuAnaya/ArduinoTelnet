
#include "TelnetClass.h"
#include <UIPEthernet.h>
#include <stdio.h>

//#define SPI_SD 3
#define SPI_ETHERNET 53 
#define FW 2.31

EstacionConfig estacionConfig;
//Ethernet & Telnet
EthernetServer server(23);
TelnetClass telnetClass;

//Program
bool Running = false;

// the setup function runs once when you press reset or power the board
void setup() {
	//***********************************************************************
	//***********************************************************************
	estacionConfig.estacionId = 3000;
	MacAddress mac = MacAddress(0xDF, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF);
	estacionConfig.ip = "172.16.7.150";
	estacionConfig.servidor_dns = "172.16.0.36";
	estacionConfig.puerta_enlace = "172.16.0.11";
	estacionConfig.mascara_red = "255.255.0.0";
	estacionConfig.mac = mac;
	estacionConfig.server = "172.16.0.37";
	estacionConfig.updPort = 8006;
	estacionConfig.interval = 600000; //10 mins

	estacionConfig.ver = FW;
	Running = false;
	////Serial
	//Serial.begin(9600);

	pinMode(SPI_ETHERNET, OUTPUT);
	digitalWrite(SPI_ETHERNET, LOW);

	//EHERNET & TELNET
	telnetClass.init(&server, &estacionConfig);
	Running = true;

}

// the loop function runs over and over again until power down or reset
void loop() {
	if (Running)
	{
		telnetClass.Update();

	}
}