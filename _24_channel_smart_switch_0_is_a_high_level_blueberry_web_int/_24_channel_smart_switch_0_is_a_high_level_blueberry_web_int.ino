// 24_Channel_Smart_Switch_Blueberry_Web_Int
// © Copyright Maxim Bortnikov 2021
// For more information and for the schematics of this device visit:https://github.com/Northstrix/24_Channel_Smart_Switch
#include <ESP8266WiFi.h>             // Include ESP8266WiFi library
#include <Servo.h>                  // Include Servo library
#include <SPI.h>                   // Include SPI library
#include <Wire.h>                 // Include Wire library
#include <Adafruit_GFX.h>        // Include Adafruit_GFX library
#include <Adafruit_SSD1306.h>   // Include Adafruit_SSD1306 library
#define OLED_RESET 16
#define debounce_time 135
Adafruit_SSD1306 display(OLED_RESET);
const char* ssid     = "Mobile Hotspot 1290";  // Your SSID (Network's name)
const char* password = "seasidehills24"; // Password from your network
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

  int x = 1;
  int b = 2;
  int c = 4;
  int d = 8;
  int e = 16;
  int f = 32;
  int g = 64;
  int h = 128;
  // ROOM N1
  int k = 1;
  int l = 2;
  int m = 4;
  int n = 8;
  int o = 16;
  int p = 32;
  int q = 64;
  int r = 128;
  // ROOM N2
  int s = 1;
  int t = 2;
  int u = 4;
  int v = 8;
  int w = 16;
  int y = 32;
  int z = 64;
  int last = 128;
  // ROOM N3
  WiFiServer server(80);
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
void setup(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(12,20);
  display.println("Connecting to the");
  display.setCursor(27,32);
  display.println("Access Point");
  display.display();  
  Serial.begin(115200); // Baudrate
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
  Serial.print("Connecting to the Newtork"); // This all will be displayed in the Serial Monitor
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin(); // Starting the server
  Serial.println("Server started");
  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
  Serial.println("/");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,16);
  display.println("Enter the IP address"); // Display device's IP address on display
  display.setCursor(20,26);
  display.println(WiFi.localIP());
  display.setCursor(24,36);
  display.println("in any browser");
  display.display();
}

 

