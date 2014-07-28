/*
  WiFlyTCP.h - WiFly Library for Arduino
*/
// ensure this library description is only included once
#ifndef _WIFLTTCP_H_
#define _WIFLTTCP_H_


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Stream.h>
#include <avr/pgmspace.h>
#include <IPAddress.h>

#define WIFLY_WLAN_AUTH_OPEN		0x00
#define WIFLY_WLAN_AUTO_WEP128		0x01
#define WIFLY_WLAN_AUTO_WPA1		0x02
#define WIFLY_WLAN_AUTO_WPA1_WPA2	0x03
#define WIFLY_WLAN_AUTO_WPA2_PSK	0x04

// library interface description

class WiFlyTCP {

	public:
  
	WiFlyTCP();
	void begin(Stream *serial, Stream *serialDebug);
	
	void setDebug(bool state);

	void debugPrintln(String str);
	void debugPrint(String str);

	void print(String str);
	void print(unsigned int value);
	void print(int value);
	void print(byte value);
	void print(unsigned long value);
	void print(char *ch);
	void print(bool st);

	void println(String str);
	void println(unsigned int value);
	void println(int value);
	void println(byte value);
	void println(unsigned long value);
	void println(char *ch);
	void println(bool st);
	
	bool enter();
	bool setDHCP();
	bool setWlanJoin();
	bool setWlanAuth(int value);
	bool setWlanKey(String password);
	bool setWlanPassword(String password);
	bool join(String ssid);
	bool setTCPAutoconn();
	bool setTCPIdle();
	
	String readTCP();
	
	bool checkConnection();
	
	bool isWifiConnected();
	bool isTCPOpened();
	
	unsigned int isTCPAvailable();
	
	String getIP();
	String getPort();
	String getNetmask();
	String getGateway();
	
	private:	
	
	String readData();
	String readData(unsigned long timeout);
	
	bool isConnected;
	bool isTcpOpen;
	bool isDebug;
	
	String ip;
	String port;
	String netmask;
	String gateway;
	
	Stream *serial;
	Stream *serialDebug;
};

#endif

