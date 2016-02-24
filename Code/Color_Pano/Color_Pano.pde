import processing.serial.*;
import processing.video.*;

boolean debug = false;

Capture camera;
String[] camForUse = {"name=Logitech HD Webcam C615,size=1920x1080,fps=30", 
                      "name=Lenovo EasyCamera,size=1280x720,fps=30"};
                      
Serial arduino;
String[] portForUse = {"COM4", "COM3", "COM6"};

color homeColor;
int[] homeCenter = new int[2];
int colorBalanceThreshold = 20;
Table savedHomeVals;

int maxPano = 400;
int panoStep = 40;

void setup(){
  println("Starting");
  loadPixels();
  size(1920, 1080);
  
  println("Looking for Cameras");
  String[] cams = Capture.list();
  if(cams.length == 0){
    println("No Cameras!");
    exit();
  }
  boolean camFound = false;
  for(String camToUse : camForUse){
    println("Trying to connect to " + camToUse);
    for(String cam : cams){
      if(cam.equals(camToUse)){
        camera = new Capture(this, cam);
        camera.start();
        println("Starting camera " + camToUse);
        camFound = true;
        break;
      }
    }
    if(camFound){
      break;
    }
  }
  
  println("Looking for Arduino");
  boolean portFound = false;
  String[] ports = Serial.list();
  if(ports.length == 0){
    println("No 'Duino!");
    exit();
  }
  for(String portToUse : portForUse){
    println("Trying to connect to " + portToUse);
    for(String port : ports){
      if(port.equals(portToUse)){
        arduino = new Serial(this, port, 9600);
        println("Starting arduino on " + portToUse);
        portFound = true;
        break;
      }
    }
    if(portFound){
      break;
    }
  }
  arduino.clear();
  println("Serial Pause");
  delay(3000);
  println("Wait for Handshake");
  while(!waitForArduino("start")){}
  arduino.write("0");
}

void draw(){
  if(debug){
    if(camera.available()){
      camera.read();
    }
    image(camera, 0, 0);
  }
}

void home(){
  if(debug){
    arduino.write("s100");
    delay(2000);
    while(!compareColors(get(homeCenter[0], homeCenter[1]), homeColor, colorBalanceThreshold)){
      if(camera.available()){
        camera.read();
      }
      image(camera, 0, 0);
    }
    arduino.write("h");
    delay(2000);
    println("Drive to Home");
    arduino.write("p-90");
    while(!waitForArduino("done")){}
  }
}

boolean compareColors(color colOne, color colTwo, int threshold){
  float[] colors = {red(colOne), green(colOne), blue(colOne), red(colTwo), green(colTwo), blue(colTwo)};
  for(int i = 0; i < (colors.length / 2); i++){
    if((colors[i] < colors[i + 2] - threshold) || (colors[i] > colors[i + 2] + threshold)){
      println("Color 1: " + hex(colOne) + ", Color 2: " + hex(colTwo) + ", not the same");
      return false;
    }
  }
  return true;
}

int panoPosition = 0;
void makePano(){
  String command = "p" + str(panoPosition);
  println(command);
  arduino.write(command);
  delay(2000);
  while(!waitForArduino("done")){}
  println("Taking Photo");
  if(panoPosition <maxPano){
    panoPosition += panoStep;
    makePano();
  }
  else{
    println("Done Making Pano");
  }
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