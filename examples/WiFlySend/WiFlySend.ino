#include <WiFlyTCP.h>                                // Include WiFly library

#define AN          A0                               // Analog read pin                 
#define DI          13                               // Digital read pin
#define INTERVAL    1000                             // Interval time to sent data

WiFlyTCP wifly;                                      // Declare WiFly instance

String ssid = "Your_SSID";                    // Your WiFi SSID
String pass = "Your_Pass";                            // Your WiFi password

void setup()
{    
    Serial.begin(9600);                              // Set baudrate of Serial 1
    Serial1.begin(9600);                             // Set baudrate of USB serial 
    
    pinMode(AN, INPUT);                             // Set analog pin to input mode
    pinMode(DI, INPUT);                             // Set digital pin to input mode
    
    wifly.begin(&Serial1, &Serial);                  // Set serial port which connect to wifly
                                                     // and serial port for debugging
                                                     // Example : Serial 1 (WiFly), Serial USB (Debugging)    

    wifly.setDebug(true);                            // Set debugging to debug serial
    wifly.enter();                                   // Enter command mode
    wifly.setDHCP();                                 // Setting for WiFi
    wifly.setWlanJoin();                             // Setting for WiFi 
    wifly.setWlanAuth(WIFLY_WLAN_AUTO_WPA1_WPA2);    // WiFi authentication type
    wifly.setWlanPassword(pass);                     // Password of WiFi
    wifly.setTCPAutoconn();                          // Setting for TCP
    wifly.setTCPIdle();                              // Setting for TCP
}

void loop()
{
    if(wifly.join(ssid))                             // Connect to target SSID
    {
        wifly.debugPrintln(wifly.getIP());           // Print IP
        wifly.debugPrintln(wifly.getPort());         // Print port
        wifly.debugPrintln(wifly.getNetmask());      // Print Netmask
        wifly.debugPrintln(wifly.getGateway());      // Print gateway
        
        String str = "";                             // This is main string
        String s = "";                               // This is temporary string
        unsigned long time = 0;                      // Time variable
        
        while(wifly.isWifiConnected())               // WiFi was connected
        {
            delay(10);                               // Delay 10 millisenconds
            
            if(wifly.isTCPOpened() && millis() - time > INTERVAL) {      // Do every "INTERVAL" (1 second)
              time = millis();                                           // Reset time variable
              char tmp[10];                                              // Initial char array
              sprintf(tmp, "A%d:D%d;", analogRead(AN), digitalRead(DI)); // Create message for send to target device
              wifly.println(tmp);                                        // Sent message
            }
                        
            if(wifly.isTCPAvailable() > 0)           // Any message has sent from some device via TCP protocol
            {
                s = wifly.readTCP();
                if(!s.equals(";"))                   // In this example, I sent a message which end with ";" 
                {                                    // Example : "Hello;"  "Speed100;"  "ON;"
                  str += s;                          // Add last string into main string
                } 
                else 
                { 
                  wifly.debugPrintln(str);           // Sent message to debug serial
                  
                  //*******************************************//
                  // Do Something Here when message has sent
                  //*******************************************//
                  
                  str = "";                          //  Clear string 
                }
            }          
        }      
        wifly.debugPrintln("Close");                 // When WiFi was disconnected
    } 
    delay(1000);                                     // Delay 1 second
}
