// Commumicate with WiFly RN-XV by Arduino board

/* 
 * Connect WiFly to UART (RX TX) on Arduino 
 * and use terminal program like as TeraTerm
 * to send command to WiFly through Arduino
 */

void setup()
{
  Serial.begin(9600);             // Serial for PC
  Serial1.begin(9600);            // Serial for WiFly
}

void loop()
{
  if(Serial.available() > 0) {    // Data has sent from PC 
    char c = Serial.read();       // Sent it to WiFly
    Serial1.write(c);
  }
  
  if(Serial1.available() > 0) {   // Data has sent from WiFly
    char c = Serial1.read();      // Sent it to PC
    Serial.write(c);
  }
}
