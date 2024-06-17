#include <Wire.h>
#include <MPU6050_light.h>
//overall 
const int maxSpeed = 255; //max PWM value written to motor speed pin. It is typically 255.
const int minSpeed = 160; //min PWM value at which motor moves
float angle; //due to how I orientated my MPU6050 on my car, angle = roll
float targetAngle = 0;
int equilibriumSpeed = 248; //rough estimate of PWM at the speed pin of the stronger motor, while driving straight 
float GyroX;
//and weaker motor at maxSpeed
int leftSpeedVal = 200;
int rightSpeedVal = 200;


// Adjust these values to tune values at the venue
//adjust the target angle the kit turns to
int rightTurnAngle = 80;
int leftTurnAngle = 80;
//for the forward command - in case of drift 
int rightMotorBias = 15;
int leftMotorBias = 0;

MPU6050 mpu(Wire);

//IMU 
float elapsedTime, currentTime, previousTime;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ; //used in void loop()
float roll, pitch, yaw;
int c = 0;
// Motor A 
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B
int enB = 6;
int in3 = 5;
int in4 = 4;

//ultrasonic
const int trigPin = 10;
const int echoPin = 12;

void setup()
 
{
  Wire.begin(); 
  mpu.begin();
  Serial.println(("Calculating offsets, do not move kit"));   
  delay(1000);  
  mpu.calcGyroOffsets();                        // This does the calibration
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
//ultrasonic
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Serial.begin(9600); 
  mpu.update();
  angle = mpu.getAngleZ();

  //ENTER CODE HERE - 1 SECOND DELAY BETWEEN ACTIONS
forward(10);
right();
  //CODE ENDS HERE
}
void loop(){

  }

void forward(int distance){
  int target = mpu.getAngleZ();
  int dist = getDistance();
  int destination = getDistance() - distance;
  delay(10);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  while(dist > destination){
  int newLeft = leftSpeedVal;
  int newRight = rightSpeedVal;
  dist = getDistance();
  Serial.print(dist);
   Serial.print(" and ");
  Serial.println(destination);
  mpu.update();
  angle = mpu.getAngleZ();
  int deltaAngle = target - angle;
  if(abs(deltaAngle) > 3){
    newLeft -= deltaAngle * 2;
    newRight += deltaAngle * 2;
  }
  analogWrite(enA, newLeft + leftMotorBias);
  analogWrite(enB, newRight+rightMotorBias);
  }
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(1000);
}

void right(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  mpu.update();
  int targetAngle = mpu.getAngleZ() - rightTurnAngle;
  while(targetAngle <= round(mpu.getAngleZ())){
  int newLeft = 150;
  mpu.update();
  angle = mpu.getAngleZ();
  //int deltaAngle = targetAngle - angle;
  //newLeft -= deltaAngle;
  analogWrite(enA, newLeft);
  analogWrite(enB, newLeft);
  }
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(1000);
}
void left(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  mpu.update();
  int targetAngle = mpu.getAngleZ() + leftTurnAngle;
  while(targetAngle >= round(mpu.getAngleZ())){
  int newRight = 150;
  mpu.update();
  angle = mpu.getAngleZ();
 // int deltaAngle = targetAngle - angle;
 // newRight += deltaAngle;
  analogWrite(enA, newRight);
  analogWrite(enB, newRight);
  }
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(1000);
}

int getDistance(){
 long duration;
 int calcDistance = 0;
 while(calcDistance == 0){
 //Clears the trigPin
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 // Sets the trigPin on HIGH state for 10 micro seconds
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 // Reads the echoPin, returns the sound wave travel time in microseconds
 duration = pulseIn(echoPin, HIGH);
 // Calculating the distance
 calcDistance = (duration * (0.034 / 2));
 Serial.println("zero");
 delay(10);
 }
Serial.println(calcDistance);
 return calcDistance - 3;
}