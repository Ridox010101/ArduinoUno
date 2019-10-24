#include <ZumoShield.h>

/*
 * Demo line-following code for the Pololu Zumo Robot
 *
 * This code will follow a black line on a white background, using a
 * PID-based algorithm.  It works decently on courses with smooth, 6"
 * radius curves and has been tested with Zumos using 30:1 HP and
 * 75:1 HP motors.  Modifications might be required for it to work
 * well on different courses or with different motors.
 *
 * https://www.pololu.com/catalog/product/2506
 * https://www.pololu.com
 * https://forum.pololu.com
 */

#include <Wire.h>
#include <ZumoShield.h>

ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
int lastError = 0;

// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)
const int MAX_SPEED = 400;
int state=0;
#define QTR_THRESHOLD 1000
#define LED 13


void waitForButtonAndCountDown()

{

  digitalWrite(LED, HIGH);

  button.waitForButton();

  digitalWrite(LED, LOW);

   

  // play audible countdown

  for (int i = 0; i < 1; i++)

  {

    delay(1000);

  }

}


void setup()
{
  
  // Play a little welcome song
  //buzzer.play(">g32>>c32");

  // Initialize the reflectance sensors module
  reflectanceSensors.init();

  // Wait for the user button to be pressed and released
  /*button.waitForButton();

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);*/
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
  //buzzer.play(">g32>>c32");

  // Wait for the user button to be pressed and released
  //button.waitForButton();

 waitForButtonAndCountDown();
  
  Serial.begin(9600);
}





bool sensor1;
bool sensor2;
bool sensor3;
bool sensor4;
bool sensor5;
bool sensor6;



void stopRobot()
{
  motors.setSpeeds(0, 0);
}
void forwardRobot() {
  //analogWrite(EA, 254); //speed 0 - 255
motors.setSpeeds(400, 400);
}

void leftRobot() {
  motors.setSpeeds(400, -400);
}

void rightRobot() {
  motors.setSpeeds(-400, 400);
}

void sharprightRobot(){
  motors.setSpeeds(-400, 400);
}

void sharpleftRobot(){
  motors.setSpeeds(400, -400);
}

#define NUM_SENSORS 6

unsigned int sensor_values[NUM_SENSORS];



void forward()
{
  if(!sensor1 && !sensor2 && sensor3 && sensor4 && !sensor5 && !sensor6)
    {
       state = 1; //forward
    }  
}

void left()
{
  if(!sensor1 && !sensor2 && !sensor3 && sensor4 && sensor5 && sensor6)
    {
       state = 2; //left
    }
}
void right()
{
  if(sensor1 && sensor2 && sensor3 && !sensor4 && !sensor5 && !sensor6)  
  {
    state = 3;
  }
}
void sharpRight(){
  if(sensor1 && sensor2 && sensor3 && sensor4 && !sensor5 && !sensor6){
    state = 4;//sharp right
  }
}
void sharpLeft(){
  if(!sensor1 && !sensor2 && sensor3 && sensor4 && sensor5 && sensor6){
    state = 5; //sharp left
  }
}
void stopthisRobot(){
  if(sensor1 && sensor2 && sensor3 && sensor4 && sensor5 && sensor6){
    state = 6; //stop
  }
}
void pitstop(){
int count = 0;
  
    if (count < 1)
      {
      if(sensor1 && sensor2 && sensor3 && sensor4 && sensor5 && sensor6)
          {
          state = 6;
          count = count++;
          delay(1000);
          }
      }  
  
  
  
  
  
}

void loop()
  {
   
  


  //readSensors();
  //actions();
  reflectanceSensors.read(sensor_values);
  //button.waitForButton();
  
  

/*
 * State selection
 */



sensor1 = sensor_values[0] >  QTR_THRESHOLD;
sensor2 = sensor_values[1] >  QTR_THRESHOLD;
sensor3 = sensor_values[2] >  QTR_THRESHOLD;
sensor4 = sensor_values[3] >  QTR_THRESHOLD;
sensor5 = sensor_values[4] >  QTR_THRESHOLD;
sensor6 = sensor_values[5] >  QTR_THRESHOLD;

  

switch(state)
{
  case 0 :
    stopRobot();
    
    forward();
    left();
    sharpLeft();
    right();
    sharpRight();
  break;
  
  case 1 :
    forwardRobot();

    
    left();
    sharpLeft();
    right();
    sharpRight();
    stopthisRobot();
  break;  
  
  case 2 :
    leftRobot();
    
    forward();
    sharpLeft();
    right();
    sharpRight();
    stopthisRobot();
  break;

     
  case 3 :
    rightRobot();
    
    forward();
    left();
    sharpLeft();
    sharpRight();
    stopthisRobot();
  break; 
  
  case 4 :
    sharprightRobot();
    
    forward();
    left();
    sharpLeft();
    right();
    stopthisRobot();
  break;

  case 5 :
    sharpleftRobot();

    forward();
    left();
    right();
    sharpRight();
    stopthisRobot();      
  break;

    case 6 :  
    pitstop();

    sharpleftRobot();
    forward();
    left();
    right();
    sharpRight();
    stopthisRobot();      
  break;
  
  default:
  state = 0;
  break;  
}
  }
