#include "TM1637.h"//Dysplay TM1637 library
#define CLK 3//Display clk pin
#define DIO 2//Display dio pin
int8_t DispMSG[4] = {};
unsigned long dispTimer;
TM1637 tm1637(CLK, DIO);//init dysplay

#include <Adafruit_Sensor.h>//DHT sensor lib
#include "DHT.h"//DHT sensor lib
#define dhtPin 6//Sensor pin
unsigned long timer;
DHT dht(dhtPin, DHT11);//init sensor

void setup() {
  Serial.begin(9600);//serial port open for data output(not nessesary)
  pinMode(12, OUTPUT);//Incorrect work indicator
  pinMode(13, OUTPUT);//Correct work indicator
  dht.begin();
  
  //Module init
  tm1637.init();
  //display bright
  /*
     BRIGHT_TYPICAL = 2 middle
     BRIGHT_DARKEST = 0 dark
     BRIGHTEST = 7      bright
  */
  tm1637.set(BRIGHT_TYPICAL);//set bright
}


void loop() {
  if(millis()-timer > 1000){
    float h = dht.readHumidity(); //humidity check
    float t = dht.readTemperature(); //temperature check
    
    if (isnan(h) || isnan(t)) {  // data correct check
      Serial.println("Read error");
      digitalWrite(12, HIGH);//incorrect work indicator - on
      digitalWrite(13, LOW);
      return;
    }else{
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);//correct work indicator - on
    }
    
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C "); //Serial port - output data

    int tDisp = dht.readTemperature();
    int hDisp = dht.readHumidity();
    int digitoneT = tDisp / 10;
    int digittwoT = tDisp % 10;
    int digitoneH = hDisp / 10;
    int digittwoH = hDisp % 10;//data cast

//PROGRAMM DELAY first method:
    
//    tm1637.display(1, digitoneT);
//    tm1637.display(2, digittwoT);
//    tm1637.display(3, 12); //  C
//    delay (5000);
//    tm1637.display(1, digitoneH);
//    tm1637.display(2, digittwoH);
//    tm1637.display(3, 17); //  H
//    delay (5000);

//PROGRAMM DELAY second method:

    if(millis()-dispTimer > 3000){
      if(millis()/1000 % 2){
        tm1637.display(1, digitoneT);
        tm1637.display(2, digittwoT);
        tm1637.display(3, 12); //  C
      }else{
        tm1637.display(1, digitoneH);
        tm1637.display(2, digittwoH);
        tm1637.display(3, 15); //  H
      }
      dispTimer = millis();
    }
    

    timer = millis();
  }
}










