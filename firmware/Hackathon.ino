#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "MAX30100_PulseOximeter.h"
#include "SH1106.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>


#define btnUp 14
#define btnDown 15
#define btnEnter 12
#define S_D 10
#define S_A A0
#define REPORTING_PERIOD_MS     1000
#define led 16
#define max_test 5
#define Addr 0x4A
#define SCL_PIN 5
#define SDO_PIN 4

byte Key;


int count = 1;


PulseOximeter pox;
SSD1306  display(0x3c, 4, 5);
void drawLines() {
  for (int16_t i=0; i<DISPLAY_WIDTH; i+=4) {
    display.drawLine(0, 0, i, DISPLAY_HEIGHT-1);
    display.display();
    delay(10);
  }
  for (int16_t i=0; i<DISPLAY_HEIGHT; i+=4) {
    display.drawLine(0, 0, DISPLAY_WIDTH-1, i);
    display.display();
    delay(10);
  }
  delay(250);

  display.clear();
  for (int16_t i=0; i<DISPLAY_WIDTH; i+=4) {
    display.drawLine(0, DISPLAY_HEIGHT-1, i, 0);
    display.display();
    delay(10);
  }
  for (int16_t i=DISPLAY_HEIGHT-1; i>=0; i-=4) {
    display.drawLine(0, DISPLAY_HEIGHT-1, DISPLAY_WIDTH-1, i);
    display.display();
    delay(10);
  }
  delay(250);

  display.clear();
  for (int16_t i=DISPLAY_WIDTH-1; i>=0; i-=4) {
    display.drawLine(DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1, i, 0);
    display.display();
    delay(10);
  }
  for (int16_t i=DISPLAY_HEIGHT-1; i>=0; i-=4) {
    display.drawLine(DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1, 0, i);
    display.display();
    delay(10);
  }
  delay(250);
  display.clear();
  for (int16_t i=0; i<DISPLAY_HEIGHT; i+=4) {
    display.drawLine(DISPLAY_WIDTH-1, 0, 0, i);
    display.display();
    delay(10);
  }
  for (int16_t i=0; i<DISPLAY_WIDTH; i+=4) {
    display.drawLine(DISPLAY_WIDTH-1, 0, i, DISPLAY_HEIGHT-1);
    display.display();
    delay(10);
  }
  delay(250);
}

// Adapted from Adafruit_SSD1306
void drawRect(void) {
  for (int16_t i=0; i<DISPLAY_HEIGHT/2; i+=2) {
    display.drawRect(i, i, DISPLAY_WIDTH-2*i, DISPLAY_HEIGHT-2*i);
    display.display();
    delay(10);
  }
}

// Adapted from Adafruit_SSD1306
void fillRect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<DISPLAY_HEIGHT/2; i+=3) {
    display.setColor((color % 2 == 0) ? BLACK : WHITE); // alternate colors
    display.fillRect(i, i, DISPLAY_WIDTH - i*2, DISPLAY_HEIGHT - i*2);
    display.display();
    delay(10);
    color++;
  }
  // Reset back to WHITE
  display.setColor(WHITE);
}

// Adapted from Adafruit_SSD1306

void setup() {
  display.init();
  display.setContrast(255);
  drawLines();
  delay(1000);
  display.clear();
  pinMode(btnUp, INPUT);
  pinMode(btnDown, INPUT);
  
    display.setFont(ArialMT_Plain_10);
  display.drawString(0,0,"Select: ");
  display.display();
  while (digitalRead(btnEnter)==0){
    display.clear();
    display.drawString(0,0,"Select: ");
    display.display();
    if (digitalRead(btnUp)==1){
      while (digitalRead(btnUp)==1);
      if (count<max_test)
        count++;
    }
    if (digitalRead(btnDown)==1){
      while (digitalRead(btnDown)==1);
      if (count>1)
        count--;
    }
  }
  switch (count)
  {
    case 1: setup_IO();
            break;
    case 2: setup_I2C_Nhip_Tim_Va_Oxy();
            break;
    case 3: setup_I2C_Max44009();
            break;
    case 4: setup_I2C_TTP229();
            break;
    case 5: setup_I2C_ApSuat();
            break;
  }
}

void setup_IO(){
  pinMode(S_D,INPUT);
}

void loop_IO(){
while (1){
  if (digitalRead(S_D)==1){
    digitalWrite(led,HIGH);
  } else {
    digitalWrite(led,LOW);
  }
  int sensor = analogRead(S_A);
  display.clear();
  display.drawString(0,0,"Select: "+(String)count);
  display.drawString(0,10,"Analog: "+(String)sensor);
  display.display();
}
}
void setup_I2C_Nhip_Tim_Va_Oxy(){
  pox.begin();
}

void loop_I2C_Nhip_Tim_Va_Oxy(){
while(1){
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        display.clear();
        display.drawString(0, 0, "HR: " + String(pox.getHeartRate()) + " bpm");
        display.drawString(0, 32, "SpO2: " + String(pox.getSpO2()) +" %");
        display.display();
        tsLastReport = millis();
    }
}
}

void setup_I2C_Max44009(){
  Wire.begin();
  Wire.beginTransmission(Addr);
  Wire.write(0x02);
  Wire.write(0x40);
  Wire.endTransmission();
  delay(300);
}

void loop_I2C_Max44009(){
while(1){  
  unsigned int data[2];
  Wire.beginTransmission(Addr);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(Addr, 2);
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  int exponent = (data[0] & 0xF0) >> 4;
  int mantissa = ((data[0] & 0x0F) << 4) | (data[1] & 0x0F);
  float luminance = pow(2, exponent) * mantissa * 0.045;
  
  display.clear();
  display.drawString(0,0,"Select: "+(String)count);
  display.drawString(0, 0, "Ambient Light luminance : " + String(luminance) + " lux");
  display.display();
  delay(300);
}
}

void setup_I2C_TTP229(){
  pinMode(SCL_PIN, OUTPUT);
  pinMode(SDO_PIN, INPUT);
}

void loop_I2C_TTP229(){
  while (1){
    Key = Read_Keypad();
    if (Key)
      Serial.println(Key);
    delay(100);
  }
}

byte Read_Keypad(void)
{
  byte Count;
  byte Key_State = 0;

  for(Count = 1; Count <= 16; Count++){
    digitalWrite(SCL_PIN, LOW);
    if (!digitalRead(SDO_PIN))
      Key_State = Count;
    digitalWrite(SCL_PIN, HIGH);
  }
  return Key_State;
}


void loop() { 
  switch (count)
  {
    case 1: loop_IO();
            break;
    case 2: loop_I2C_Nhip_Tim_Va_Oxy();
            break;
    case 3: loop_I2C_Max44009();
            break;
    case 4: loop_I2C_TTP229();
            break;
    case 5: loop_I2C_ApSuat();
            break;
  }
}
