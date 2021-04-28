
#include <avr/io.h>
#define PI 3.1415926535897932384626433832795

/*This code uses Arduino Uno digital pwm pin 3 and gnd to output a pwm signal through the solidstate relay that controls the drill speed.
Can substitute an LED or probably a servo motor for low power testing. Need to take rpm data normalized between integers 0 - 120 to establish 
current and newspeed values. 
*/
//has incremental speed controls
float Duty = 3*PI/2; //Initializes the Duty Cycle at 0%
float OnCount; int TopCount = 255; int On;
//======== Simulating RPM Sensor Feedback ================
float rpmMax = 1000; //arbitrary value
float rpm = 0; //current value (arbitrary)
//float rpmDes = 350; //Desired value (arbitrary)
//========================================================
int newspeed = 255;
int current; 
int DcShift;
float tempFloat = 0;
int comPortInitialization;
byte direction = 1;
int tempInt = 0;
int top_switch = 0;
int bottom_switch = 0;


void setup() {
          fastp_init(); //set registers for fast pwm, mode 7, non-inverting
          drill_init(); //Set top value for 65Hz operation, initialize at 0% duty cycle, 
          Serial.begin(115200); // setup serial
          pinMode(36, OUTPUT);
          pinMode(30, INPUT);
          pinMode(32, INPUT);
          attachInterrupt(digitalPinToInterrupt(30), DOWN, HIGH);
          attachInterrupt(digitalPinToInterrupt(32), UP, HIGH);
          digitalWrite(36,direction);
        }

void loop() {
    if(Serial.available()>0)
  {
    char incomingByte = Serial.read();
    Serial.println(incomingByte);
    
    if(incomingByte == 'A')
    {
      SpeedUp();
    }
    if(incomingByte == 'F')
    {
      SlowDown();
    }
    if(incomingByte == 'S')
    {
      Stop();
    }
    if(incomingByte == 'M')
    {
      maxSpeed();
    }
    if(incomingByte == 'B')
    {
      slowestSpeed();
    }
    if(incomingByte == 'U')
    {
      UP();
    }
    if(incomingByte == 'D')
    {
      DOWN();
    }
  }
//  top_switch = digitalRead(30);
//  bottom_switch = digitalRead(32);
//  if(top_switch == 1){
//    DOWN();//sends it down
//  }
//  if(bottom_switch == 1){
//    UP();
//  }
    OCR2A = newspeed; //Top value for this button
    tempFloat = OCR2A/2;
    tempInt = int(tempFloat);
    OCR2B = tempInt; //Ontime of the 65Hz signal Initialized to zero
}
void SlowDown(){
  if(newspeed >= 255)
  {
    newspeed = 255;
  }
  else{
    newspeed = newspeed + 20;
  }
  //incomingByte = NULL;
}
void SpeedUp(){
  if(newspeed <= 20){
    newspeed = 20;
  }
  else{
    newspeed = newspeed - 20;
  }
  //incomingByte = NULL;
  
}
void Stop(){
  newspeed = 0;
  //incomingByte = NULL 
}
void maxSpeed(){
  newspeed = 20;
  //incomingByte = NULL 
}
void slowestSpeed(){
  newspeed = 255;
  //incomingByte = NULL 
}
void DOWN(){
  digitalWrite(36,LOW); 
}
void UP()
{
  digitalWrite(36, HIGH);
}
    



  void fastp_init (void){
  //This specifically uses PD3 as the PWM output pin.
  pinMode(9,OUTPUT); //Set Port D pin 3 to output. Connecting OC2B to the pin (Speaker Fast PWM pin).
  TCCR2A |= (1<<WGM21) | (1<<WGM20); //Setting up wave generation mode for fast PWM. Still need to set WGM22
  TCCR2B |= (1<<WGM22); //Fast PWM wave generation mode 7 set. OCR2A is TOP.
  TCCR2A |= (1<<COM2B1); //COM2B0 is default 0. COM2B1 set for non-inverting operation.
}

void drill_init (void){
    OCR2A = TopCount; //Top value for this button
    tempFloat = TopCount/2;
    tempInt = int(tempFloat);
    OCR2B = tempInt; //Ontime of the 65Hz signal Initialized to zero
    TCCR2B |= (1<<CS21); //Pre-scale = 8   16M/(8*256) = 7.8125kHz and start the clock
  }
 