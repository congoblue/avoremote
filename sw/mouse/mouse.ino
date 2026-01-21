#include <AbsMouse.h> 

#include <math.h> 

uint8_t pinstate=1, KeyHit=0, LastKey, seq=0, faderactive=0;  

uint8_t enca, encb, encc;

int preva0;

uint32_t atime=0,v,fp;

uint32_t KeyState=0, kr=0;


#define AN1 A6
#define AN0 A7
#define ROT3B A4
#define ROT3A A5
#define SWCOM1 A3
#define SWCOM2 A2
#define SWCOM3 A1
#define SW1 D2
#define SW2 D3
#define SW3 D4
#define SW4 D5
#define SW5 D6
#define SW6 D7
#define SW7 D8
#define SW8 D9
#define ROT1A D10
#define ROT1B D11
#define ROT2A D12
#define ROT2B D13
  

void setup() { 

  //AbsMouse.init(1920,1080); 

  Serial.begin(115200);

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(SW6, INPUT_PULLUP);
  pinMode(SW7, INPUT_PULLUP);
  pinMode(SW8, INPUT_PULLUP);

  pinMode(ROT1A, INPUT);
  pinMode(ROT1B, INPUT);
  pinMode(ROT2A, INPUT);
  pinMode(ROT2B, INPUT);
  pinMode(ROT3A, INPUT);
  pinMode(ROT3B, INPUT);

  digitalWrite(SWCOM1,1);
  digitalWrite(SWCOM2,1);
  digitalWrite(SWCOM3,1);

  pinMode(SWCOM1, OUTPUT);
  pinMode(SWCOM2, OUTPUT);
  pinMode(SWCOM3, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(AN0, INPUT); //fader 1
  pinMode(AN1, INPUT); //fader 2

  delay(1000); // Let USB settle 

  Serial.print("Avoremote\r\n");

} 

void EncoderISR(void)
{
  static char prevenc=0x0F;
  char nowenc=(PIND>>2)&0x0F;
  char diff=prevenc^nowenc;

  if (diff&0x01)
  {
    if (digitalRead(ROT1B)==0)
    {    
      if (digitalRead(ROT1A)==0)enca++; else enca--;
    }
    else
    {    
      if (digitalRead(ROT1A)==0)enca--; else enca++;
    }    
  }
  if (diff&0x02)
  {   
    if (digitalRead(ROT1A)==0)
    {    
      if (digitalRead(ROT1B)==0)enca--; else enca++;
    }
    else
    {    
      if (digitalRead(ROT1B)==0)enca++; else enca--;
    }    
  }
  
  
  if (diff&0x04)
  {
    if (digitalRead(ENC2B)==0)
    {    
      if (digitalRead(ENC2A)==0)encb++; else encb--;
    }
    else
    {    
      if (digitalRead(ENC2A)==0)encb--; else encb++;
    }    
  }
  if (diff&0x08)
  {   
    if (digitalRead(ENC2A)==0)
    {    
      if (digitalRead(ENC2B)==0)encb--; else encb++;
    }
    else
    {    
      if (digitalRead(ENC2B)==0)encb++; else encb--;
    }    
  }
  prevenc=(PIND>>2)&0x0F;
}

uint8_t SwRead(void)  
{
   uint8_t v=0;
   if (digitalRead(SW1)) v|=1;
   if (digitalRead(SW2)) v|=2;
   if (digitalRead(SW3)) v|=4;
   if (digitalRead(SW4)) v|=8;
   if (digitalRead(SW5)) v|=16;
   if (digitalRead(SW6)) v|=32;
   if (digitalRead(SW7)) v|=64;
   if (digitalRead(SW8)) v|=128;
   return v;
}

void loop() { 

  uint32_t a0,v,i;
  static uint8_t ld=0;
  static uint8_t ks=0;

  //key scan
  if (ks==0) 
  {
    kr&=0xFFFF00;
    kr|=(~SwRead())&0xFF;
    ks=1;
    digitalWrite(SWCOM1,1);
    digitalWrite(SWCOM2,0);
  }
  else if (ks==1) 
  {
    kr&=0xFF00FF;
    kr|=((~SwRead())<<8)&0x00FF00;
    ks=2;
    digitalWrite(SWCOM2,1);
    digitalWrite(SWCOM3,0);
  }
  else if (ks==2) 
  {
    kr&=0x00FFFF;
    kr|=((~SwRead())<<16)&0xFF0000;
    ks=0;
    digitalWrite(SWCOM3,1);
    digitalWrite(SWCOM1,0);

    //check if anything has changed
    if (kr!=KeyState)
    {
       if (kr>KeyState) //newly pressed
       {
         KeyHit=1;
         v=kr^KeyState;
         for (i=0; i<24; i++) if (v&(1<<i)) LastKey=i;
       }
       KeyState=kr;
       
       //Serial.print(KeyState,HEX);
    }
  }
  

   if (KeyHit!=0)
   {
      KeyHit=0;
      Serial.println(LastKey);
   }

   ld++; 
   if (ld==50) {digitalWrite(LED_BUILTIN,1);}
   if (ld>=100) 
   {
      ld=0; 
      digitalWrite(LED_BUILTIN,0); 
   }

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

  
  

  delay(10); 

} 
