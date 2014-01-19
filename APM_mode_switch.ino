/*

  APM Mode Switch - Set flight mode for ArduPilot Mega autopilots via
  an RC transmitter's PPM (trainer) input
  
  Version 1.0

  (C) 2013 Stefan Gofferje - http://stefan.gofferje.net/

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Usage:
  Connect the PPM out of the board to the PPM in / trainer port of your
  transmitter, as well as ground from the board to ground on the PPM /
  trainer port of your transmitter. In mixes, assign trainer / PPM
  channel 1 to the respective TX channel (e.g. channel 5 for APM:Copter).
  
*/

#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t Dingbats1_XL[];

// Remember to change the model parameter to suit your display module!
UTFT          myGLCD(ITDB32S,38,39,40,41);
UTouch        myTouch(6,5,4,3,2);

// Finally we set up UTFT_Buttons :)
UTFT_Buttons  myButtons(&myGLCD, &myTouch);
UTFT_Buttons  myLargeButtons(&myGLCD, &myTouch);


// Basic config
#define numchan 8          // number of chanels in PPM stream
#define servo_default 1500 // default servo value (1500 = center)
#define PPM_FrLen 22500    // PPM frame length in microseconds
#define PPM_PulseLen 300   // PPM pulse length
#define PPM_Polarity 1     // PPM polarity -  0 = neg, 1 = pos
#define PPM_outPin 10      // PPM output pin

// I/O-Pins
// Button 0 & LED 0 are pre-wired, I think
// LED are wired to D2-D7
// Busttons are wired from D11, D12, A0 (14), A1 (15), A2 (16), A3 (17)


const byte LED[]={0,2,3,4,5,6,7};
const byte BUTTON[]={0,17,16,15,14,12,11};



// Channel PWM values
int ppm[numchan];

void setup()
{
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  
  myButtons.setTextFont(SmallFont);
  myButtons.setSymbolFont(Dingbats1_XL);

    myLargeButtons.setTextFont(BigFont);
  myLargeButtons.setSymbolFont(Dingbats1_XL);

}

void loop()
{
  int but1, but2, but3, but4, but5, but6, but7, but8, but9, pressed_button;
  boolean default_colors = true;
  
  but1 = myButtons.addButton( 10,  10, 90,  60, "Stabilize");
  but2 = myButtons.addButton( 115,  10, 90,  60, "ALT HOLD");
  but3 = myButtons.addButton( 220,  10, 90,  60, "LOITER");
  
  but4 = myLargeButtons.addButton( 10,  85, 90,  60, "RTL");
  but5 = myLargeButtons.addButton( 115,  85, 90,  60, "LAND");
  but6 = myButtons.addButton( 220,  85, 90,  60, "MANUAL");

  but7 = myButtons.addButton( 10,  160, 90,  60, "CRUISE");
  but8 = myButtons.addButton( 115,  160, 90,  60, "CIRCLE");
  but9 = myLargeButtons.addButton( 220,  160, 90,  60, "AUTO");

  /*
  */

  /*
  but2 = myButtons.addButton( 10,  60, 300,  30, "Button 2");
  but3 = myButtons.addButton( 10, 100, 300,  30, "Button 3");
  */

  myButtons.drawButtons();
  myLargeButtons.drawButtons();

    myButtons.setButtonColors(VGA_WHITE,VGA_GRAY,VGA_WHITE, VGA_BLUE,VGA_RED);
    myLargeButtons.setButtonColors(VGA_WHITE,VGA_GRAY,VGA_WHITE, VGA_BLUE,VGA_RED);

  myButtons.drawButton(but1);
  myButtons.drawButton(but3);
  myLargeButtons.drawButton(but5);
  myButtons.drawButton(but7);
  myLargeButtons.drawButton(but9);

  myGLCD.print("You pressed:", 40, 228);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setBackColor(VGA_WHITE);
  myGLCD.print("None    ", 140, 228);

  while(1) 
  {
    if (myTouch.dataAvailable() == true)
    {
      pressed_button = myButtons.checkButtons();

      if (pressed_button==but1)
        myGLCD.print("Button 1", 140, 228);
      if (pressed_button==but2)
        myGLCD.print("Button 2", 140, 228);
      if (pressed_button==but3)
        myGLCD.print("Button 3", 140, 228);
      if (pressed_button==-1)
        myGLCD.print("None    ", 140, 228);
    }
  }
}


// Petit a Petit on deviens moins petit

