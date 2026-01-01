#include <AbsMouse.h> 

#include <math.h> 

uint8_t pinstate=1, KeyHit=0, seq=0, faderactive=0;  

int preva0;

uint32_t atime=0,v,fp;


  

void setup() { 

  AbsMouse.init(1920,1080); 

  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);

  pinMode(A0, INPUT);

  delay(1000); // Let USB settle 

} 

  

void loop() { 

  int a0;

  if (digitalRead(12)==0)
  {
     if (pinstate==0)
     {
        pinstate=1;
        KeyHit=1;
     }
  }
  else pinstate=0;

  if (digitalRead(11)==0)
  {
     if (pinstate==0)
     {
        pinstate=2;
        KeyHit=2;
     }
  }
  else pinstate=0;

  if (digitalRead(10)==0)
  {
     if (pinstate==0)
     {
        pinstate=4;
        KeyHit=4;
     }
  }
  else pinstate=0;


  if (KeyHit!=0)
  {
    /*seq++; if (seq>3) seq=0;
    if (seq==0) AbsMouse.move(10, 10); 
    if (seq==1) AbsMouse.move(1900, 10); 
    if (seq==2) AbsMouse.move(1900, 1000); 
    if (seq==3) AbsMouse.move(10, 1000); */
    if (KeyHit==1) AbsMouse.move(1498, 1012); //go
    if (KeyHit==2) AbsMouse.move(1461, 961);  //cue
    if (KeyHit==4) AbsMouse.move(1391, 854);  //back
    AbsMouse.press(MOUSE_LEFT);
    KeyHit=0;
    delay(100); 
    AbsMouse.release(MOUSE_LEFT);
    faderactive=0;
    delay(100); 
  }

  a0=analogRead(A0);
  if (abs(a0-preva0)>20)
  {
     preva0=a0;
     if (faderactive==0) fp=a0; //this is a new move starting. Remember where we started off from
     atime=millis();
     v=932+((1022-966)*2*(fp-a0)/1024);
     AbsMouse.move(1361, v);
     if (faderactive==0) 
     {
         faderactive=1;
         AbsMouse.press(MOUSE_LEFT);
     }
  }

  if ((faderactive!=0)&&((millis()-atime)>250))
  {
      AbsMouse.release(MOUSE_LEFT);
      faderactive=0;
  }

  /*
  // Calculate position in polar coordinates 

  int dx = int(radius * cos(angle)); 

  int dy = int(radius * sin(angle)); 

  

  // Move the mouse 

  AbsMouse.move(dx, dy); 

  

  // Advance angle 

  angle += angleStep; 

  

  // Adjust radius 

  if (expanding) { 

    radius += radiusStep; 

    if (radius >= maxRadius) expanding = false; 

  } else { 

    radius -= radiusStep; 

    if (radius <= minRadius) expanding = true; 

  } 
  */

  delay(10); 

} 
