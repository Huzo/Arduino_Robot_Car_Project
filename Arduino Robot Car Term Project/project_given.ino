/*
  ENGG1100 Given program for demo.
  Board: Arduino Nano
  Version: 201708.08_AlexSiu

Output, Servo : (D9)
Output,  7-segment (D10,D11)
Output, motors (D3,D2), (D5,D4)
Input: Ultrasound (D12,D13)
Input: S1 (A5)
Input: S2 (A4)
Input: S3 (A3)
Input: S4 (A2)
*/
//left motor is 40 slower than right.
#include "ENGG1100.h"
#include <Servo.h>
#define BLK 0
#define WHT 1
  
FSMClass FSM1;          //The given FSM is named 'FSM1'.
TM1637DisplayClass LEDDisplay(D10, D11); //7-segment LED Display is connected to D10,D11 for debug
ServoClass Servo1(D9);
SensorClass S1(A5); 
SensorClass S2(A4); 
SensorClass S3(A3); 
SensorClass S4(A2); 
MotorClass MotorR(D5,D4);
MotorClass MotorL(D3,D2);
UltrasoundSensorClass Sonic(D6,D7);
int outer_balls = -1;
int rotations = 0;
int first_ball = 0;

//===== Basically, no need to modify setup() and loop() ====
void setup()
{
  Serial.begin(115200);          //optional, for debug
  LEDDisplay.setBrightness(15); //optional
  FSM1.init(S_999);           // must have this line, you can change the first state of the FSM
  Servo1.init();
}

