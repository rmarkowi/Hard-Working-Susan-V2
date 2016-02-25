void setup() {
  Serial.begin (9600);
  Serial.println("start");
}

void loop() {
  if(Serial.available() > 0){
    if(Serial.readString().equals("p0")){
      delay(100);
      Serial.println("done");
    }
  }
}
