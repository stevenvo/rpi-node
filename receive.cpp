#include <cstdlib>
#include <iostream>
#include <RF24/RF24.h>
 
using namespace std;
 
// spi device, spi speed, ce gpio pin
RF24 radio(0,22);
const uint8_t pipes[][11] = {"svo-master","svo-sensor"};

typedef struct {
  float w;
  float x;
  float y;
  float z;
} Quat;

typedef struct {
  Quat Q;
} Mpu_Data;

Mpu_Data mpu_data;


void setup(void) {
   // init radio for reading
   radio.begin();
   radio.enableDynamicPayloads();
   radio.setAutoAck(1);
   radio.setRetries(15,15);
   //radio.setDataRate(RF24_1MBPS);
   //radio.setPALevel(RF24_PA_MAX);
   //radio.setChannel(76);
   //radio.setCRCLength(RF24_CRC_16);
   radio.openReadingPipe(1,pipes[1]);
   radio.startListening();
   radio.printDetails();
   cout << "Listening...\n";
}
 
void loop(void) {
   // 32 byte character array is max payload
   // char receivePayload[32];
   char str[80];
 
   while (radio.available()) {
      // read from radio until payload size is reached
      uint8_t len = radio.getDynamicPayloadSize();      
      // radio.read(receivePayload, len);
      // display payload
      // cout << receivePayload << endl;

      radio.read(&mpu_data, len);
      Quat quat = mpu_data.Q;
      sprintf(str, "Quat: w=%f, x=%f, y=%f, z=%f", quat.w, quat.x, quat.y, quat.z);
      cout << str << endl;
   }
}
 
int main(int argc, char** argv) {
   setup();
   while(1)
      loop();
 
   return 0;
}
