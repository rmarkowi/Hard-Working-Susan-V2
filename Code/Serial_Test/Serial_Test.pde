import processing.serial.*;
Serial arduino;

void setup(){
  arduino = new Serial(this, "COM6", 9600);
  //arduino.bufferUntil(10);
  delay(2000);
}
  
void draw(){
  arduino.write("p0");
  delay(10);
  while(!waitForArduino("done"));
  delay(1000);
}
boolean waitForArduino(String toLookFor){
  if(arduino.available() > 0){
    String readString = arduino.readString();
    println(readString);
    if(readString.contains(toLookFor)){
      arduino.clear();
      return true;
    }
  }
  return false;
}