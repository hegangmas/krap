#include <Wire.h>
#include <TEA5767Radio.h>

TEA5767Radio Radio;

int ledPin = 1;
int btn_backward = 4; 
int btn_forward = 3;
double freq=88.9;

int b=0;
int f=0;

boolean state = false;
void setup() {
  
pinMode(ledPin, OUTPUT);  
pinMode(btn_backward, INPUT);
digitalWrite(btn_backward, HIGH);
pinMode(btn_forward, INPUT);   
digitalWrite(btn_forward, HIGH);
  Radio.setFrequency(freq);  
}

void loop() {

  if(b==0&&digitalRead( btn_backward)==0){
    b=1;    
  }
  if(b==1&&digitalRead( btn_backward)==1){
    
    if(freq>87.5){freq=freq-0.1;}
    
    b=0;
  }
  
   if(f==0&&digitalRead( btn_forward)==0){
    f=1;    
  }
   if(f==1&&digitalRead( btn_forward)==1){
     if(freq<108){freq=freq+0.1;}
    f=0;
   }
  delay(100);
  Radio.setFrequency(freq); 
}
