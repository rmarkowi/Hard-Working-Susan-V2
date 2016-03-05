#include <Time.h>
#include <TimeLib.h>

/* Send a "s" followed by a value between -100 and 100 to turn motor by speed
 *  Send a "p" followed by a value between -399 and 399 to move to a position
 *  Send a "h" to set the current position to home
 *  Send a "g" to get the current Position, returned as an int + "\n"
 *  Returns a "done\n" when a p-move is complete
 */

#define encoder0PinA  2
#define encoder0PinB  4

const int motorBkwd =  10;
const int motorFwd = 9;

int mSpeed = 0;
int pos = 0;

int controller_I = 0;
int last_err = 0;

boolean isForward = false;

volatile int encoder0Pos = 0;
float lastTime;
float lastPos;
char input[6];

void setup() { 
  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  
  pinMode(motorFwd, OUTPUT);
  pinMode(motorBkwd, OUTPUT);
  analogWrite(motorFwd, 0);
  analogWrite(motorBkwd, 0);

  attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
  Serial.begin (9600);
  Serial.println("start");
  lastTime = millis();
} 

void loop(){
  if (Serial.available() > 0){
    (Serial.readString()).toCharArray(input, sizeof(input));
    if(input[0] == 's'){
      String percent = String(input);
      mSpeed = (percent.substring(1)).toInt();
      doDrive();
    }
    else if(input[0] == 'p'){
      String percent = String(input);
      pos = (percent.substring(1)).toInt();
    }
    else if(input[0] == 'h'){
      mSpeed = 0;
      doDrive();
      encoder0Pos = 0;
      Serial.println("done");
    }
    else if(input[0] == '0'){
      mSpeed = 0;
      doDrive();
    }
    else if(input[0] = 'g'){
      if(encoder0Pos <= -400){
        encoder0Pos = 0 - (encoder0Pos % 400);
      }
      else{
        encoder0Pos %= 400;
      }
      Serial.println(String(encoder0Pos));
    }
  }
  doMove();
}

void doEncoder() {
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos++;
  } 
  else {
    encoder0Pos--;
  }
  Serial.println(encoder0Pos);
}

void doDrive(){
  mSpeed = map(mSpeed, -100, 100, -255, 255);
  if(mSpeed > 0 && mSpeed <= 255){
    analogWrite(motorFwd, mSpeed);
    analogWrite(motorBkwd, 0);
    isForward = true;
  }
  else if(mSpeed < 0 && mSpeed >= -255){
    analogWrite(motorFwd, 0);
    analogWrite(motorBkwd, abs(mSpeed));
    isForward = false;
  }
  else if(mSpeed == 0){
    analogWrite(motorFwd, 255);
    analogWrite(motorBkwd, 255);
    isForward = false;
  }
}

void doMove(){
  if(pos < 0){
    pos = 0 - (pos % 400);
  }
  else{
    pos %= 400;
  }
  /*
  if((encoder0Pos - pos < 0)){
    mSpeed = 100;
  }
  else if(encoder0Pos - pos > 0){
    mSpeed = -100;
  }
  else{
    mSpeed = 0;
    if(encoder0Pos <= -400){
      encoder0Pos = -1 * (encoder0Pos % 400);
    }
    else{
      encoder0Pos %= 400;
    }
  }*/
  int err = pos - encoder0Pos;
  controller_I += err;
  int D = err - last_err;
  last_err = err;

  const int P_GAIN = 1;
  const int I_DIVISOR = 25;
  const int D_GAIN = 25;

  int tspeed = P_GAIN*err +  D_GAIN*D + (controller_I / I_DIVISOR);
  // +
  
  mSpeed = min(100, max(-100, tspeed));
  doDrive();
  delay(100);
  /*if(encoder0Pos != pos){
    doMove(pos);
  }*/
}

float getSpeed(){
  float currentTime = millis();
  float currentPos = encoder0Pos;
  float currentSpeed;
  if(currentTime != lastTime){
    currentSpeed = ((abs(currentPos - lastPos)) / (abs(currentTime - lastTime)));
    lastTime = currentTime;
    lastPos = currentPos;
  }
  return currentSpeed;
}

