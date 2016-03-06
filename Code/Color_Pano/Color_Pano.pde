import processing.serial.*;
import processing.video.*;

Capture camera;
String[] camForUse = {"name=Logitech HD Webcam C615,size=1920x1080,fps=30", 
                      "name=Lenovo EasyCamera,size=1280x720,fps=30"};
                      
Serial arduino;
String[] portForUse = {"COM4", "COM3", "COM6"};

String posString = "";
int currentArduinoPos = 0;

int captureW = 20;
int captureH = 1080;
int panoLength = 400;
int panoStep = 20;

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
  println("Serial Pause");
  delay(3000);
  arduino.clear();
}

void draw(){/*
  for(int i = 0; i < 10; i ++){
    if(camera.available()){
      camera.read();
    }
    PImage temp = createImage(1920, 1080, ARGB);
    temp.copy(camera, 0, 0, 1920, 1080, 0, 0, 1920, 0);
    temp.save("data/img_" + i + ".jpg");
    delay(2000);
  }*/
  while(!makePano());
}

void getArduinoPos(){
  if(arduino.available() > 0){
    int posByte = arduino.read();
    if(posByte != 10 && posByte != 13){
      posString += char(posByte);
    }
    else if(posByte == 13){
      try{
        currentArduinoPos = Integer.parseInt(posString);
      }
      catch(Exception e){}
      finally{
        posString = "";
      }
    }
  }
}

boolean makePano(){
  getArduinoPos();
  int panoPos = 0;
  while(panoPos < panoLength){
    arduino.write("p" + panoPos);
    while(currentArduinoPos < panoPos){
      println(panoPos);
      getArduinoPos();
    }
    PImage camImage;
    PImage tempImage;
    camImage = createImage(camera.width, camera.height, ARGB);
    tempImage = createImage(captureW, captureH, ARGB);
    int sx = (camera.width / 2) - (captureW / 2);
    if(camera.available()){
      camera.read();
    }
    camImage = camera.get();
    tempImage.copy(camImage, sx, 0, captureW, captureH, 0, 0, captureW, captureH);
    String fileName = ("data/photo_" + panoPos + ".jpg");
    tempImage.save(fileName);
    panoPos += panoStep;
  }
  int textureW = (panoLength / panoStep) * captureW;
  int textureH = 1080;
  PImage texture = createImage(textureW, textureH, ARGB);
  for(int panoNum = 0; panoNum < (panoLength / panoStep); panoNum += panoStep){
    String imageToLoad = "data/photo_" + panoNum + ".jpg");
    PImage tempImage = loadImage(imageToLoad);
    int dx = panoNum * captureW;
    texture.copy(tempImage, 0,0, captureW, captureH, dx, 0, captureW, captureH);
  }
  texture.save("data/texture.jpg");
  return true;
}