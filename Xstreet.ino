/*******************************************************************
 Blynk lets you create beautiful drag-and-drop visual interfaces
 for your projects in 5 minutes. And it works with almost every
 hardware out there.

 Docs, Tutorials, everything:  http://www.blynk.cc
 Github:                       http://github.com/blynkkk
 Blynk Community:              http://community.blynk.cc
 Follow Us:                    http://www.facebook.com/blynkapp
                               http://twitter.com/blynk_app

 Blynk library is licensed under MIT license
 This example code is in public domain.

*********************************************************************

 You’ll need, in general:
  - Blynk App (download from AppStore or Google Play)
  - Arduino UNO or similar microcontroller board
  - Decide how to connect Arduino to the Internet (USB, Ethernet,
    Wi-Fi, etc.). Bluetooth is on the way.

 There is a bunch of great example sketches included to show you how to get
 started. Think of them as LEGO bricks  and combine them as you wish.
 For example, take the Ethernet Shield sketch and combine it with the
 Servo example, or choose a USB sketch and add a code from SendData
 example.

*********************************************************************

 Let’s turn ON your LED with Blynk!
 In this example we'll use Arduino UNO + Ethernet Shield

 5 Steps guide:
 (watch video tutorial here: )

  1. Connect LED to Pin 5
  ( http://arduino.cc/en/uploads/Tutorial/simplefade_bb.png )

 In the Blynk App:
  2. Create New Project
  3. Email yourself Auth Token. You can do it later at any time
  4. Add a Button Widget. Select Pin D9 in Widget's Settings
  5. Press Play icon. Enjoy Blynking!

********************************************************************/

#define BLYNK_PRINT Serial // Enables Serial Monitor
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h> // This part is for Ethernet stuff
#include <DHT.h>


#define DHTPIN 4  
#define DHTTYPE DHT11
#define enc1 17 //A3 
#define enc2 18 //A4 
#define enc3 19 //A5 

DHT dht(DHTPIN, DHTTYPE);
char auth[] = "62c0bfe90dbd41e1b29fd75f75172f79"; // Put your Auth Token here. (see Step 3 above)

int mot_log[32]={1,0,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,0,1,1,0,0,1};
int mot=1, mot_old, mot_pin[4]={6,7,8,9}, V1_but, V2_but, t_xstr, vlag_xstr;
int count3, new_reg, old_reg, knop, knop_count, punkt, punkt_s, znachenie;
int menu_vozvrat, pause_knop;
long millis_old, millis_count;
float h, t;

WidgetLCD lcd(V0);
WidgetLED led1(V4);
WidgetLED led2(V5);

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);  // Here your Arduino connects to the Blynk Cloud.
  for (int i=0; i<4; i++) {pinMode(mot_pin[i], OUTPUT);}
  pinMode(enc1, INPUT);
  pinMode(enc2, INPUT);
  pinMode(enc3, INPUT);
  dht.begin();
}
/******************Chtenie temp, vlagnost******************/
   void temp_read() {
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
   h = dht.readHumidity();
   t = dht.readTemperature();  
 }
/*********************Function SHTOR******************/
 void move_mot () { if (V1_but==1 || V2_but==1 ){
   if (V1_but==1)mot++; else {if (V2_but==1)mot--;}   
    millis_old=millis();
    if (mot==9)  mot=1;
    if (mot<1)   mot=8;
    for (int i=0; i<4; i++) {
    digitalWrite(mot_pin[i], mot_log[mot*4-i-1]); } } 
  else stop_mot (); }
    
 void stop_mot () {   
    for (int i=0; i<4; i++) {
    digitalWrite(mot_pin[i], 0);  }  }

void encoder () {
   new_reg = digitalRead(enc1)+2*digitalRead(enc2);
   knop = digitalRead(enc3);  
     if (punkt==5) punkt=1;    
     if (punkt==0) punkt=4;   
   if (new_reg!=old_reg) { millis_count=millis();
       
          if (old_reg==0) {if(new_reg == 1) punkt_s += 1;  if(new_reg == 2) punkt_s -= 1; } 
          if (old_reg==1) {if(new_reg == 3) punkt_s += 1;  if(new_reg == 0) punkt_s -= 1; }
          if (old_reg==3) {if(new_reg == 2) punkt_s += 1;  if(new_reg == 1) punkt_s -= 1; } 
          if (old_reg==2) {if(new_reg == 0) punkt_s += 1;  if(new_reg == 3) punkt_s -= 1; } 
          if   (punkt_s==8) {punkt_s=4; znachenie=1;  count3++;}  
          if  (punkt_s==0) {punkt_s=4; znachenie=-1;  count3--;}  
        
 }  old_reg=new_reg; 
 if (knop==0 ) {knop_count++;}
 else {knop_count=0; if(millis()>millis_count+1000) {millis_count=millis(); menu_vozvrat++;}}
 if (knop_count>3000 & pause_knop==0) {punkt++; pause_knop=2000; knop_count=0;} 
 if (pause_knop>0) pause_knop--;
 if (menu_vozvrat>5) {menu_vozvrat=0; punkt=1;}
 }    
 
   

void leds () { 
  if (V1_but==1) led1.on(); else led1.off();
  if (V2_but==1) led2.on(); else led2.off(); }
  
void lcd_xstr () { lcd.clear();
lcd.print(0, 0, "Temp="); lcd.print(6, 0, t);  
lcd.print(0, 1, "Vlagnost="); lcd.print(10, 1, h);  }
  
    
BLYNK_WRITE(V1) { V1_but= param.asInt(); }
BLYNK_WRITE(V2) { V2_but= param.asInt(); }

void loop()
{
  Blynk.run(); // All the Blynk Magic happens here...
  new_reg = digitalRead(enc1)+2*digitalRead(12);
  if (millis()%1000==0){lcd_xstr ();}
  if (millis()%10000==0){temp_read(); }
  
 if (millis()%10==0){
   if (V1_but==1 || V2_but==1 ) leds ();  else  leds ();}
 if (millis()%2==0)move_mot(); 
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

