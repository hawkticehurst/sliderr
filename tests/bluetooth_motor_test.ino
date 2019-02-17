#include <Metro.h>
#include "GoBLE.h"
int joystickX;
int M1dirpin = 4;
int M1steppin = 5;
int M1en =1 2;        
bool motorEnableFlag;

Metro blink1Metro = Metro(1000);
Metro blink2Metro = Metro(0);
void setup() {
  // put your setup code here, to run once:
  pinMode(M1dirpin,OUTPUT);
  pinMode(M1steppin,OUTPUT);
  pinMode(M1en,OUTPUT);
  Goble.begin();
  Serial.begin(115200);
}
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(joystickX);
   if(Goble.available())
    {
      joystickX = Goble.readJoystickX();
      if (joystickX > 128)
       {
        digitalWrite(M1dirpin,LOW);
        joystickX=255-joystickX;
       }
       else if (joystickX<128)
        {
          digitalWrite(M1dirpin,HIGH);
          joystickX=joystickX-1;
          }
          if (joystickX==128)
          motorEnableFlag=false;
          else
          motorEnableFlag=true;

    }
Metro blink2Metro = Metro(joystickX/50);
if(blink1Metro.check()){
 Serial.println(joystickX);
  }
    if(blink2Metro.check()&&motorEnableFlag){
    digitalWrite(M1steppin,LOW);
   digitalWrite(M1steppin,HIGH);

 }
}
