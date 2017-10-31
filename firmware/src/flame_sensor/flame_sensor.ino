#include "Wire,h";

int warnPin = 16;                   // loa,den ket noi Pin 10
int button=0;                           // button ket noi Pin 2
int flame_din=10;                    // Dout ket noi Pin 9
int flame_ain=A0;                 // Aout ket noi Pin A0
int ad_value;                          // bien luu gia tri adc
int button_value;                   // bien luu trang thai button
void setup() {
  pinMode(flame_din,INPUT);             // set Input  
  pinMode(flame_ain,INPUT);             // set Input 
  pinMode(warnPin,OUTPUT);            // set Output
  pinMode(button,INPUT);                    // set Input
  Serial.begin(9600);                             // ket noi UART, baud = 9600
}
 
void loop() {
  ad_value=analogRead(flame_ain);              // doc gia tri adc
  if(digitalRead(flame_din)==LOW)                  // so sanh vs LOW, neu = thong bao co lua, den va loa bat
  {
    Serial.println("CO LUA");
    Serial.println(ad_value);
    digitalWrite(warnPin,HIGH);
  }
  button_value = digitalRead(button);                 // doc trang thai button
  if(button_value==HIGH)                                      // nhan button
  { 
    while(button_value == HIGH)                           // doi phim nha
    {  
      button_value = digitalRead(button);
    }
    digitalWrite(warnPin,LOW);                               // tat loa va den
    Serial.println("BINH THUONG");                       // thong bao binh thuong
  }
}
