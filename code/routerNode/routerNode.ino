#include <XBee.h>

#define MAX_FRAME_DATA_SIZE 110

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();

uint8_t payload[] = { 0, 0, 0, 0, 0};

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40d940e5);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int phH;
int phL;
int tH;
int tL;
int node;

int statusLed = 13;
int errorLed = 13;

void flashLed(int pin, int times, int wait) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}

void setup() {
  
 pinMode(statusLed, OUTPUT);
 pinMode(errorLed, OUTPUT);
  
 Serial.begin(9600);
 xbee.begin(Serial);
 xbee.setSerial(Serial);
  
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
  tH = rx.getData(3);
  tL = rx.getData(4);

 }
 
  payload[0] = node;
  payload[1] = phH;
  payload[2] = phL;
  payload[3] = tH;
  payload[4] = tL;
  
  xbee.send(zbTx);

  // flash TX indicator
  flashLed(statusLed, 1, 10);

  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(500)) {
    // got a response!

    // should be a znet tx status              
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);

      // get the delivery status, the fifth byte
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        // success.  time to celebrate
        flashLed(statusLed, 5, 10);
      } else {
        // the remote XBee did not receive our packet. is it powered on?
        flashLed(errorLed, 3, 10);
      }
    }
  } else if (xbee.getResponse().isError()) {
    //nss.print("Error reading packet.  Error code: ");  
    //nss.println(xbee.getResponse().getErrorCode());
  } else {
    // local XBee did not provide a timely TX Status Response -- should not happen
    flashLed(errorLed, 2, 10);
  }
 
 
 delay(200);
}