void loop(){
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available())  
  {
    delay(1);
  } 
  String request = client.readStringUntil('\r');
  Serial.println(request);

 
  if(request.indexOf("/ch1=ON") != -1)
  {
    x = 0;
  }
  if(request.indexOf("/ch1=OFF") != -1)
  {
    x = 1;
  }
    if(request.indexOf("/ch2=ON") != -1)
  {
    b = 0;
  }
  if(request.indexOf("/ch2=OFF") != -1)
  {
    b = 2;
  }
    if(request.indexOf("/ch3=ON") != -1)
  {
    c = 0;
  }
  if(request.indexOf("/ch3=OFF") != -1)
  {
    c = 4;
  }
    if(request.indexOf("/ch4=ON") != -1)
  {
    d = 0;
  }
  if(request.indexOf("/ch4=OFF") != -1)
  {
    d = 8;
  }
    if(request.indexOf("/ch5=ON") != -1)
  {
    e = 0;
  }
  if(request.indexOf("/ch5=OFF") != -1)
  {
    e = 16;
  }
    if(request.indexOf("/ch6=ON") != -1)
  {
    f = 0;
  }
  if(request.indexOf("/ch6=OFF") != -1)
  {
    f = 32;
  }
    if(request.indexOf("/ch7=ON") != -1)
  {
    g = 0;
  }
  if(request.indexOf("/ch7=OFF") != -1)
  {
    g = 64;
  }
    if(request.indexOf("/ch8=ON") != -1)
  {
    h = 0;
  }
  if(request.indexOf("/ch8=OFF") != -1)
  {
    h = 128;
  }
    if(request.indexOf("/ch9=ON") != -1)
  {
    k = 0;
  }
  if(request.indexOf("/ch9=OFF") != -1)
  {
    k = 1;
  }
    if(request.indexOf("/ch10=ON") != -1)
  {
    l = 0;
  }
  if(request.indexOf("/ch10=OFF") != -1)
  {
    l = 2;
  }
    if(request.indexOf("/ch11=ON") != -1)
  {
    m = 0;
  }
  if(request.indexOf("/ch11=OFF") != -1)
  {
    m = 4;
  }
    if(request.indexOf("/ch12=ON") != -1)
  {
    n = 0;
  }
  if(request.indexOf("/ch12=OFF") != -1)
  {
    n = 8;
  }
    if(request.indexOf("/ch13=ON") != -1)
  {
    o = 0;
  }
  if(request.indexOf("/ch13=OFF") != -1)
  {
    o = 16;
  }
    if(request.indexOf("/ch14=ON") != -1)
  {
    p = 0;
  }
  if(request.indexOf("/ch14=OFF") != -1)
  {
    p = 32;
  }
    if(request.indexOf("/ch15=ON") != -1)
  {
    q = 0;
  }
  if(request.indexOf("/ch15=OFF") != -1)
  {
    q = 64;
  }
    if(request.indexOf("/ch16=ON") != -1)
  {
    r = 0;
  }
  if(request.indexOf("/ch16=OFF") != -1)
  {
    r = 128;
  }
    if(request.indexOf("/ch17=ON") != -1)
  {
    s = 0;
  }
  if(request.indexOf("/ch17=OFF") != -1)
  {
    s = 1;
  }
    if(request.indexOf("/ch18=ON") != -1)
  {
    t = 0;
  }
  if(request.indexOf("/ch18=OFF") != -1)
  {
    t = 2;
  }
    if(request.indexOf("/ch19=ON") != -1)
  {
    u = 0;
  }
  if(request.indexOf("/ch19=OFF") != -1)
  {
    u = 4;
  }
    if(request.indexOf("/ch20=ON") != -1)
  {
    v = 0;
  }
  if(request.indexOf("/ch20=OFF") != -1)
  {
    v = 8;
  }
    if(request.indexOf("/ch21=ON") != -1)
  {
    w = 0;
  }
  if(request.indexOf("/ch21=OFF") != -1)
  {
    w = 16;
  }
    if(request.indexOf("/ch22=ON") != -1)
  {
    y = 0;
  }
  if(request.indexOf("/ch22=OFF") != -1)
  {
    y = 32;
  }
    if(request.indexOf("/ch23=ON") != -1)
  {
    z = 0;
  }
  if(request.indexOf("/ch23=OFF") != -1)
  {
    z = 64;
  }
    if(request.indexOf("/ch24=ON") != -1)
  {
    last = 0;
  }
  if(request.indexOf("/ch24=OFF") != -1)
  {
    last = 128;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(12,0);
  display.println("IP:");
  display.setCursor(31,0);
  display.println(WiFi.localIP());
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(6,22);
  display.println("Wi-Fi mode");
  display.setCursor(88,22);
  display.println(chan);
  display.setCursor(10,42);
  display.println("Activated");
  display.display(); 
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

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html"); // All below is a web page code written in HTML
  client.println("");
  client.println("<!DOCTYPE html>\n"
"<html>\n"
" <head>\n"
" <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n"
" <meta charset='utf-8'>\n"
"    <style>\n"
"  * {\n"
"  margin: 0;\n"
"  padding: 0;\n"
"}\n"
"html,\n"
"body {\n"
"  margin:0;\n"
"}\n"
"\n"
".bg {\n"
"  animation:slide 3s ease-in-out infinite alternate;\n"
"  background-image: linear-gradient(-60deg, #1E1F26 50%, #FFFFFF 50%);\n"
"  bottom:0;\n"
"  left:-50%;\n"
"  opacity:.5;\n"
"  position:fixed;\n"
"  right:-50%;\n"
"  top:0;\n"
"  z-index:-1;\n"
"}\n"
"\n"
".bg2 {\n"
"  animation-direction:alternate-reverse;\n"
"  animation-duration:4s;\n"
"}\n"
"\n"
".bg3 {\n"
"  animation-duration:5s;\n"
"}\n"
"\n"
".content {\n"
"  background-color:rgba(255,255,255,.8);\n"
"  border-radius:.25em;\n"
"  box-shadow:0 0 .25em rgba(0,0,0,.25);\n"
"  box-sizing:border-box;\n"
"  left:50%;\n"
"  padding:10vmin;\n"
"  position:fixed;\n"
"  text-align:center;\n"
"  top:50%;\n"
"  transform:translate(-50%, -50%);\n"
"}\n"
"\n"
"h1 {\n"
"  font-family:monospace;\n"
"}\n"
"\n"
"@keyframes slide {\n"
"  0% {\n"
"    transform:translateX(-25%);\n"
"  }\n"
"  100% {\n"
"    transform:translateX(25%);\n"
"  }\n"
"}\n"
"}\n"
".noselect {\n"
"  -webkit-touch-callout: none;\n"
"    -webkit-user-select: none;\n"
"     -khtml-user-select: none;\n"
"       -moz-user-select: none;\n"
"        -ms-user-select: none;\n"
"            user-select: none;\n"
"\t\t-webkit-tap-highlight-color: transparent;\n"
"}\n"
"\n"
".button {\n"
"\twidth: 70%;\n"
"\theight: 50px;\n"
"  text-decoration: none;\n"
"\tcursor: pointer;\n"
"\tbackground-color: #0072E3;\n"
"\tborder: none;\n"
"  color: #EEE;\n"
"\tborder-radius: 30px;\n"
"  background-image: linear-gradient(-120deg, #123C69 0, #1A5FA2 100%);\n"
"\tbox-shadow: 0 0 200px rgba(255,255,255,0.0);\n"
"\ttransition: 1s;\n"
"\tdisplay: flex;\n"
"\talign-items: center;\n"
"\tjustify-content: center;\n"
"  margin: 5px;\n"
"}\n"
"\n"
"button:after {\n"
"\tposition: absolute;\n"
"\tcontent: '';\n"
"\twidth: 25px;\n"
"\theight: 25px;\n"
"\tborder-radius: 100%;\n"
"\tbackground: transparent;\n"
"\tbox-shadow: 0 0 80px rgba(255,255,255,1);\n"
"}\n"
"\n"
"button:hover {\n"
"\tbox-shadow: 0 0 100px rgba(255,255,255,1);\n"
"}\n"
"\n"
"button:hover:after {\n"
"\tanimation: animate 3s infinite linear;\n"
"}\n"
"\n"
"button:focus {\n"
"\toutline: none;\n"
"}\n"
"\n"
"@keyframes animate {\n"
"\t0% { transform: translateX(60px)}\n"
"\t50% {transform: translateX(-60px)}\n"
"\t100% {transform: translateX(60px)}\n"
"}\n"
".buttons {\n"
"  display: flex;\n"
"  flex-direction: row;\n"
"  flex-wrap: wrap;\n"
"  justify-content: center;\n"
"  text-align: center;\n"
"  width: 100%;\n"
"  height: 100%;\n"
"  margin: 0 auto;\n"
"}\n"
".container {\n"
"  display: inline-block;\n"
"  align-items: center;\n"
"  display: flex;\n"
"  flex-direction: column;\n"
"  justify-content: top;\n"
"  text-align: center;\n"
"  background-image: linear-gradient(-180deg, #1E1F26 0, #283655 100%);\n"
"  padding:  40px 0px; \n"
"  width: 300px;\n"
"  margin-top: 10vh;\n"
"  margin-bottom: 10vh;\n"
"  margin-left: auto;\n"
"  margin-right: auto;\n"
"  //border-top: solid 1px rgba(255,255,255,.5);\n"
"  border-radius: 10px;\n"
"  box-shadow: 0px 2px 7px rgba(0,0,0,0.2);\n"
"  overflow: hidden;\n"
"}\n"
"\n"
"h4 {\n"
"  text-align: center;\n"
"  color: #EEE;\n"
"  letter-spacing: 0.05em;\n"
"  margin: 0 0 0.4em;\n"
"  font-size: 1em;\n"
"}\n"
"p {\n"
"  text-align: left;\n"
"  color: #EEE;\n"
"  letter-spacing: 0.05em;\n"
"  font-size: 0.8em;\n"
"  margin: 0 0 2em;\n"
"}    \n"
"</style>\n"
" <title>24 channel smart switch with Wi-Fi support</title>\n"
"\n"
"</head>\n"
"<body>\n"
"  <div class=\"bg\"></div>\n"
"<div class=\"bg bg2\"></div>\n"
"<div class=\"bg bg3\"></div>\n"
"\n"
"  <div class=\"buttons\">\n"
"  <div class=\"container\">\n"
"      <h4>ROOM N1</h4>\n"
"      <p>Channel 1</p>\n"
"      <a href=\"ch1=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch1=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 2</p>\n"
"      <a href=\"ch2=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch2=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 3</p>\n"
"      <a href=\"ch3=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch3=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 4</p>\n"
"      <a href=\"ch4=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch4=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 5</p>\n"
"      <a href=\"ch5=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch5=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 6</p>\n"
"      <a href=\"ch6=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch6=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 7</p>\n"
"      <a href=\"ch7=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch7=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 8</p>\n"
"      <a href=\"ch8=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch8=OFF\" class=\"button\">OFF</a>\n"
"  </div>\n"
"  <div class=\"container\">\n"
"      <h4>ROOM N2</h4>\n"
"      <p>Channel 9</p>\n"
"      <a href=\"ch9=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch9=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 10</p>\n"
"      <a href=\"ch10=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch10=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 11</p>\n"
"      <a href=\"ch11=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch11=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 12</p>\n"
"      <a href=\"ch12=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch12=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 13</p>\n"
"      <a href=\"ch13=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch13=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 14</p>\n"
"      <a href=\"ch14=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch14=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 15</p>\n"
"      <a href=\"ch15=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch15=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 16</p>\n"
"      <a href=\"ch16=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch16=OFF\" class=\"button\">OFF</a>\n"
"  </div>\n"
"    <div class=\"container\">\n"
"      <h4>ROOM N3</h4>\n"
"      <p>Channel 17</p>\n"
"      <a href=\"ch17=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch17=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 18</p>\n"
"      <a href=\"ch18=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch18=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 19</p>\n"
"      <a href=\"ch19=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch19=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 20</p>\n"
"      <a href=\"ch20=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch20=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 21</p>\n"
"      <a href=\"ch21=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch21=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 22</p>\n"
"      <a href=\"ch22=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch22=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 23</p>\n"
"      <a href=\"ch23=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch23=OFF\" class=\"button\">OFF</a>\n"
"            </br>\n"
"      <p>Channel 24</p>\n"
"      <a href=\"ch24=ON\" class=\"button\">ON</a>\n"
"      <a href=\"ch24=OFF\" class=\"button\">OFF</a>\n"
"  </div>\n"
"</div>\n"
"\n"
"</body>\n"
"</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
