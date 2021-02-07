// 24_Channel_Smart_Switch_No_Wi-Fi_modification_(Manual_mode)
// © Copyright Maxim Bortnikov 2021
// For more information and for the schematics of this device visit: https://github.com/Northstrix/24_Channel_Smart_Switch
#include <ESP8266WiFi.h>             // Include ESP8266WiFi library
#include <Servo.h>                  // Include Servo library
#include <SPI.h>                   // Include SPI library
#include <Wire.h>                 // Include Wire library
#include <Adafruit_GFX.h>        // Include Adafruit_GFX library
#include <Adafruit_SSD1306.h>   // Include Adafruit_SSD1306 library
#define OLED_RESET 16
#define debounce_time 175
Adafruit_SSD1306 display(OLED_RESET);
int latchPin = 12; //D6
int latchPin1 = 13; //D7
int latchPin2 = 15; //D8
int clockPin = 16; //D0
int dataPin = 14; //D5
int chan = 1;
bool firstlatchon = 1;
bool firstlatchoff = 0;
bool secondlatchon = 1;
bool secondlatchoff = 0;
bool thirdlatchon = 1;
bool thirdlatchoff =0;
// It'll be ...on = 1; ...off = 0 for SN74HC373,KM555TM7,etc
byte curcomb;
byte combinations[256]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,
0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,
0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,
0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,
0x5C,0x5D,0x5E,0x5F,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,0x71,0x72,0x73,
0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,
0x8C,0x8D,0x8E,0x8F,0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,0xA0,0xA1,0xA2,0xA3,
0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,
0xBC,0xBD,0xBE,0xBF,0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,0xD0,0xD1,0xD2,0xD3,
0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,
0xEC,0xED,0xEE,0xEF,0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF};

  int x,b,c,d,e,f,g,h; // ROOM N1
  int k,l,m,n,o,p,q,r; // ROOM N2
  int s,t,u,v,w,y,z,last; // ROOM N3
  void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
    int i=0;
    int pinState;
    pinMode(myClockPin, OUTPUT);
    pinMode(myDataPin, OUTPUT);
    digitalWrite(myDataPin, 0);
    digitalWrite(myClockPin, 0);
    for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);
    if ( myDataOut & (1<<i) ) {
    pinState= 1;
    }
    else {
    pinState= 0;
    }
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
    }
    digitalWrite(myClockPin, 0);
  }
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(12,20);
  display.println("Manual mode activated");
  display.display();  
  pinMode(latchPin, OUTPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(0,INPUT);
  pinMode(2,INPUT);
  digitalWrite(latchPin, firstlatchon);
  digitalWrite(latchPin1, secondlatchon);
  digitalWrite(latchPin2, thirdlatchon);
  shiftOut(dataPin, clockPin, 0x00);
  digitalWrite(latchPin, firstlatchoff);
  digitalWrite(latchPin1, secondlatchoff);
  digitalWrite(latchPin2, thirdlatchoff);
}


void loop() {
   if (digitalRead(0) == LOW && digitalRead(2) == HIGH){
  delay(debounce_time); // debounce
  chan++;
  }
  if (digitalRead(2) == LOW && digitalRead(0) == HIGH){
    delay(debounce_time); // debounce
    chan--;
  }
  if (chan>24) chan = 1;
  if (chan<1) chan = 24;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Manual mode activated");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,22);
  display.println("Channel");
  display.setCursor(88,22);
  display.println(chan);
  display.display(); 
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 1 && x == 0)
  {
    x = 1;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 1 && x == 1)
  {
    x = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 2 && b == 0)
  {
    b = 2;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 2 && b == 2)
  {
    b = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 3 && c == 0)
  {
    c = 4;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 3 && c == 4)
  {
    c = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 4 && d == 0)
  {
    d = 8;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 4 && d == 8)
  {
    d = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 5 && e == 0)
  {
    e = 16;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 5 && e == 16)
  {
    e = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 6 && f == 0)
  {
    f = 32;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 6 && f == 32)
  {
    f = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 7 && g == 0)
  {
    g = 64;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 7 && g == 64)
  {
    g = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 8 && h == 0)
  {
    h = 128;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 8 && h == 128)
  {
    h = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 9 && k == 0)
  {
    k = 1;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 9 && k == 1)
  {
    k = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 10 && l == 0)
  {
    l = 2;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 10 && l == 2)
  {
    l = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 11 && m == 0)
  {
    m = 4;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 11 && m == 4)
  {
    m = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 12 && n == 0)
  {
    n = 8;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 12 && n == 8)
  {
    n = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 13 && o == 0)
  {
    o = 16;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 13 && o == 16)
  {
    o = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 14 && p == 0)
  {
    p = 32;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 14 && p == 32)
  {
    p = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 15 && q == 0)
  {
    q = 64;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 15 && q == 64)
  {
    q = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 16 && r == 0)
  {
    r = 128;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 16 && r == 128)
  {
    r = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 17 && s == 0)
  {
    s = 1;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 17 && s == 1)
  {
    s = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 18 && t == 0)
  {
    t = 2;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 18 && t == 2)
  {
    t = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 19 && u == 0)
  {
    u = 4;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 19 && u == 4)
  {
    u = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 20 && v == 0)
  {
    v = 8;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 20 && v == 8)
  {
    v = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 21 && w == 0)
  {
    w = 16;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 21 && w == 16)
  {
    w = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 22 && y == 0)
  {
    y = 32;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 22 && y == 32)
  {
    y = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 23 && z == 0)
  {
    z = 64;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 23 && z == 64)
  {
    z = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }
    if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 24 && last == 0)
  {
    last = 128;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is on");
    display.display(); 
  }
  if(digitalRead(0) == LOW && digitalRead(2) == LOW && chan == 24 && last == 128)
  {
    last = 0;
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,42);
    display.println("is off");
    display.display(); 
  }

  curcomb = combinations[x+b+c+d+e+f+g+h];
  digitalWrite(latchPin, firstlatchoff);
  digitalWrite(latchPin1, secondlatchoff);
  digitalWrite(latchPin2, thirdlatchoff);
  shiftOut(dataPin, clockPin, curcomb);
  digitalWrite(latchPin, firstlatchon);
  delayMicroseconds(1);
  digitalWrite(latchPin, firstlatchoff);
  curcomb = combinations[k+l+m+n+o+p+q+r];
  shiftOut(dataPin, clockPin, curcomb);
  digitalWrite(latchPin1, secondlatchon);
  delayMicroseconds(1);
  digitalWrite(latchPin1, secondlatchoff);
  curcomb = combinations[s+t+u+v+w+y+z+last];
  shiftOut(dataPin, clockPin, curcomb);
  digitalWrite(latchPin2, thirdlatchon);
  delayMicroseconds(1);
  digitalWrite(latchPin2, thirdlatchoff);

}
