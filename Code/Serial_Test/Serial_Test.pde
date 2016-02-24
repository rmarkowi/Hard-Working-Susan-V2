import processing.serial.*;
Serial arduino;

void setup(){
  arduino = new Serial(this, "COM4", 9600);
  //arduino.bufferUntil(10);
  while(!waitForArduino("start")){}
  arduino.write("p100");
  while(!waitForArduino("done")){}
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