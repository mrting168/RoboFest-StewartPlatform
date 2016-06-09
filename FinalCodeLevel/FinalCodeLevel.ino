#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
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

MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity; 
float ypr[3];

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() 
{
    mpuInterrupt = true;
}


void setup()
{
  Wire.begin();
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
  Serial.println(F("\nSend any character to begin DMP programming and demo: "));
  while (Serial.available() && Serial.read()); // empty buffer
  while (!Serial.available());                 // wait for data
  while (Serial.available() && Serial.read()); // empty buffer again
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
    
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } 
    else 
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
    
    Servo1L.attach(11);
    Servo1R.attach(10);
    Servo2L.attach(9);
    Servo2R.attach(6);
    Servo3L.attach(5);
    Servo3R.attach(3);
}

void loop()
{
  if (!dmpReady) return;
  while (!mpuInterrupt && fifoCount < packetSize) 
  {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
  }
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount(); //accounts for overload. will send error if receiving values faster than it can deal with them.
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) 
  {
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
  } 
  else if (mpuIntStatus & 0x02) 
  {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;

    #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            
            Serial.print("ypr\t"); //HERE IS WHERE THE THING IS THE THING AND THE VALUES ARE READ. IT IS AN ARRAY.  
            Serial.print(ypr[0] * 180/M_PI); // convert from radians to degrees
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
            
            //autobalancing x axis
            if (ypr[1]* 180/M_PI > 10 || ypr[1]* 180/M_PI < -10)
            {
              Servo1R.write(14*(int(ypr[1]*-180/M_PI))-90); //mathematical formula for converting sensor values to usable encoder values
              Servo1L.write(180-(14*(int(ypr[1]*-180/M_PI)))+90);
              Servo2R.write(180-(14*(int(ypr[1]*-180/M_PI)))+90);
              Servo2L.write(14*(int(ypr[1]*-180/M_PI))-90);
              Servo3R.write(180-(14*(int(ypr[1]*-180/M_PI)))+90);
              Servo3L.write(14*(int(ypr[1]*-180/M_PI))-90);
            }
              
            //end autobalancing x axis
            //autobalancing y axis
            else if (ypr[2]* 180/M_PI > 10 || ypr[2]* 180/M_PI < -10) 
            {
              Servo2R.write(180-(14*(int(ypr[2]*-180/M_PI)))+90);
              Servo2L.write(14*(int(ypr[2]*-180/M_PI))-90);
              Servo1R.write(14*(int(ypr[2]*-180/M_PI))-90);
              Servo1L.write(180-(14*(int(ypr[2]*-180/M_PI)))+90);
              Servo3R.write(14*(int(ypr[2]*-180/M_PI))-90);
              Servo3L.write(180-(14*(int(ypr[2]*-180/M_PI)))+90);
            }
            //end autobalancing y axis
            else
            {
              Servo2L.write(90);
              Servo2R.write(90);
              Servo1L.write(90);
              Servo1R.write(90);
              Servo3L.write(90);
              Servo3R.write(90);
//              Serial.println("RESETTINGGGGG");
            }
        
        #endif
         
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
  }
  
  
}
