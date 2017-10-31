#include<Wire.h>

// MAX44009 I2C address is 0x4A(74)
#define Addr 0x4A

int warnPin = 16;
void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select configuration register
  Wire.write(0x02);
  // Continuous mode, Integration time = 800 ms
  Wire.write(0x40);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
  pinMode(warnPin,OUTPUT);
}

void loop()
{
  unsigned int data[2];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x03);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // luminance msb, luminance lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data to lux
  int exponent = (data[0] & 0xF0) >> 4;
  int mantissa = ((data[0] & 0x0F) << 4) | (data[1] & 0x0F);
  float luminance = pow(2, exponent) * mantissa * 0.045;

  // Output data to serial monitor
  Serial.print("Ambient Light luminance :");
  Serial.print(luminance);
  if(luminance == 0)
  {
    digitalWrite(warnPin,0);
  } else {
    digitalWrite(warnPin,1);
  }
  Serial.println(" lux");
  delay(300);
}
