void mouseClicked(){
  if(debug){
    homeColor = get(mouseX, mouseY);
    homeCenter[0] = mouseX;
    homeCenter[1] = mouseY;
    println("mouseX: " + mouseX + ", mouseY: " + mouseY + ", color: " + hex(homeColor));
  }
}

void keyPressed(){
  if(key == 'd'){
    debug = !debug;
    println(debug);
  }
  else if(key == 27){
    exit();
  }
  else if(key == 'h'){
    home();
  }
}