void loop()
{
    FSM1.run(); //important, don't modify this line              
}
//================= Students add STATES below this line ====================
//----------start of state S_999 -----
void S_999()
{
  if(FSM1.doTask())
  {
    Servo1.setValue(90);
    LEDDisplay.setValue(999); 
    MotorL.setSpeed(250);
    MotorR.setSpeed(250);    
  }
  if (FSM1.getTime() >700) FSM1.transit(S_101);  //1000
}
//------------------------------------
void S_101() //first black line.   
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(101); 
  }
  if (S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_102);
  if (S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== BLK) FSM1.transit(S_103);
  if (S2.getHiLow()== BLK && S3.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_104);  
  if (S1.getHiLow()== BLK && S2.getHiLow()== BLK && S3.getHiLow()== BLK && S4.getHiLow() == BLK) FSM1.transit(S_201);
  if (S2.getHiLow()== WHT && S3.getHiLow()== WHT && S4.getHiLow()== WHT) FSM1.transit(S_105);    
}
//------------------------------------
void S_102()//WBW- Go forward, just follow the line   
{
  if(FSM1.doTask())         
  {
    LEDDisplay.setValue(102); 
    MotorR.setSpeed(250); 
    MotorL.setSpeed(250);  
  }
  if (!(S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== WHT)) FSM1.transit(S_101);  
}
//------------------------------------
void S_103()   //WBB - Turn right a bit.
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(103); 
    MotorR.setSpeed(0);
    MotorL.setSpeed(250);   
  }
  if (!(S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== BLK)) FSM1.transit(S_101);   
}
//------------------------------------
void S_104()   //BBW - Turn left a bit.
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(104); 
    MotorR.setSpeed(250);
    MotorL.setSpeed(0);
  }
  if (!(S2.getHiLow()== BLK && S3.getHiLow()== BLK && S4.getHiLow()== WHT)) FSM1.transit(S_101);   
}
//------------------------------------
void S_105()// WWW - just turn right. you are at the corner.  
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(105); 
    MotorR.setSpeed(250); 
    MotorL.setSpeed(0);  
  }
  if (S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_101);  
}
//------------------------------------
void S_201() //BBBB  Triple edge encountered. Turn left. 
{
  if(FSM1.doTask())
  {
    if(first_ball == 1){
    LEDDisplay.setValue(201); 
    MotorR.setSpeed(250);
    MotorL.setSpeed(0);   
    }   
  }
  if(first_ball == 0) FSM1.transit(S_601);
  if (S1.getHiLow()== WHT && S2.getHiLow()== WHT) FSM1.transit(S_301); 
}
//------------------------------------
void S_301() //After triple edge.   
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(301); 
  }
  if (S1.getHiLow()== BLK){
    outer_balls++;
    LEDDisplay.setValue(outer_balls);
    if(outer_balls == rotations){
      FSM1.transit(S_401); 
    }
    else{
      while(S1.getHiLow()==BLK){
        MotorR.setSpeed(110); 
        MotorL.setSpeed(110);
      }
      FSM1.transit(S_301);
    }
  }
  if (S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_302);
  if (S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== BLK) FSM1.transit(S_303);
  if (S2.getHiLow()== BLK && S3.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_304);   
  if (S2.getHiLow()== WHT && S3.getHiLow()== WHT && S4.getHiLow()== WHT) FSM1.transit(S_305);
}
//------------------------------------
void S_302()   //WBW- Go forward. 
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(302); 
    MotorR.setSpeed(250);//220
    MotorL.setSpeed(250);//250   
  }
  if (S1.getHiLow()== BLK){
    outer_balls++;
    LEDDisplay.setValue(outer_balls);
    if(outer_balls == rotations){
      FSM1.transit(S_401); 
    }
    else{
      while(S1.getHiLow()==BLK){
        MotorR.setSpeed(110); 
        MotorL.setSpeed(110);
      }
      FSM1.transit(S_301);
    }
  }
  if (!(S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== WHT)) FSM1.transit(S_301); 
}
//------------------------------------
void S_303()  //WBB - Turn right a bit.  
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(303);
    MotorR.setSpeed(0);  
    MotorL.setSpeed(250);
  }
  if (S1.getHiLow()== BLK){
    outer_balls++;
    LEDDisplay.setValue(outer_balls);
    if(outer_balls == rotations){
      FSM1.transit(S_401); 
    }
    else{
      while(S1.getHiLow()==BLK){
        MotorR.setSpeed(110); 
        MotorL.setSpeed(110);
      }
      FSM1.transit(S_301);
    }
  }
  if (!(S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== BLK)) FSM1.transit(S_301); 
}
//------------------------------------
void S_304()  //BBW - Turn left a bit. 
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(304); 
    MotorR.setSpeed(250); 
    MotorL.setSpeed(0);  
  }
  if (S1.getHiLow()== BLK){
    outer_balls++;
    LEDDisplay.setValue(outer_balls);
    if(outer_balls == rotations){
      FSM1.transit(S_401); 
    }
    else{
      while(S1.getHiLow()==BLK){
        MotorR.setSpeed(110); 
        MotorL.setSpeed(110);
      }
      FSM1.transit(S_301);
    }
  }
  if (!(S2.getHiLow()== BLK && S3.getHiLow()== BLK && S4.getHiLow()== WHT)) FSM1.transit(S_301);   
}
//------------------------------------
void S_305()  //WWW - 360. 
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(305); 
    MotorR.setSpeed(0); 
    MotorL.setSpeed(0);
    Servo1.setValue(90);
    while(FSM1.getTime() < 1000){
      MotorR.setSpeed(200); 
      MotorL.setSpeed(200);  
    }
    MotorR.setSpeed(-120); 
    MotorL.setSpeed(120);  
    rotations++;
  }
  
  outer_balls = -1;
  if (S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_101);   
}
//------------------------------------
void S_401() //S1=B, must turn and detect the ball  
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(401); 
    if(outer_balls == rotations){
      LEDDisplay.setValue(401); 
      while(S1.getHiLow()==BLK){
        MotorR.setSpeed(-200);
        LEDDisplay.setValue(200);
        MotorL.setSpeed(-200);
      }
      MotorR.setSpeed(0);
      MotorL.setSpeed(0);
      while(!(Sonic.getDistance()<40)){
        MotorR.setSpeed(-80);
        LEDDisplay.setValue(80);
        MotorL.setSpeed(80);
      }
      MotorR.setSpeed(0);
      MotorL.setSpeed(0);
    }
    else{
      FSM1.transit(S_301);
      /*while(FSM1.getTime() < 1000){
        FSM1.transit(S_101);
      }
      while(S1.getHiLow() != BLK){
        FSM1.transit(S_101);
      }*/
    }
  }
  if (Sonic.getDistance()<40 || FSM1.getTime()>2000)  FSM1.transit(S_402); 
}
//------------------------------------
void S_402() //ball found, wait a bit.  
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(402); 
    MotorR.setSpeed(-10); 
    MotorL.setSpeed(-10);
  }
  if (FSM1.getTime() >500 || Sonic.getDistance()<40)  FSM1.transit(S_403); 
}
//------------------------------------
void S_403() //stop the motors. Put the catcher down and catch the ball.  
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(403); 
    Servo1.setValue(190);
    MotorR.setSpeed(0);
    MotorL.setSpeed(0);      
  }
  if (FSM1.getTime() >1000)  FSM1.transit(S_501); 
}
//------------------------------------
void S_501()  //go back to the track.  
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(501);
    MotorR.setSpeed(175);
    MotorL.setSpeed(-140);      
  }
  if (S2.getHiLow()== WHT && S3.getHiLow()== BLK && S4.getHiLow()== WHT) FSM1.transit(S_101);
}
//------------------------------------
void S_601()  //first_ball  
{
  if(FSM1.doTask())
  {
    LEDDisplay.setValue(601);
    while(S1.getHiLow()==BLK){
      MotorR.setSpeed(200);
      MotorL.setSpeed(200);
    }
    Servo1.setValue(190);
    first_ball = 1;
    while(!(S2.getHiLow()== WHT && S3.getHiLow()== WHT && S4.getHiLow()== WHT)){
      MotorR.setSpeed(-220);
      MotorL.setSpeed(-200);    
    }
  }
  if (S2.getHiLow()== WHT && S3.getHiLow()== WHT && S4.getHiLow()== WHT) FSM1.transit(S_999);
}

