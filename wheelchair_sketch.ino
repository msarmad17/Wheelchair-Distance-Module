const byte hPin1 = 2; // Hall effect sensor 1
const byte hPin2 = 3; // Hall effect sensor 2

int count = 0;
volatile int rState, lState, rTicks, lTicks = 0; // 0 or 1 depending on proximity of magnet
double rOdo, lOdo = 0;

#include <math.h>
#define RADIUS 11.1875
#define RES 36
#define SEP 15
#define RAD_TO_DEG 57.295779513

const double res_angle = (2*PI)/RES;
const double res_inches = RADIUS*res_angle;


void setup() {

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2), rInterrupt, RISING); // Can also use CHANGE
  attachInterrupt(digitalPinToInterrupt(3), lInterrupt, RISING); // Can also use CHANGE

}

 

void loop() {
  count++;
  //Serial.print("Loop ");
  //Serial.println(count);
  
  int roTicks, loTicks, iroTicks, iloTicks, rChange, lChange = 0;
  double angle = 0;
  
  do{
    iroTicks = rTicks;
    iloTicks = lTicks;
    delay(2000);
    rChange = rTicks - iroTicks;
    lChange = lTicks - iloTicks;
    roTicks += rChange;
    loTicks += lChange;
  }
  while(rChange != lChange);
  
  if((roTicks == loTicks) || (roTicks*loTicks == 0))
    //Serial.println("Zero degrees turned");
    return;
  else{
    if(roTicks < loTicks){
      angle = getAngle(roTicks, loTicks);
      Serial.print(angle);
      Serial.println(" degrees turned to the left");
    }
    else{
      angle = getAngle(loTicks, roTicks);
      Serial.print(angle);
      Serial.println(" degrees turned to the right");
    }
  }
  Serial.print(roTicks);
  Serial.print(", ");
  Serial.println(loTicks);
  Serial.println();
}

double getAngle(int T1, int T2){
  double S1 = T1*res_inches;
  double S2 = T2*res_inches;
  
  double ang = RAD_TO_DEG*(S1 / ((SEP*S1)/(S2 - S1)));
  return ang;
} 

void rInterrupt() { // was pin_ISR

  rState = digitalRead(hPin2);
  //Serial.print("Sensor: ");
  //Serial.println(hState);
  
  rTicks++; // Magnet was sensed, increase the tick counters);
  Serial.print(rTicks);
  Serial.print(",");
  Serial.println(lTicks);
  
  rOdo += res_inches;
  Serial.print(rOdo);
  Serial.println(" inches on right wheel");

}

void lInterrupt() { // was pin_ISR

  lState = digitalRead(hPin1);
  //Serial.print("Sensor: ");
  //Serial.println(hState);
  
  lTicks++; // Magnet was sensed, increase the tick counters);
  Serial.print(rTicks);
  Serial.print(",");
  Serial.println(lTicks);
  
  lOdo += res_inches;
  Serial.print(lOdo);
  Serial.println(" inches on left wheel");

}

 

