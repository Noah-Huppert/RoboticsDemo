#pragma config(Sensor, S1, lightSensor, sensorLightActive)
/* ^ Init sensors ^ */

/* Init light sensor vars */
int lightValue = 0;//Init var that will hold current light sensor value
int lightLineValue;//Init var that will hold the light sensor value of the line

/* Init motor vars */
int primarySpeed = 20;
int secondarySpeed = 0;

/* Set motor values */
void setMotors(int a, int b){
	motor[motorA] = a;//Set motorA
	motor[motorB] = b;//Set motorB
}

task main()
{

	/*
	motorA = left
	motorB = right

	The higher the lightSensor value the lighter it is
	*/

	//Turn on Pid CTRL
	nMotorPIDSpeedCtrl[motorA] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[motorB] = mtrSpeedReg;

	wait1Msec(100);//Wait 100 Msec for sensors to init
	lightLineValue = SensorValue(lightSensor);//When the program starts the robot should be place on the line, gets line value

	while (true){//Main Loop
		lightValue = SensorValue(lightSensor);//Asign current light sensor value for esier use

		/*
		------ Line Following ------
		Following Left side of the line
		If light value is getting lighter turn RIGHT(>= lightLineValue)
		If light value is getting darker turn LEFT(<= lightLineValue)
		*/

		if (lightValue >= lightLineValue){//Going off left, turn right
			setMotors(primarySpeed, secondarySpeed);
		} else if (lightValue <= lightLineValue){//Going off right, turn left
			setMotors(secondarySpeed, primarySpeed);
		} else{//On the line, go straight
			setMotors(primarySpeed, primarySpeed);
		}

		wait1Msec(50);
	}

}
