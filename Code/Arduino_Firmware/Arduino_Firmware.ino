/* Software to control the Hard Working Susan
 * Created by Ruben Markowitz
 * 
 * Contol: Send over serial desired motor speed
 * % between -100 and 100
 */
const int motorBkwd =  5;
const int motorFwd = 3;
const int motorMode = 2;

const int encoderIn1 = 13;
const int encoderIn2 = 12;

int readTimeout = 10;
int drivePercent = 0;

int currentEncoder1State;
int currentEncoder2State;
int driveLocation = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(readTimeout);
  pinMode(motorMode, OUTPUT);
  pinMode(motorFwd, OUTPUT);
  pinMode(motorBkwd, OUTPUT);
  digitalWrite(motorMode, LOW);
  analogWrite(motorFwd, 0);
  analogWrite(motorBkwd, 0);

  pinMode(encoderIn1, INPUT);
  pinMode(encoderIn2, INPUT);
  currentEncoder1State = digitalRead(encoderIn1);
  currentEncoder2State = digitalRead(encoderIn2);
  
}

void loop() {
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
