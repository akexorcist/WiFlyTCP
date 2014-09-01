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

/* 
 * Control POPBOT-XT via WiFi
 */

#include <popxt.h>                                   // Include POPBOT-XT library 
#include <WiFlyTCP.h>                                // Include WiFly library

WiFlyTCP wifly;                                      // Declare WiFly instance

String ssid = "YOUR_SSID";                    // Your WiFi SSID
String pass = "YOUR_PASS";                            // Your WiFi password

boolean conn = false;                                // Declare boolean variable for TCP connection state

void setup()
{
    delay(2000);                                     // Delay 2 seconds
    glcdClear();                                     // Clear GLCD screen
    
    Serial.begin(9600);                              // Set baudrate of Serial 1
    Serial1.begin(9600);                             // Set baudrate of USB serial 
    
    glcdString(0, 0, "Initial");                     // Show "Initial" on GLCD
    
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
    glcd(0, 0, "Connecting...");                     // Show "Initial" on GLCD
    
    char buf[ssid.length() + 1];                     // Declare char array for SSID
    ssid.toCharArray(buf, ssid.length() + 1);        // Convert string of SSID to char array
    glcdString(0, 1, buf);                           // Show SSID on GLCD
    
    if(wifly.join(ssid)) {                           // Connect to target WiFi
        glcd(0, 0, "Connected    ");                 // Show "Connected" on GLCD
        
        String ip = wifly.getIP();                   // Get ip
        String port = wifly.getPort();               // Get port
        String address = ip + ":" + port;            // Merge ip and port into address
        
        char buf[address.length() + 1];                  // Declare char array for ip address
        address.toCharArray(buf, address.length() + 1);  // Convert string of ip address to char array
        glcdString(0, 2, buf);                           // Show ip address on GLCD
   
        glcd(4, 0, "CLOSE");                          // Show "CLOSE" on GLCD that mean TCP not yet connected
        
        String str = "";                              // Declare string for incoming message
        String s = "";                                // Declare string for each incoming message
        
        while(wifly.isWifiConnected()) {              // Check WiFi connection state
            delay(10);                                // Delay 10 milliseconds
            
            if(!conn && wifly.isTCPOpened()) {        // If TCP isn't connected state and TCP connection was started
              glcd(4, 0, "OPEN ");                    // Show "OPEN" on GLCD
              conn = true;                            // Change connection state to true
            } else if(conn && !wifly.isTCPOpened()) { // But if TCP is connected state and TCP connection was closed
              glcd(4, 0, "CLOSE");                    // Show "CLOSE" on GLCD
              conn = false;                           // Change connection state to false
            }
            
            if(wifly.isTCPAvailable() > 0){           // Check incoming message on TCP
                                                      // I habe
            
                s = wifly.readTCP();                  // Read message into last string
                                                      // In this example, I sent a message which end with ";" 
                                                      // Example : "Hello;"  "Speed100;"  "ON;"
                                                      
                if(!s.equals(";")) {                            // If message isn't ";"
                    str += s;                                   // Add last string into main string
                } else {                                        // But if message is ";"
                    if(str.charAt(0) == 'F') {                  // First string is "F"
                        str = str.substring(1, str.length());   // Remove first string
                        int spd = str.toInt();                  // Get speed from remain string
                        glcd(5, 0, "Forward:%d    ", spd);      // Show "Forward:Speed" on GLCD
                        fd(spd);                                // Moving forward with speed 
                    } else if(str.charAt(0) == 'B') {
                        str = str.substring(1, str.length());
                        int spd = str.toInt();
                        glcd(5, 0, "Backward:%d   ", spd);
                        bk(spd);
                    } else if(str.charAt(0) == 'L') {
                        str = str.substring(1, str.length());
                        int spd = str.toInt();
                        glcd(5, 0, "TurnLeft:%d   ", spd);
                        sl(spd);
                    } else if(str.charAt(0) == 'R') {
                        str = str.substring(1, str.length());
                        int spd = str.toInt();
                        glcd(5, 0, "TurnRight:%d  ", spd);
                        sr(spd);
                    } else if(str.charAt(0) == 'S') {
                        str = str.substring(1, str.length());
                        int spd = str.toInt();
                        glcd(5, 0, "Stop          ", spd);
                        ao();
                    } 
                  
                    str = "";                                    // Clear string
                }
            }
        }
        
        // TCP was closed
        
        glcd(0, 0, "Disconnected");                              // Show "Disconnected" on GLCD
    }
}
