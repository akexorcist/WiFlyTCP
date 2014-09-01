/*
 * Copyright (c) 2014 Akexorcist
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include <WiFlyTCP.h>                                // Include WiFly library

WiFlyTCP wifly;                                      // Declare WiFly instance

String ssid = "YOUR_SSID";                    // Your WiFi SSID
String pass = "YOUR_PASS";                            // Your WiFi password

void setup()
{    
    Serial.begin(9600);                              // Set baudrate of Serial 1
    Serial1.begin(9600);                             // Set baudrate of USB serial 
    
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
        
        while(wifly.isWifiConnected())               // WiFi was connected
        {
            delay(10);                               // Delay 10 millisenconds
                        
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
                  
                  //*******************//
                  // Do Something Here
                  //*******************//
                  
                  str = "";                          //  Clear string 
                }
            }          
        }      
        wifly.debugPrintln("Close");                 // When WiFi was disconnected
    } 
    delay(1000);                                     // Delay 1 second
}
