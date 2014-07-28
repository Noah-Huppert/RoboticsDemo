#pragma config(Sensor, S3,     sonarSensor,    sensorSONAR)
/* ^ Init sensors ^ */

/* Init ultrasonic sensor values */
int wallDistance;//When program start the robot should be the distance you want from the wall, this var will be used for keeping track of that
int currentDistance;//Init var which will be used to keep track of distance away from wall

/* Init motor speed vars */
int primarySpeed = 50;
int secondarySpeed = 30;

/* Set motor speed */
void setMotors(int a, int b){
	motor[motorA] = a;//Set motorA
	motor[motorB] = b;//Set motorB
}

task main()
{
	wait1Msec(100);//Wait 100 Msec for sensors to init


	wallDistance = SensorValue(sonarSensor);//Set distance away from wall to follow


	while(true){//Main loop
		currentDistance = SensorValue(sonarSensor);//Set current ultrasonic sensor value for better use

		if(currentDistance <= wallDistance){//To far away from wall, turn LEFT
			setMotors(secondarySpeed, primarySpeed);
		} else if(currentDistance >= wallDistance){//To close to wall, turn RIGHT
			setMotors(primarySpeed, secondarySpeed);
		} else{//Good distance from wall, go straight
			setMotors(primarySpeed, primarySpeed);
		}
	}

}
