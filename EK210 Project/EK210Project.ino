

// which analog pins to connect
#define THERMISTORPIN A0
#define TPIN2 A1  
#define TPIN3 A2
#define TPIN4 A3        
#define PWMPIN 10
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    

// LCD business
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);



int samples[NUMSAMPLES];
int samples2[NUMSAMPLES];
int samples3[NUMSAMPLES];
int samples4[NUMSAMPLES];

void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);
  pinMode(PWMPIN, OUTPUT);
  
 // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
}

void loop(void) {
  uint8_t i;
  float average;
  float average2;
  float average3;
  float average4;

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   samples2[i] = analogRead(TPIN2);
   samples3[i] = analogRead(TPIN3);
   samples4[i] = analogRead(TPIN4);
   delay(10);
  }
  
  // average all the samples out
  average = 0;
  average2 = 0;
  average3 = 0;
  average4 = 0;
 
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
     average2 += samples2[i];
     average3 += samples3[i];
     average4 += samples4[i];
  }
  average /= NUMSAMPLES;
  average2 /= NUMSAMPLES;
  average3 /= NUMSAMPLES;
  average4 /= NUMSAMPLES;


  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;

  average2 = 1023 / average2 - 1;
  average2 = SERIESRESISTOR / average2;

  average3 = 1023 / average3 - 1;
  average3 = SERIESRESISTOR / average3;
  
  average4 = 1023 / average4 - 1;
  average4 = SERIESRESISTOR / average4;
  
  
  float steinhart1;
  steinhart1 = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart1 = log(steinhart1);                  // ln(R/Ro)
  steinhart1 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart1 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart1 = 1.0 / steinhart1;                 // Invert
  steinhart1 -= 273.15;  // convert to C
  steinhart1 = steinhart1 + 1.64; //offset because the resistor attached to it sucks

  float steinhart2;
  steinhart2 = average2 / THERMISTORNOMINAL;     // (R/Ro)
  steinhart2 = log(steinhart2);                  // ln(R/Ro)
  steinhart2 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart2 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart2 = 1.0 / steinhart2;                 // Invert
  steinhart2 -= 273.15;                         // convert to C
  
   float steinhart3;
  steinhart3 = average3 / THERMISTORNOMINAL;     // (R/Ro)
  steinhart3 = log(steinhart3);                  // ln(R/Ro)
  steinhart3 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart3 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart3 = 1.0 / steinhart3;                 // Invert
  steinhart3 -= 273.15;                         // convert to C
  float steinhart4;
  steinhart4 = average4 / THERMISTORNOMINAL;     // (R/Ro)
  steinhart4 = log(steinhart4);                  // ln(R/Ro)
  steinhart4 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart4 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart4 = 1.0 / steinhart4;                 // Invert
  steinhart4 -= 273.15;                         // convert to C

  float AverageOfAll;
  AverageOfAll = 0;
  AverageOfAll = steinhart1 + steinhart2 + steinhart3 + steinhart4;
  AverageOfAll = AverageOfAll / 4;

  // More math
  float Maximum;
  float Minimum;
  Maximum = max(steinhart1,max(steinhart2,max(steinhart3,steinhart4)));
  Minimum = min(steinhart1,min(steinhart2,min(steinhart3,steinhart4)));
  
  unsigned long CurrentTime = millis();
  CurrentTime = CurrentTime / 60000; // Convert to minutes
 
  
  // Printing information (On Serial Monitor, sanity checks lol)
  Serial.println(steinhart1);
  Serial.println(steinhart2);
  Serial.println(steinhart3);
  Serial.println(steinhart4);
  Serial.print("Average Temperature ");
  Serial.print(AverageOfAll);
  Serial.println(" *C");
  
  Serial.print("Maximum Temperature ");
  Serial.print(Maximum);
  Serial.println(" *C");
  
  Serial.print("Minimum Temperature ");
  Serial.print(Minimum);
  Serial.println(" *C");
  Serial.print("Time Elapsed (minutes) ");
  Serial.print(CurrentTime);
  Serial.println(" ");
  Serial.println(" ");
  delay(1000);

  // Printing to LCD
    lcd.setCursor(0,0);
lcd.print("Avg   Time ");
 lcd.setCursor(0,1);
 lcd.print(AverageOfAll);
 lcd.print("*C ");
 lcd.print(CurrentTime);
 lcd.print(" Mins ");
 delay(3000);
lcd.setCursor(0,0);
lcd.print("Min   Max ");
lcd.setCursor(0,1);
lcd.print(Minimum);
lcd.print("*C ");
lcd.print(Maximum);
lcd.print("*C ");
delay(2000);

 
  // PWM Mess

int counter;
counter = 0;
  if (Maximum >= 36) digitalWrite(PWMPIN, LOW);
  
  if (Maximum <= 36) digitalWrite(PWMPIN, HIGH);
  

  






  
  }
