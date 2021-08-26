/*

  The following program setups the stepper motors and turns them in clockwise and counterclockwise manners to ensure they are wired correctly and working as intended.
  
*/


//Used to send the number of steps
#define motorXStep 2
#define motorYStep 3
#define motorZStep 4

//Used to determine the directions of the motors (clockwise or counterclockwise)
#define motorXDir 5
#define motorYDir 6
#define motorZDir 7

//Turned to HIGH by default. Recommend attaching a 2 pin jumper cap between EN and GND on the CNC Shield.
#define enableMotors 8 

//motorStep = Step Pin on Driver
//steps = Amount of Steps to Travel (200 Steps = 1 Rotation given 1.8 Degree Step Angle and Full Steps Enabled)
//delayTime = Speed of motor (Increase to move motor slower or decrease to move motor faster)

void step(int motorStep, int steps, int delayTime){
	for(int i = 0; i < steps; i++){
		digitalWrite(motorStep, HIGH);
		delay(delayTime);
		digitalWrite(motorStep, LOW);
		delay(delayTime);
	}
}

void setup(){
	pinMode(motorXStep, OUTPUT);
	pinMode(motorYStep, OUTPUT);
	pinMode(motorZStep, OUTPUT);

	pinMode(motorXDir, OUTPUT);
	pinMode(motorYDir, OUTPUT);
	pinMode(motorZDir, OUTPUT);
	
	//Enables the motors
	digitalWrite(8, LOW);
}

void loop(){
	//Moves the motors in a CLOCKWISE direction
	digitalWrite(motorXDir, HIGH);
	digitalWrite(motorYDir, HIGH);
	digitalWrite(motorZDir, HIGH);

	step(motorXStep, 200, 1000);
	step(motorYStep, 200, 1000);
	step(motorZStep, 200, 1000);
  
  //Moves the motors in a COUNTERCLOCKWISE direction
	digitalWrite(motorXDir, LOW);
	digitalWrite(motorYDir, LOW);
	digitalWrite(motorZDir, LOW);

	step(motorXStep, 200, 1000);
	step(motorYStep, 200, 1000);
	step(motorZStep, 200, 1000);
}
