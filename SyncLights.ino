#include <Time.h>
#include <SPI.h>
#include <Adafruit_WS2801.h>
#include <Ethernet.h>
#include "PatternInstruction.h"
#include "PatternList.h"


/* ******** Settings *********/

// pin settings
int dataPin = 2;  // Yellow wire
int clockPin = 3;  // Green wire

// Server settings
int serverPort = 1190;
IPAddress centralServer(192,168,1,122);
int centralServerPort = 80;
EthernetClient syncClient;

// Object setup
Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);
byte mac[] = { 0xEA, 0xF3, 0xD1, 0xAD, 0x11, 0x3F };
// Shield should grab a DHCP address
EthernetServer server(serverPort);
int grid[5][5] = {
  { 0, 1, 2, 3, 4 },
  { 5, 6, 7, 8, 9 },
  { 10, 11, 12, 13, 14 },
  { 14, 16, 17, 18, 19 },
  { 20, 21, 22, 23, 24 }
};
PatternList *cur_list = new PatternList();

unsigned long pattern_start;
unsigned long pattern_stop;

boolean need_set_ready = true;

/* ********* Setup *********/
void setup() {
  Ethernet.begin(mac);
  delay(1000);  // Just make sure we have time to get an address via DHCP
  server.begin();
  strip.begin();
  strip.show();
  randomSeed(analogRead(0));
  Serial.begin(9600);
  setTime(0,0,0,0,0,0);
  clientRegister();
}

/* ********* Main *********/
void loop() {
  if (need_set_ready) {
    clientSetReady();
    need_set_ready = false;
  }
  wait_for_client();
}

void wait_for_client() {
  colorAll(0);
  EthernetClient client = server.available();
  if (client == true) {
    need_set_ready = true;
    Serial.println("Connected");
    // Reset the list
    cur_list = new PatternList();
    // We have a connection
    byte init = client.read();
    Serial.println("Init");
    // 0x01 == start pattern
    if (init == 0x01) {
      // Get the 32bit pattern length
      int pattern_len = client.read();
      pattern_len <<= 8;
      pattern_len |= client.read();
      
      pattern_len <<= 8;
      pattern_len |= client.read();
      
      pattern_len <<= 8;
      pattern_len |= client.read();

      // how many seconds to loop the pattern
      uint8_t pattern_time = client.read();
      for (int i = 0; i < pattern_len; i++) {
        // read the color
        client.read(); // Blank byte
        Color c;
        c.r = client.read();
        c.g = client.read();
        c.b = client.read();
        // read the coords
        uint8_t x = client.read();
        uint8_t y = client.read();
        uint16_t t = client.read();

        // read the delay
        t <<= 8;
        t |= client.read();
        // make the instruction
        PatternInstruction *pi = new PatternInstruction(x,y,c,t);
        cur_list->add(pi);
      }
      client.println("load complete");
      Serial.println("load complete");
      client.stop();
      PatternNode *cur_node;
      pattern_start = millis();
      pattern_stop = pattern_start + (long)pattern_time*1000L;
      while (true) {
        while ((cur_node = cur_list->getNext())){
          PatternInstruction *cur_instr = cur_node->getInstruction();
          strip.setPixelColor(grid[cur_instr->getX()][cur_instr->getY()], cur_instr->getColor());
          strip.show();
          delay(cur_instr->getTime());
          if (millis() > pattern_stop) {
            break;
          }
        }
        if (millis() > pattern_stop) {
          break;
        }
        cur_list->reset();
      }
    }
  }
}

void clientSetReady() {
  Serial.println("Setting ready...");
  if(syncClient.connect(centralServer, centralServerPort)) {
    syncClient.print("GET /?action=ready&ip=");
    syncClient.print(Ethernet.localIP());
    syncClient.print("&port=");
    syncClient.print(serverPort);
    syncClient.print(" HTTP/1.1");
    syncClient.println();
    syncClient.println();
    syncClient.stop();
  } else {
    Serial.println("Client connection failed!");
  }
}

void clientRegister() {
  Serial.println("Registering...");
  if (syncClient.connect(centralServer, centralServerPort)) {
    syncClient.print("GET /?action=register&ip=");
    syncClient.print(Ethernet.localIP());
    syncClient.print("&port=");
    syncClient.print(serverPort);
    syncClient.print(" HTTP/1.1");
    syncClient.println();
    syncClient.println();
    syncClient.stop();
  } else {
    Serial.println("Client connection failed!");
  }
}
// set all pixels to a "Color" value
void colorAll(uint32_t c) {
 int i;
 for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
 }
 strip.show();
}
