 /* TTP224 4-Channel Capacitive Touch Module */
 /*           Options Observation            */ 
 /*
 *  Display data from the TTP224. Setup based on the Module's
 *  'out-of-the-bag' options, all shorts open.
 *  Can be used for further observation of CMOS and Open Drain
 *  but each case requires a differing circuit.
 *  CMOS Active High          - default case
 *  CMOS Active Low           - Use curent limitimg circuit
 *  Open Drain, Active High   - Use Pull Down (example 10K)
 *  Open Drain, Active Low    - Use Pull Up (example 4.6K)
 *  
 *  Created:  April 2016
 *            For Instructable
 *  By     :  TrewRoad
 */
 /*  ** In case it is unfamiliar **
 *  Conditional Compilation
 *  This sketch has conditional compilation to handle the respective
 *  active HIGH/LOW condition at the digital data pins.
 *  Accessing an Array
 *  This sketch has an array. The first element of the array is at index 0.
 */
 
#define AHLB_VDD                    // Conditional Compilation.

const byte the_pins[]={14,12,13,15};
//const byte the_pins[]={9,8,7,6};  // use this combo if easier to wire
            // use any sequence where Module Pad #1
            // is the first array element e.g. {6}
            // should display adequately with this sketch.
int i;
boolean re_start=true; 
void setup()
{
  Serial.begin(115200);
  // Set the pin modes on digital pins
  for(i=0; i<sizeof(the_pins); i++)
  {
    #ifdef AHLB_VDD
    pinMode(the_pins[i], INPUT_PULLUP);  //Internal pull up not useful in open drain
                                         //but does no harm.
    #else
    pinMode(the_pins[i], INPUT);
    #endif
  }
  //Serial.println("Enter Loop Processing");  //Only of utility when loop() is empty (or suspect)
  re_start=true;
}
 
void loop()
{
  if(re_start) {Serial.println("Ready to go");re_start=false;}
  String a_msg="";
  for(i=0; i<sizeof(the_pins); i++)
  {
    #ifdef AHLB_VDD
    if(digitalRead(the_pins[i]) == LOW)
    #else
    if(digitalRead(the_pins[i]) == HIGH) 
    #endif
    {
      //Correlate to module Key Pads
      a_msg += String(i+1);
    }
    else {
      a_msg += " ";
    }
    a_msg += " ";
  }
  String b_msg=a_msg;
  a_msg.trim();
  if (a_msg.length()>0)   Serial.println(b_msg);
  delay(250);         // 4 times per second
}
