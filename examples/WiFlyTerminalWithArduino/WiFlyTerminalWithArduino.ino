/*
 * Copyright (c) 2013 Akexorcist
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
