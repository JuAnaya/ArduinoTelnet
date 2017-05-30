
#include "TelnetClass.h"
#include <SoftReset.h>

TelnetClass::TelnetClass()
{

}


TelnetClass::~TelnetClass()
{
	if (client)
	{
		client.stop();
		connectFlag = 0;
	}
}


void TelnetClass::init(EthernetServer* server, EstacionConfig* config)
{
	_server = server;


	MacAddress mac;
	ip = IPAddress();
	servidor_dns = IPAddress();
	puerta_enlace = IPAddress();
	mascara_red = IPAddress();

	_config = config;

	ip.fromString(_config->ip);
	servidor_dns.fromString(_config->servidor_dns);
	puerta_enlace.fromString(_config->puerta_enlace);
	mascara_red.fromString(_config->mascara_red);
	mac = _config->mac;


	Ethernet.begin(mac.mac, ip, servidor_dns, puerta_enlace, mascara_red);
	_server->begin();
	//*****************************************************

}


/////////////////////////////////////////////// ip del telnet 



void TelnetClass::Update()
{
	executeServerLogic();
}


void TelnetClass::executeServerLogic()
{
	if (connectFlag && !client.connected())
	{
		connectFlag = 0;
		client.stop();
	}

	if (_server->available() && !connectFlag) {
		connectFlag = 1;
		client = _server->available();
		printGreetingMessage();
		printPrompt();
	}
	if (client.connected() && client.available())
		getReceivedText();

	if (connectFlag)
		checkConnectionTimeout();
}

void TelnetClass::checkConnectionTimeout()
{
	if (millis() - timeOfLastActivity > allowedConnectTime) {
		client.println();
		client.println("Timeout - desconectado");
		client.stop();
		connectFlag = 0;
	}
}
void TelnetClass::getReceivedText()
{
	char c;
	int charsWaiting;
	charsWaiting = client.available();
	do {
		c = client.read();
		textBuff[charsReceived] = c;
		charsReceived++;
		charsWaiting--;
	} while (charsReceived <= textBuffSize && c != 0x0d && charsWaiting > 0);
	textBuff[charsReceived] = '\0';

	if (c == 0x0d) {
		parseReceivedText();
		printPrompt();
	}
}

//CMD SELECTOR
void TelnetClass::parseReceivedText()
{
	switch (textBuff[0])
	{
	case 'd':
		if (CommunicationProcess())
			agregarFuncion();
		break;
	case 'i':
		if (CommunicationProcess())
			printInfo();
		break;
	case 'a':
		if (CommunicationProcess())
			agregarFuncion();
		break;
	case 'l':
		if (CommunicationProcess()) {
			agregarFuncion();
		}
		break;

	case 'c':
		if (CommunicationProcess())
		{
			client.println("OK");
			checkCloseConnection();
		}
		break;
	case 't':
		if (CommunicationProcess())
		{
			agregarFuncion();
		}
		break;
	case 'r':
		if (CommunicationProcess())
		{
			closeConnection();
			soft_restart();
		}
		break;
	case '?':
		if (CommunicationProcess())
		{
			client.println("OK");
			printHelpMessage();
		}
		break;
	case 0x0d:
		break;
	default:
		if (CommunicationProcess())
			printErrorMessage();
		break;
	}
}

//CMD i
void TelnetClass::printInfo()
{

	client.println("\nEstacion Info.");
	client.print("fw ver:");
	client.println(_config->ver);
	client.print("Id:");
	client.println(_config->estacionId);
	client.print("ip:");
	client.println(_config->ip);
	client.print("gw:");
	client.println(_config->puerta_enlace);
	client.print("dns:");
	client.println(_config->servidor_dns);
	client.print("mask:");
	client.println(_config->mascara_red);
	client.print("mac:");
	char str[4];

	for (int i = 0; i < 6; i++)
	{
		sprintf(str, "%02X", _config->mac.mac[i]);
		client.print(str);
		client.print(":");
	}
	client.println("");
	client.print("Comm Interval:");
	client.println(_config->interval / 1000);
}

bool TelnetClass::CommunicationProcess()
{
	if (!client.connected())
	{
		connectFlag = 0;
		client.stop();
		return false;
	}
	else
		return true;
}

#pragma region Commands


//CMD cl
void TelnetClass::closeConnection()
{
	client.println("OK");
	client.println("\nSesion terminada.\n");
	client.stop();
	connectFlag = 0;
}

//CMD ?
void TelnetClass::printHelpMessage()
{
	client.println("\nComandos firmware v1.0:\n");
	client.println("  cl        -cerrar conexion");
	client.println("  a         -agregar funcion ");
	client.println("  l         -agregar funcion ");
	client.println("  i         -configuracion");
	client.println("  t         -agregar funcion ");
	client.println("  d         -agregar funcion ");
	client.println("  r         -reset");
	client.println("  ?	        -ayuda");

}

void TelnetClass::agregarFuncion()
{
	client.println("OK");
	client.println("\n ejecutar funcion agregada.\n");
}

#pragma endregion

void TelnetClass::checkCloseConnection()
{
	if (textBuff[1] == 'l' && textBuff[2] == 0x0d)
		closeConnection();
	else
		printErrorMessage();
}
void TelnetClass::printPrompt()
{
	timeOfLastActivity = millis();
	client.flush();
	charsReceived = 0;
	client.print("\n>");
}
void TelnetClass::printErrorMessage()
{
	client.println("Comando no reconocido  ? para ayuda.");
}

void TelnetClass::printGreetingMessage()
{
	client.println("\nServidor Telnet");
	client.println("? para ayuda");

}

#pragma region MacAddressClass

MacAddress::MacAddress()
{

}

MacAddress::MacAddress(byte m1, byte m2, byte m3, byte m4, byte m5, byte m6)
{
	mac[0] = m1;
	mac[1] = m2;
	mac[2] = m3;
	mac[3] = m4;
	mac[4] = m5;
	mac[5] = m6;
}

#pragma endregion





