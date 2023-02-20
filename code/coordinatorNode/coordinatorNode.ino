
#include <XBee.h>

#define MAX_FRAME_DATA_SIZE 110

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();

int node;

int phH;
int phL;
int phHL;
float phV;

int tH;
int tL;
int tHL;
float tV;

void setup() {
  
 Serial.begin(9600);
 xbee.begin(Serial); 
}
 
void loop() {
 xbee.readPacket();
 if (xbee.getResponse().isAvailable()) {
  if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
   xbee.getResponse().getZBRxResponse(rx);
  }

  node = rx.getData(0);
  
  phH = rx.getData(1);
  phL = rx.getData(2);
  phHL = phL + (phH * 256);
  phV=(float)phHL*5.0/1024; 
  phV=3.5*phV;
  
  tH = rx.getData(3);
  tL = rx.getData(4);
  tHL = tL + (tH * 256);
  tV=(float)tHL * (5.0 /1024.0);
  
 }
 
 Serial.write(node);
 Serial.write(phH);
 Serial.write(phL);
 Serial.write(tH);
 Serial.write(tL);
 
 delay(80);
}
