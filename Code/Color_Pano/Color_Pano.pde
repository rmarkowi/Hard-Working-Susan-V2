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
int colorBalanceThreshold = 6;

void setup(){
  loadPixels();
  size(1920, 1080);
  println("Starting");
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
  boolean portFound = false;
  String[] ports = Serial.list();
  if(ports.length == 0){
    println("No 'Duino!");
    exit();
  }
  boolean arduinoFound = false;
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
  arduino.write("s100");
  while(!compareColors(get(homeCenter[0], homeCenter[1]), homeColor, colorBalanceThreshold)){}
  arduino.write("h");
}

boolean compareColors(color colOne, color colTwo, int threshold){
  float[] colors = {red(colOne), green(colOne), blue(colOne), red(colTwo), green(colTwo), blue(colTwo)};
  for(int i = 0; i < (colors.length / 2); i++){
    if((colors[i] < colors[i + 2] - threshold) || (colors[i] > colors[i + 2] + threshold)){
      println("Color 1: " + hex(colOne) + ", Color 2: " + hex(colTwo) + ", not the same");
      return false;
    }
  }
  println("Color 1: " + hex(colOne) + ", Color 2: " + hex(colTwo) + ", THE SAME");
  return true;
}