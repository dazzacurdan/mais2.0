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

struct TIME
{
  int servo;
  int cottura;
};

struct PIN
{
  int rele0;
  int rele1;
  int stepper;
  byte interrupt0;
  byte interrupt1;
};

struct COIN
{
  volatile float value;
  volatile bool isChanged;
};

TIME time_t;                   
PIN pin;
COIN coin;

void setup()
{
  time_t = {40,5000};
  pin = {7,8,9,3,2};
  coin = {0.00, false};
  
  Serial.begin(9600);                 
  
  pinMode(pin.interrupt0, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin.interrupt0), coinInserted, CHANGE);//RISING);

  pinMode(pin.interrupt1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin.interrupt1), coinInserted, CHANGE);
  
  pinMode(pin.rele0, OUTPUT);
  digitalWrite(pin.rele0, HIGH);
  
  myservo.attach(pin.stepper);
}

void coinInserted()    
//The function that is called every time it recieves a pulse
{
  //Serial.println("Coin insered");
  coin.value = coin.value + 0.05;  
//As we set the Pulse to represent 5p or 5c we add this to the coinsValue
  coin.isChanged = true;                           
//Flag that there has been a coin inserted
}

void loop()
{
  if(coin.isChanged)//Check if a coin has been Inserted
  {
    coin.isChanged = false;  
    //Serial.print("Credit: £");
    //Serial.println(coinsValue);
    for(int pos = 0; pos < 90; ++pos)
    {
      myservo.write(pos);
      delay(time_t.servo);
    }
    
    digitalWrite(pin.rele0, LOW);  
    delay(time_t.cottura);  
    digitalWrite(pin.rele0, HIGH);
    //Serial.println("FINISH");
  }
}
