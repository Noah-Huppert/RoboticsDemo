#pragma config(Sensor, S2,     touchSensor,    sensorTouch)
/* ^ Init sensors ^ */

/* Sets motor speed */
void setMotors(int a, int b){
	motor[motorA] = a;//Set motorA
	motor[motorB] = b;//Set motorB
}

/* Tells if touch sensor is pressed */
bool touched(){
	bool isPressed = false;//Set value that will indicate if touch sensor is pressed

	if (SensorRaw(touchSensor) < 512){//The touch sensor is very weird and returns values 0 - 1024, if the value is less than 512 the sensor is pressed
		isPressed = true;
	}

	return isPressed;
}

task main()
{
	wait1Msec(100);//Wait 100 Msec for sensors to init

	setMotors(50, 50);//Set motors to half speed forward

	while (!touched()){//Wait until touch sensor is touched

	}

	setMotors(0, 0);//Stop motors
	wait1Msec(500);//Wait 500 Msec for motors to fully stop
	setMotors(-50, -50);//Set motors to half speed backwards
	wait1Msec(1000);//Wait 1 second
	setMotors(0, 0);//Stop motors
}
