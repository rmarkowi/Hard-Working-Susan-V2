#include <Time.h>
#include <TimeLib.h>

/* Send a "s" followed by a value between -100 and 100 to turn motor
 *  Encoder sends values whenever they change between 0 and 65535
 */

#define encoder0PinA  2
#define encoder0PinB  4

const int motorBkwd =  10;
const int motorFwd = 9;

int readTimeout = 10;
int drivePercent = 0;

boolean isForward = false;

volatile unsigned int encoder0Pos = 0;

float lastPos = 0;
float lastTime = 1; 
float lastSpeed = 1;
float accel = 0;

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
  Serial.setTimeout(readTimeout);
  Serial.println("start");                // a personal quirk

} 

void loop(){
  String input;
  if (Serial.available() > 0){
    input = (Serial.readString());
    drivePercent = input.toInt();
    if(drivePercent > 0 && drivePercent <= 100){
      analogWrite(motorFwd, drivePercent);
      analogWrite(motorBkwd, 0);
      isForward = true;
    }
    else if(drivePercent < 0 && drivePercent >= -100){
      analogWrite(motorFwd, 0);
      analogWrite(motorBkwd, abs(drivePercent));
      isForward = false;
    }
    else if(drivePercent == 0){
      analogWrite(motorFwd, 100);
      analogWrite(motorBkwd, 100);
    }
  }
}

void doEncoder() {
  float currentTime = millis();
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }
  if(encoder0Pos == 400){
    encoder0Pos = 0;
  }
  else if(encoder0Pos == 65135){
    encoder0Pos = 0;
  }
  Serial.print(encoder0Pos);
}
/* See this expanded function to get a better understanding of the
 * meanings of the four possible (pinA, pinB) value pairs:
 */
void doEncoder_Expanded(){
  if (digitalRead(encoder0PinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder0PinB) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      encoder0Pos = encoder0Pos - 1;         // CCW
    } 
    else {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  { 
    if (digitalRead(encoder0PinB) == LOW) {   // check channel B to see which way
                                              // encoder is turning  
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }

  }
  Serial.println (encoder0Pos, DEC);          // debug - remember to comment out
                                              // before final program run
  // you don't want serial slowing down your program if not needed
}
/*  to read the other two transitions - just use another attachInterrupt()
in the setup and duplicate the doEncoder function into say, 
doEncoderA and doEncoderB. 
You also need to move the other encoder wire over to pin 3 (interrupt 1). 
*/ 
