/* read a rotary encoder with interrupts
   Encoder hooked up with common to GROUND,
   encoder0PinA to pin 2, encoder0PinB to pin 4 (or pin 3 see below)
   it doesn't matter which encoder pin you use for A or B  

   uses Arduino pullups on A & B channel outputs
   turning on the pullups saves having to hook up resistors 
   to the A & B channel outputs 

*/ 

#define encoder0PinA  2
#define encoder0PinB  4

const int motorBkwd =  10;
const int motorFwd = 9;

int readTimeout = 10;
int drivePercent = 0;

volatile unsigned int encoder0Pos = 0;

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
    }
    else if(drivePercent < 0 && drivePercent >= -100){
      analogWrite(motorFwd, 0);
      analogWrite(motorBkwd, abs(drivePercent));
    }
    else if(drivePercent == 0){
      analogWrite(motorFwd, 0);
      analogWrite(motorBkwd, 0);
    }
  }
}

void doEncoder() {
  /* If pinA and pinB are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   *
   * For more information on speeding up this process, see
   * [Reference/PortManipulation], specifically the PIND register.
   */
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }

  Serial.println (encoder0Pos, DEC);
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
