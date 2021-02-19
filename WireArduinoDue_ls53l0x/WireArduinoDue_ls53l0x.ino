// Wire Arduino to Gilisymo Plugin
// by Giovanni Scozzola <https://giovanniscozzola.blogspot.fr/>

// This example demonstrate how to read ranging data from Gilisymo Plugin LS53L0X by using the i2c interface.
// Here we suppose that we have only one plugin. Instead of using a fixed address, at the begin, 
// we do a for loop to discover which plugin is connected.
// When a plugin is detected, the address is saved and this is used to read the ranging data forever.
// To access the ranging data we read two bytes on index A8 as you can check in the LS documentation 
// @ <https://gilisymo.com/content/6-documentation> : the first byte is the MSB and the second is the LSB.
// The ranging data is printed out on the Serial connection and then this can be seen on Serial Monitor or Plotter Monitor.

// Created 23 April 2017

// This example code is in the public domain.


#include <Wire.h>

#define RangingAddress 0xA8

int serch_done = 0;
int addressfound = 0;

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
}

int getRangingdata(uint8_t slaveaddress)
{
  byte b0;
  byte b1;
  int result;

  // get two registers containing ranging data
  Wire.requestFrom(slaveaddress, 2, RangingAddress, 1, true);
  b0 = Wire.read(); // contains the ranging MSB
  b1 = Wire.read(); // contains the ranging LSB

  result = b0*256 + b1;

  return result;
}

void loop()
{
byte address;
byte error;
int rangingValue;

  if (!serch_done) {
    // search the address of the LS Plugin, this is done only once!
    for(address=1;address<127;address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
          serch_done = 1;
          addressfound = address;
          break;
        }
    }
  } else {
        Serial.print("LS@0x");
        if (addressfound<16)
          Serial.print("0");
        Serial.print(addressfound,HEX);
        Serial.print(": ");

        rangingValue = getRangingdata(addressfound);
        Serial.println(rangingValue);

  }

  delay(100);
}
