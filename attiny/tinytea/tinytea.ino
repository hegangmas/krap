#include <TEA5767Tiny.h>

TEA5767Tiny Radio;
double stored_frequency;
int search_mode = 0;
int search_direction;
unsigned long current_time, last_written;

int ledPin = 1;
int btn_backward = 4; 
int btn_forward = 3;

int b=0;
int f=0;

boolean state = false;
void setup() {
  
pinMode(ledPin, OUTPUT);  
pinMode(btn_backward, INPUT);  
pinMode(btn_forward, INPUT);   
 
  Radio.init();    
  Radio.set_frequency(88.9);  
  digitalWrite(ledPin,HIGH);
}

void loop() {
  
  unsigned char buf[5];
  double current_freq;
  
  current_time = millis();
  
  if (Radio.read_status(buf) == 1) {
    current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
    if (current_freq != stored_frequency && current_time - last_written > 10000 && search_mode == 0) {
      stored_frequency = current_freq;
      last_written = current_time;
    }
  }
  
  if (search_mode == 1) {
        digitalWrite(ledPin,LOW);
      if (Radio.process_search (buf, search_direction) == 1) {
          search_mode = 0;
            digitalWrite(ledPin,HIGH);
      }
  }
  
  if(b==0&&digitalRead( btn_backward)==0){
    b=1;    
  }
  if(b==1&&digitalRead( btn_backward)==1){
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    delay(300);
    b=0;
  }
  
   if(f==0&&digitalRead( btn_forward)==0){
    f=1;    
  }
   if(f==1&&digitalRead( btn_forward)==1){
     search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    delay(300);
    f=0;
   }
  delay(100);
}
