#include <LiquidCrystal.h>
#include "IRremote.h"
#define ENABLE 5
#define DIRA 3
#define DIRB 4
int receiver = 13; // Signal Pin of IR receiver to Arduino Digital Pin 13
int i;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{

  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D: Serial.println("VOL+"); break;
  case 0xFFA857: Serial.println("VOL-");    break;

  default: 
    Serial.println(" other button : ");
    Serial.println(results.value);

  }// End Case

  delay(500); // Do not get immediate repeat
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the Receiver
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
if (irrecv.decode(&results)) // have we received an IR signal?

  {
    translateIR(); 
    // if received value is up volume, turn on fan, LCD display showing time since turned on.
    // if received value is down volume, turn off fan
    irrecv.resume(); // receive the next value
  }
}
