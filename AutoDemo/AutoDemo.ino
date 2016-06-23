#include <Servo.h>

Servo Servo1L; 
Servo Servo1R; 
Servo Servo2L; 
Servo Servo2R; 
Servo Servo3L;
Servo Servo3R; 

int pos=0;
int rate=0; 
int x=0;


Peekaboo!

int photoSensor1= A0;// 2 
int photoSensor2= A1; //3
int photoSensor3= A2; //1

int photoValue1=0; 
int photoValue2=0; 
int photoValue3=0; 

void setup()
{
  Servo1L.attach(11);
  Servo1R.attach(10);
  Servo2L.attach(9);
  Servo2R.attach(6);
  Servo3L.attach(5);
  Servo3R.attach(3);
  Serial.begin(9600);  
}

void loop()
{
  photoValue2= analogRead(photoSensor1);
  photoValue3= analogRead(photoSensor2);
  photoValue1= analogRead(photoSensor3);
  Serial.println(photoValue1);
  Serial.println(photoValue2);
  Serial.println(photoValue3);
  
  if(photoValue1>photoValue2+50 && photoValue1>photoValue3+50)
  {
    tilt1();
    drop2();
    drop3();
    delay(100);
  }
  else if(photoValue2>photoValue1+50 && photoValue2>photoValue3+50)
  {
    tilt2();
    drop1();
    drop3();
    delay(100);
  }
  else if(photoValue3>photoValue2+50 && photoValue3>photoValue1+50)
  {
    tilt3();
    drop1();
    drop2();
    delay(100);
  }
  else
  {
    Servo1L.write(90);
    Servo1R.write(90);
    Servo2L.write(90);
    Servo2R.write(90);
    Servo3L.write(90);    
    Servo3R.write(90);        
  }
}
void YmoveUp(int rate)
{
  for(pos=90; pos<180; pos+=1)
  {
    Servo1L.write(pos);
    Servo2L.write(pos);
    Servo3L.write(pos);    
    Servo1R.write(180-pos);
    Servo2R.write(180-pos);
    Servo3R.write(180-pos);    
    
    delay(rate);
  }
  
}

void YmoveDown(int rate)
{
  for(pos=180; pos>=90; pos-=1)
  {
    Servo1L.write(pos);
    Servo2L.write(pos);
    Servo3L.write(pos);
    
    Servo1R.write(180-pos);
    Servo2R.write(180-pos);
    Servo3R.write(180-pos);
    
    delay(rate); 
  }    
}
void tilt3()
{
  for(pos=90; pos<180; pos++)
  {
    Servo3L.write(pos);
    Servo3R.write(180-pos);
  }
}
void tilt2()
{
  for(pos=90; pos<180; pos++)
  {
    Servo2L.write(pos);
    Servo2R.write(180-pos);
  }
}
void tilt1()
{
  for(pos=90; pos<180; pos++)
  {
    Servo1L.write(pos);
    Servo1R.write(180-pos);
  }
}
void drop1()
{
  for(pos=90; pos>=45; pos-=1)
  {
    Servo1L.write(pos);
    Servo1R.write(180-pos);
  }
}
void drop2()
{
  for(pos=90; pos>=45; pos-=1)
  {
    Servo2L.write(pos);
    Servo2R.write(180-pos);
  }
}
void drop3()
{
  for(pos=90; pos>=45; pos-=1)
  {
    Servo3L.write(pos);
    Servo3R.write(180-pos);
  }
}

//1R and 1L up and down
  //3R up and 3L down
  //2R up and 2L down 
void surge()
{
  for(pos=0; pos<=20; pos++)
  {
    Servo1L.write(pos);
    Servo3R.write(pos);
  }
  for(pos=20; pos>=0; pos-=1)
  {
    Servo1R.write(pos);
    Servo2L.write(pos);
  }
  for(pos=0; pos<=20; pos++)
  {
    Servo2R.write(pos);
    Servo3L.write(pos);
  }  
  
  delay(30);
  for(pos=20; pos>=0; pos-=1)
  {
    Servo1L.write(pos);
    Servo3R.write(pos);
  }
  for(pos=0; pos<=20; pos++)
  {
    Servo1R.write(pos);
    Servo2L.write(pos);
  }
  for(pos=20; pos>=0; pos-=1)
  {
    Servo2R.write(pos);
    Servo3L.write(pos);
  }
}