void setup2()
{


/*

  Serial.begin(9600);

  // for each of the 7 LED pin, set up for output, light up the LED, wait 100ms, turn off the LED.
  // this should result in each LED lighting up briefly then turning off.


  for (int i=0;i<=6;i++) {

    pinMode(LED[i],OUTPUT);  // set ups the LED pins for output.
    digitalWrite(LED[i],HIGH); // lights up the LED
    delay(100); // wait
    digitalWrite(LED[i],LOW); // turns off the LED
  }

  // each of the 7 buttons pin is set for input then set to HIGH meaning at 5 volts


  for (int i=0;i<=6;i++) {
    pinMode(BUTTON[i],INPUT); 
    digitalWrite(BUTTON[i],HIGH);
  }

  // All LED are turned on, off, on, off all at the same time

  setled(-1); // Turn on all LED
  delay(50); // wait 
  setled(0); // Turn off all LED
  delay(100); // wait
  setled(-1); // Turn on all LED
  delay(50); // wait
  setled(0); // Turn off all LED
   

  // 2000 is mode 6 apparently
  ppm[0]=2000; // We start in Mode 6
  
  // All others to center
  // All others (1-7) are set to 1500
  for(int i=1; i<numchan; i++){
    ppm[i]= servo_default;
  }

  // configure the PPM out pin to OUTPUT mode
  pinMode(PPM_outPin, OUTPUT);

  // write NEGATIVE to the PPM pin
  digitalWrite(PPM_outPin, !PPM_Polarity);
  
  // Set up timer interrupt
  cli(); // no interrupt
  TCCR1A = 0; 
  TCCR1B = 0;
  OCR1A = 100;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11);
  TIMSK1 |= (1 << OCIE1A);
  sei(); // interrupts enabled
  */

}

/* setLED (nr)

0 => All LED are shut off
-1 => All LED are turned on
Anything else turn off all LED but for [nr]

*/

void setled(int nr) {
  if (nr==0) {
    for (int i=0;i<=6;i++) {
      digitalWrite(LED[i],LOW);
    }
  }
  else if (nr==-1) {
    for (int i=0;i<=6;i++) {
      digitalWrite(LED[i],HIGH);
    }
  }
  else {
    for (int i=0;i<=6;i++) {
      digitalWrite(LED[i],LOW);
    }
    digitalWrite(LED[nr],HIGH);
  }
}  

// Petit 2

void loop2() {


/*
  // If Button 1 is pressed, set the PPM Value of channel 0 to 1000

  if (digitalRead(BUTTON[1])==LOW) ppm[0]=1000;
  else if (digitalRead(BUTTON[2])==LOW) ppm[0]=1296;
  else if (digitalRead(BUTTON[3])==LOW) ppm[0]=1426;
  else if (digitalRead(BUTTON[4])==LOW) ppm[0]=1556;
  else if (digitalRead(BUTTON[5])==LOW) ppm[0]=1686;
  else if (digitalRead(BUTTON[6])==LOW) ppm[0]=2000;

  // If the PPM channel 0 is between 0 and 1230, turn on LED 1
  // If the PPM channel 0 is 1231 to 1360, turn on LED 2
  // Button 6 -> Turn on LED 6

  
  if ((ppm[0] > 0) && (ppm[0] < 1230)) setled(1);
  else if ((ppm[0] > 1231) && (ppm[0] < 1360)) setled(2);
  else if ((ppm[0] > 1361) && (ppm[0] < 1490)) setled(3);
  else if ((ppm[0] > 1491) && (ppm[0] < 1620)) setled(4);
  else if ((ppm[0] > 1621) && (ppm[0] < 1750)) setled(5);
  else if (ppm[0] > 1750) setled(6);

  // Print value of PPM channel 0 
  Serial.println(ppm[0]);
*/
}

ISR(TIMER1_COMPA_vect){
  static boolean state = true;
  
  TCNT1 = 0;
  
  if(state) {
    digitalWrite(PPM_outPin, PPM_Polarity);
    OCR1A = PPM_PulseLen * 2;
    state = false;
  }
  else{
    static byte channel;
    static unsigned int rest;
  
    digitalWrite(PPM_outPin, !PPM_Polarity);
    state = true;

    if(channel >= numchan){
      channel = 0;
      rest = rest + PPM_PulseLen;
      OCR1A = (PPM_FrLen - rest) * 2;
      rest = 0;
    }
    else{
      OCR1A = (ppm[channel] - PPM_PulseLen) * 2;
      rest = rest + ppm[channel];
      channel++;
    }     
  }
}
