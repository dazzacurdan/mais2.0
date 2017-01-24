/*
 * M@is 2.0
 * 
 *    WIRING:
 * WHITE:   Coin data   2   
 * GRAY:    Relè data   7
 * WHITE:   Relè data   8   
 * ORANGE:  Servo data  9
 * RED:     +5
 * BLACK:   GND
 * 
 */

#include <Servo.h>

Servo myservo;

struct PIN
{
  int rele0;
  int rele1;
  int stepper;
};

int pos = 0; 

const int coinInt = 0; 
//Attach coinInt to Interrupt Pin 0 (Digital Pin 2). Pin 3 = Interrpt Pin 1.

volatile float coinsValue = 0.00;
//Set the coinsValue to a Volatile float
//Volatile as this variable changes any time the Interrupt is triggered
int coinsChange = 0;                  
//A Coin has been inserted flag

PIN pin;

void setup()
{
   pin= {7,8,9};
  
  Serial.begin(9600);                 
  //Start Serial Communication
  attachInterrupt(coinInt, coinInserted, RISING);
  //If coinInt goes HIGH (a Pulse), call the coinInserted function
  //An attachInterrupt will always trigger, even if your using delays
  pinMode(pin.rele0, OUTPUT);

  myservo.attach(pin.stepper);
     

}

void coinInserted()    
//The function that is called every time it recieves a pulse
{
  Serial.println("Coin insered");
  coinsValue = coinsValue + 0.05;  
//As we set the Pulse to represent 5p or 5c we add this to the coinsValue
  coinsChange = 1;                           
//Flag that there has been a coin inserted
}

void loop()
{
  if(coinsChange == 1)//Check if a coin has been Inserted
  {
    coinsChange = 0;  
    Serial.print("Credit: £");
    Serial.println(coinsValue);
    for(pos = 0; pos < 90; ++pos)
    {
      myservo.write(pos);
      delay(15);
    }
    digitalWrite(pin.rele0, LOW);  
    delay(5000);  
    digitalWrite(pin.rele0, HIGH);
    Serial.println("FINISH");
  }
}
