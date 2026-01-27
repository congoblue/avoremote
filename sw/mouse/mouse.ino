#include <AbsMouse.h> 
#include <EEPROM.h>
#include <math.h> 

uint8_t pinstate=1, KeyHit=0, LastKey, seq=0, faderactive0=0, faderactive1=0;  

int32_t Enca, Encb, Encc;
int32_t timea=0,timeb=0,timec=0;
uint8_t EncaChange, EncbChange, EnccChange;
uint8_t EncaNew, EncbNew, EnccNew;

int preva0, preva1, encoffset, Swiggle;

uint32_t atime0=0,atime1=0,v,fp;

uint32_t KeyState=0, kr=0, SetupKd=0;

uint8_t mousedn=0, SetupMode=0;

uint32_t xp[32],yp[32];


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

#define ENCODER_TIMEOUT 500
#define FADER_TIMEOUT 250  
  

void setup() { 

  AbsMouse.init(1920,1080); 

  Serial.begin(115200);

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(SW6, INPUT_PULLUP);
  pinMode(SW7, INPUT_PULLUP);
  pinMode(SW8, INPUT_PULLUP);

  pinMode(ROT1A, INPUT_PULLUP);
  pinMode(ROT1B, INPUT_PULLUP);
  pinMode(ROT2A, INPUT_PULLUP);
  pinMode(ROT2B, INPUT_PULLUP);
  pinMode(ROT3A, INPUT_PULLUP);
  pinMode(ROT3B, INPUT_PULLUP);

  digitalWrite(SWCOM1,1);
  digitalWrite(SWCOM2,1);
  digitalWrite(SWCOM3,1);

  pinMode(SWCOM1, OUTPUT);
  pinMode(SWCOM2, OUTPUT);
  pinMode(SWCOM3, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(AN0, INPUT); //fader 1
  pinMode(AN1, INPUT); //fader 2

  if (EEPROM.read(0)==0xAA) //eeprom is initialised
  {
    for (int i=0; i<32; i++)
    {
      xp[i]=EEPROM.read(i*4+1)+256*EEPROM.read(i*4+2);
      yp[i]=EEPROM.read(i*4+3)+256*EEPROM.read(i*4+4);
    }
  }
  else
  {
    for (int i=0; i<32; i++)
    {
      xp[i]=10+i*20;
      yp[i]=10;
    }
  }

  delay(1000); // Let USB settle 

  Serial.print("Avoremote\r\n");

} 

void EncoderPoll(void)
{
  static uint8_t prevenc=0x00;
  int8_t enca=0, encb=0, encc=0;
  uint8_t nowenc=digitalRead(ROT1A)|(digitalRead(ROT1B)<<1)|(digitalRead(ROT2A)<<2)|(digitalRead(ROT2B)<<3)|(digitalRead(ROT3A)<<4)|(digitalRead(ROT3B)<<5);

  char diff=prevenc^nowenc;

  if (diff&1)
  {
    if (prevenc&1)  //falling edge only
    {
        
        if (nowenc&2) enca=+1; else enca=-1;
        if (timea!=0) {Enca=(millis()-timea)*enca; EncaChange=1;} else EncaNew=1;
        timea=millis();
    }
  }

  if (diff&4)
  {
    if (prevenc&4) 
    {
        
        if (nowenc&8) encb=+1; else encb=-1;
        if (timeb!=0) {Encb=(millis()-timeb)*encb; EncbChange=1;} else EncbNew=1;
        timeb=millis();
    }
  }

  if (diff&16)
  {
    if (prevenc&16) 
    {
        
        if (nowenc&32) encc=+1; else encc=-1;
        if (timec!=0) {Encc=(millis()-timec)*encc; EnccChange=1;} else EnccNew=1;
        timec=millis();
    }
  }


  prevenc=nowenc;
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

void StorePos(uint8_t key)
{
  xp[key]=(analogRead(AN0)*1920)/1024;
  yp[key]=(analogRead(AN1)*1080)/1024;
  if (EEPROM.read(0)!=0xAA) EEPROM.write(0,0xAA); //make sure eeprom flagged as init
  EEPROM.write(key*4+1,xp[key]&0xFF); EEPROM.write(key*4+2,(xp[key]>>8)&0xFF);
  EEPROM.write(key*4+3,yp[key]&0xFF); EEPROM.write(key*4+4,(yp[key]>>8)&0xFF);
}

const uint8_t encchange[25]={
1,1,1,1,2,2,2,2,3,3, 3,4,4,4,5,5,6,6,7,8, 9,10,12,15,20
};

void loop() { 

  static uint8_t x=0;
  uint32_t a0,a1,v,i;
  static uint8_t ld=0;
  static uint8_t ks=0;

  x++;

  EncoderPoll();

  if (EncaChange)
  {
    EncaChange=0;
    if (SetupMode==0)
    {
      if (EncaNew)
      {
        EncaNew=0;
        if (mousedn!=0) {AbsMouse.release(MOUSE_LEFT); delay(100); faderactive0=0; faderactive1=0; timeb=0; timec=0;}
        AbsMouse.move(xp[24],yp[24]); 
        AbsMouse.press(MOUSE_LEFT);
        encoffset=0;
        mousedn=1;
      }
      else
      {
        //Serial.println(Enca);
        if (abs(Enca)>100) v=1; else v=encchange[(100-abs(Enca))/4];
        if (Enca>0) 
        {
          encoffset+=v; 
        }
        else 
        {
          if (encoffset+xp[24]>v) encoffset-=v; else encoffset=xp[24];
        }      
        AbsMouse.move(xp[24]+encoffset,yp[24]); 
      }
    }
    else
    {
       StorePos(24);
    }

  }

  if (EncbChange)
  {
    EncbChange=0;
    if (SetupMode==0)
    {
      if (EncbNew)
      {
        EncbNew=0;
        if (mousedn!=0) {AbsMouse.release(MOUSE_LEFT); delay(100); faderactive0=0; faderactive1=0; timea=0; timec=0;}
        AbsMouse.move(xp[25],yp[25]); 
        AbsMouse.press(MOUSE_LEFT);
        encoffset=0;
        mousedn=1;
      }
      else
      {
        if (abs(Encb)>100) v=1; else v=encchange[(100-abs(Encb))/4];
        if (Encb>0) 
        {
          encoffset+=v; 
        }
        else 
        {
          if (encoffset+xp[25]>v) encoffset-=v; else encoffset=xp[25];
        }      
        AbsMouse.move(xp[25]+encoffset,yp[25]); 
      }
    }
    else
    {
       StorePos(25);
    }

  }

  if (EnccChange)
  {
    EnccChange=0;
    if (SetupMode==0)
    {
      if (EnccNew)
      {
        EnccNew=0;
        if (mousedn!=0) {AbsMouse.release(MOUSE_LEFT); delay(100); faderactive0=0; faderactive1=0; timea=0; timeb=0;}
        AbsMouse.move(xp[26],yp[26]); 
        AbsMouse.press(MOUSE_LEFT);
        encoffset=0;
        mousedn=1;
      }
      else
      {
        if (abs(Encc)>100) v=1; else v=encchange[(100-abs(Encc))/4];
        if (Encc>0) 
        {
          encoffset+=v; 
        }
        else 
        {
          if (encoffset+xp[26]>v) encoffset-=v; else encoffset=xp[26];
        }      
        AbsMouse.move(xp[26]+encoffset,yp[26]); 
      }
    }
    else
    {
       StorePos(26);
    }

  }


  if (timea!=0) if ((millis()-timea)>ENCODER_TIMEOUT) 
  {
    timea=0;
    AbsMouse.release(MOUSE_LEFT);
    mousedn=0;
  }
  if (timeb!=0) if ((millis()-timeb)>ENCODER_TIMEOUT) 
  {
    timeb=0;
    AbsMouse.release(MOUSE_LEFT);
    mousedn=0;
  }
  if (timec!=0) if ((millis()-timec)>ENCODER_TIMEOUT) 
  {
    timec=0;
    AbsMouse.release(MOUSE_LEFT);
    mousedn=0;
  }

  if (x>=10)
  {
    x=0;

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

  //check for entering setup mode
  if (KeyState==0x80)
  {
     if (SetupKd!=0)
     {
        if (millis()-SetupKd>10000) 
        {
          if (SetupMode==0) 
          {
            SetupMode=1; 
            Serial.println("Start Setup mode");
            SetupKd=0;
          }
          else 
          {
            SetupMode=0;
            Serial.println("End Setup mode");
            SetupKd=0;
            AbsMouse.move(10,10); 
          }
        }
     }
     else 
     {
        SetupKd=millis();
     }
  }
  else
     SetupKd=0;
  
  //diagnostic

   if (KeyHit!=0)
   {
      //KeyHit=0;
      Serial.println(LastKey);
   }

   //flash the LED
   ld++; 
   if (ld==50) {digitalWrite(LED_BUILTIN,1); Swiggle=2;}
   if (ld>=100) 
   {
      ld=0; 
      digitalWrite(LED_BUILTIN,0); 
      Swiggle=0;
      //Serial.println(nowenc,HEX);
   }

   if (KeyHit!=0)
   {
      KeyHit=0;
      if (SetupMode==0)
      {
        AbsMouse.move(xp[LastKey],yp[LastKey]); 
        AbsMouse.press(MOUSE_LEFT);
        delay(100); 
        AbsMouse.release(MOUSE_LEFT);
        faderactive0=0;
        delay(100); 
      }
      else //setupmode. Store the position
      {         
         StorePos(LastKey);
      }
   }

   if (SetupMode==0)
   {
    a0=analogRead(AN0);
    if (abs(a0-preva0)>20)
    {
        //Serial.println(a0);
        preva0=a0;
        if (faderactive0==0) fp=a0; //this is a new move starting. Remember where we started off from
        atime0=millis();
        v=yp[23]+120+(112*(fp-a0)/1024);
        AbsMouse.move(xp[23], v);
        if (faderactive0==0) 
        {
              faderactive0=1;
              if (mousedn!=0) {AbsMouse.release(MOUSE_LEFT); faderactive1=0; timea=0; timeb=0; timec=0;}
              AbsMouse.press(MOUSE_LEFT);
              mousedn=1;
        }
    }

    if ((faderactive0!=0)&&((millis()-atime0)>FADER_TIMEOUT))
    {
          AbsMouse.release(MOUSE_LEFT);
          mousedn=0;
          faderactive0=0;
    }

    a1=analogRead(AN1);
    if (abs(a1-preva1)>20)
    {
        //Serial.println(a1);
        preva1=a1;
        if (faderactive1==0) fp=a1; //this is a new move starting. Remember where we started off from
        atime1=millis();
        v=yp[22]+120+(112*(fp-a1)/1024);
        AbsMouse.move(xp[22], v);
        if (faderactive1==0) 
        {
              faderactive1=1;
              if (mousedn!=0) {AbsMouse.release(MOUSE_LEFT); faderactive0=0; timea=0; timeb=0; timec=0;}
              AbsMouse.press(MOUSE_LEFT);
              mousedn=1;
        }
    }

    if ((faderactive1!=0)&&((millis()-atime1)>FADER_TIMEOUT))
    {
          AbsMouse.release(MOUSE_LEFT);
          mousedn=0;
          faderactive1=0;
    }
   }
   else //setup mode. Sliders move the cursor
   {
       a0=analogRead(AN0);
       a1=analogRead(AN1);
       AbsMouse.move((a0*1920)/1024+Swiggle, (a1*1080)/1024);
   }

  }
  

  delay(1); 

} 
