/*
  WiFlyTCP.h - WiFly Library for Arduino
*/


#include "WiFlyTCP.h"

/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
WiFlyTCP::WiFlyTCP() {

}

void WiFlyTCP::begin(Stream *serial, Stream *serialDebug)
{
	this->serial = serial;
	this->serialDebug = serialDebug;
	this->isConnected = false;
	this->isTcpOpen = false;
	this->port = "2000";
	this->ip = NULL;
	this->netmask = NULL;
	this->gateway = NULL;
	this->isDebug = false;
	//debug.begin(serialport);
	//this->baudrate = baudrate;      		// which step the motor is on
	//this->serialport = serialport;
  //this->serialport.begin(serialport);
}

void WiFlyTCP::print(String str) {
	this->serial->print(str);
}

void WiFlyTCP::print(unsigned int value) {
	print((String)value);
}

void WiFlyTCP::print(int value) {
	print((String)value);
}

void WiFlyTCP::print(byte value) {
	print((String)value);
}

void WiFlyTCP::print(unsigned long value) {
	print((String)value);
}

void WiFlyTCP::print(char *ch) {
	println(String(ch));
}

void WiFlyTCP::print(bool st) {
	print((String)st);
}

void WiFlyTCP::println(String str) {
	this->serial->println(str);
}

void WiFlyTCP::println(unsigned int value) {
	println((String)value);
}

void WiFlyTCP::println(int value) {
	println((String)value);
}

void WiFlyTCP::println(byte value) {
	println((String)value);
}

void WiFlyTCP::println(unsigned long value) {
	println((String)value);
}

void WiFlyTCP::println(char *ch) {
	println(String(ch));
}

void WiFlyTCP::println(bool st) {
	println((String)st);
}

void WiFlyTCP::setDebug(bool state) {
	this->isDebug = state;
}

void WiFlyTCP::debugPrintln(String str) 
{
	if(this->isDebug)
		serialDebug->println(str);
}

void WiFlyTCP::debugPrint(String str) 
{
	serialDebug->print(str);
}

bool WiFlyTCP::enter() {
    debugPrintln("$$$");
    serial->write("$$$");
    bool result = readData(1000).equals("CMD");
	if(result) {
		return result;
	} 
	serial->println("$$$");
    readData();
    readData();
    readData();
	return result;
}

bool WiFlyTCP::setDHCP() {
    String str = "set ip dhcp 1";
    serial->println(str);
    readData();
    bool result = readData().equals("AOK");
	readData();
	return result;
}

bool WiFlyTCP::setWlanJoin() {
    String str = "set wlan join 0";
    serial->println(str);
    readData();
    bool result = readData().equals("AOK");
	readData();
	return result;
}

bool WiFlyTCP::setWlanAuth(int value) {
    String str = "set wlan auth " + String(value);
    serial->println(str);
    readData();
    bool result = readData().equals("AOK");
	readData();
	return result;
}

bool WiFlyTCP::setWlanPassword(String password) {
    String str = "set wlan phrase " + String(password);
    serial->println(str);
    readData();
    bool result = readData().equals("AOK");
    readData();
	return result;
}

bool WiFlyTCP::setWlanKey(String password) {
    String str = "set wlan key " + String(password);
    serial->println(str);
    readData();
    bool result = readData().equals("AOK");
    readData();
	return result;
}

bool WiFlyTCP::join(String ssid) {
	enter();
	debugPrintln("Join");
    String str = "join " + ssid;
    serial->println(str);
    readData();
    readData();
    readData();
	
    if(readData(3000).equals("timeout")) {
		return false;	
	}
	
    if(readData(5000).equals("Associated!")) {
		this->isConnected = true;
        readData();
        readData();
        readData();
        readData();
		
		String raw = readData();
        this->ip = raw.substring(3, raw.length() - 5); // IP
		
		raw = readData();
        this->netmask = raw.substring(3, raw.length()); // Netmask
		
		raw = readData();
        this->gateway = raw.substring(3, raw.length()); // Gateway
		
        readData(500); // Listener
        return true;
    }
    return false;
}

bool WiFlyTCP::setTCPAutoconn() {
    String str = "set sys autoconn 1";
    serial->println(str);
    readData();
    readData();
    bool result = readData().equals("AOK");
    //readData();
	return result;
	//return false;
}

bool WiFlyTCP::setTCPIdle() {
    String str = "set comm idle 0";
    serial->println(str);
    readData();
    readData();
    bool result = readData().equals("AOK");
    //readData();
	return result;
	//return false;
}

bool WiFlyTCP::isWifiConnected() {
	return this->isConnected;
}

bool WiFlyTCP::isTCPOpened() {
	return this->isTcpOpen;
}

unsigned int WiFlyTCP::isTCPAvailable() {
	return serial->available();
}

String WiFlyTCP::getIP() {
	return this->ip;
}

String WiFlyTCP::getPort() {
	return this->port;
}

String WiFlyTCP::getNetmask() {
	return this->netmask;
}

String WiFlyTCP::getGateway() {
	return this->gateway;
}

bool WiFlyTCP::checkConnection() {
	bool status = readData(300).substring(0, 7).equals("Disconn");
	if(status) {
		readData(1000);
		readData(1000);
	}
	isConnected = !status;
	return !status;
}

String WiFlyTCP::readData() {
    String data = "";
    while(true) {
        if(serial->available() > 0) { 
            char r = serial->read();
            if (r == '\n') {
                debugPrintln(data);
                return data;
            } else if(r == '\r') {
            } else {
                data += r;  
            }
        }
    }
}

String WiFlyTCP::readData(unsigned long timeout) {
    String data = "";
	unsigned long time = millis();
    while(millis() - time < timeout) {
        if(serial->available() > 0) { 
            char r = serial->read();
            if (r == '\n') {
                debugPrintln(data);
                return data;
            } else if(r == '\r') {
            } else {
                data += r;  
            }
        }
    }
    debugPrintln("Timeout");
    return "timeout";
}

String WiFlyTCP::readTCP() {
    char r = serial->read();
	if(r == '*') {
		String data = "";
		while(true) {
			if(serial->available() > 0) { 
				char r1 = serial->read();
				if(r1 == '*') {
					break;
				} else {
					data += r1;  
				}
			}
		}
		
		debugPrintln(data);
		if(data.equals("OPEN")) {
			this->isTcpOpen = true;
		} else if(data.equals("CLOS")) {
			this->isTcpOpen = false;
		}		
		return "";
	} else if(r == 'D') {
		String data = "D";
		while(true) {
			if(serial->available() > 0) { 
				char r1 = serial->read();
				data += r1;
			} else {
				if(data.length() >= 7) {
					String s = data.substring(0, 7);
					if(s.equals("Disconn")) {
						this->isConnected = false;
						this->isTcpOpen = false;
						readData(500);
						readData(500);
						readData(8000);
						return "";
					} else {
						return data;
					}
				}
			}
		}
	} else {
		return (String)r; 
	}
	return "";
}

