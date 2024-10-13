/*
  Simple example for receiving
 
  https://github.com/sui77/rc-switch/
*/
 
#include <RCSwitch.h>
 
RCSwitch mySwitch = RCSwitch();
int oldvalue;
int value;
 
void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}
 
void loop() {
  if (mySwitch.available()) {
   
   
        Serial.print("Recebido: ");
        Serial.print( mySwitch.getReceivedValue() );      
        Serial.print(" / ");
        Serial.print( mySwitch.getReceivedBitlength() );
        Serial.print("bit ");
        Serial.print("Protocolo: ");
        Serial.println( mySwitch.getReceivedProtocol() );
        mySwitch.resetAvailable();
       

}
}
