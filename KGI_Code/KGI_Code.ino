
#include <LiquidCrystal.h>
#include "IRremote.h"
#define ENABLE 2 // goes to pin 1
#define DIRA 1 //goes to pin 7p
#define DIRB 4
#define BLUE 3
#define GREEN 5
#define RED 6
int receiver = 13; // Signal Pin of IR receiver to Arduino Digital Pin 13
int i;
int Time;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{

  switch(results.value)

  {
  case 0xFFA25D:
  Serial.println("POWER"); //ON
  display();
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
  break;
  case 0xFFE21D:
  Serial.println("FUNC/STOP"); //OFF
  digitalWrite(ENABLE,LOW);
  lcd.noDisplay();
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  break;
  case 0xFF629D: 
  Serial.println("VOL+"); // Forward Direction
  digitalWrite(ENABLE,LOW);
  delay(1000);
  digitalWrite(ENABLE,HIGH);
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  break;
  case 0xFFA857: 
  Serial.println("VOL-"); // Backward Direction
  digitalWrite(ENABLE,LOW);
  delay(1000);
  digitalWrite(ENABLE,HIGH);
  digitalWrite(DIRA,LOW);  //reverse
  digitalWrite(DIRB,HIGH);
  break;

  default: 
    Serial.println(" other button : ");
    Serial.println(results.value);

  }// End Case
  delay(500); // Do not get immediate repeat
}
void display(){
  lcd.display();
  lcd.setCursor(0,0);
  lcd.print("Time Elapsed:");


  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the Receiver
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  lcd.begin(16, 2);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

void loop() {
Time = (millis()/1000);
if (irrecv.decode(&results)) // have we received an IR signal?

  {
      Time = (millis()/1000);
  // put your main code here, to run repeatedly:
  #define delayTime 10
  lcd.setCursor(0,1);
  lcd.print(Time);
  lcd.print(" seconds");
    translateIR(); 
    // if received value is up volume, turn on fan, LCD display showing time since turned on.
    // if received value is down volume, turn off fan
    delay(1000);
    irrecv.resume(); // receive the next value
  }
  if ((Time > 30) & (Time < 60))
  {
      lcd.setCursor(0,1);
  lcd.print(Time);
  lcd.print(" seconds");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE,LOW);
  }
  if (Time > 60)
  {
      lcd.setCursor(0,1);
  lcd.print(Time);
  lcd.print(" seconds");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE,LOW);

    
  }
}
