#pragma config(Sensor, S1,     touchSensor,    sensorTouch)
#pragma config(Sensor, S2,     lightSensor,    sensorLightActive)
#pragma config(Sensor, S3,     touchSensorB,    sensorTouch)
/* ^ Init Sensors ^ */


/* Define motor speeds */
int closeSpeed = -10;
int openSpeed = 5;

/* Init vars for Light Sensor Thresholds */
int normalLight = 0;
int exitThreshold = 0;
int enterThreshold = 0;

/* Set pre determined motor encoder values for each coin */
float pen = 564.3;
float nic = 611.1;
float dim = 530.1;
float qua = 724.5;

/* Init vars to count how many of each coin has passed through */
int penC = 0;
int nicC = 0;
int dimC = 0;
int quaC = 0;

/* Init vars for finding the difference between the current motor encoder value and the predetermined ones for each coin*/
float penD = 0;
float nicD = 0;
float dimD = 0;
float quaD = 0;

/* Set motor speed */
void setMotor(int a){
	motor[motorA] = a;//Set motor speed
}

/* Determies if the specified touch sensor is pressed */
bool touched(int sensor){
	bool isPressed = false;//Init var to determine if the touch sensor is touched
	int sValue = 0;//Init var that will hold the current touch sensor value

	/* Based on the input, determine the sensor to read, then get its value */
	switch(sensor){
		case 1:
			 sValue = SensorRaw(touchSensor);
			break;
		case 2:
			sValue = SensorRaw(touchSensorB);
			break;
	}

	if (sValue < 512){//the touch sensor is very weird and outputs a values from 0 - 1024, if it is less than 512 the sensor is pressed
		isPressed = true;//Indicate that the sensor is pressed
	}

	return isPressed;
}

/* Close the chamber */
bool setClose(){
	if (!touched(1)){//Since this will be getting called continuously, there is a good chance the chamber is already closed, here we are checking if the chamber is already closed
		setMotor(closeSpeed);//If the chamber is not already closed, start the motor at a pre determined closing speed
	} else{
		return false;//If the chamber is already closed exit the function, also return false to indicate that the chamber was already closed
	}

	while (!touched(1)){//Wait until the chamber is closed

	}

	setMotor(0);//Stop the motors
	return true;//Return true to indicate that the chamber was not already closed
}

/* Find the lowest out of the 4 values inputed */
int lowest(float a, float b, float c, float d){
	float lowest = 100000;//Init lowest value var, set to value so high it will always be higher than anything inputed
	int ii = 0;//Init var keeping track of what position is the lowest
	/*        ^           a - 0, b - 1, c - 2, d - 3                 ^   */

	int t[4] = { a, b, c, d };//Create an array of all the inputed values

	for (int i = 0; i < 4; i++){//Make a for loop that will loop 4 times, each time it loops the var i goes up by 1
		if (t[i] < lowest){//Check if the current value being checked is lower than the lowest value found so far
			lowest = t[i];//If it is, set current value as the lowest value found so far
			ii = i;//Keep track of what position the lowest value was in
		}
	}

	return ii;//Return the position of the lowest number
}

/* Count the current coin */
void countCoin()
{

	/* Reset encoder values */
	nMotorEncoder[motorA] = 0;

	normalLight = SensorValue(lightSensor);//Get normal light value, without coin
	enterThreshold = normalLight + 100;//Set threshold in which coin is in the chamber
	exitThreshold = normalLight + 50;//Set threshold in which coin has exited the chamber

	/* Make exit condition vars */
	bool cEnter = false;
	bool cExit = false;

	/* Start opening chamber */
	setMotor(openSpeed);

	/* Wait until coin has entered the chamber */
	while (!cEnter){
		int currentLight = SensorValue(lightSensor);//Get current light value for esier use

		if (currentLight >= enterThreshold){
			cEnter = true;//Exit the loop
		}
	}

	/* Wait until coin has left the chamber */
	while (!cExit){
		int currentLight = SensorValue(lightSensor);//Get current light value for easier use

		if (currentLight < (normalLight + 50)){
			cExit = true;//Exit the loop
		}
	}

	setMotor(0);//Stop chamber from opening



	int encoderVal = nMotorEncoder[motorA];//Get current encoder value

	/* Calculate how far the encoder value is from each type of coin */
	penD = abs(pen - encoderVal);
	nicD = abs(nic - encoderVal);
	dimD = abs(dim - encoderVal);
	quaD = abs(qua - encoderVal);

	int coinN = lowest(penD, nicD, dimD, quaD);//Find the lowest difference, lowest() returns 0 - 3


	string coin = "Quarter";//Init coin name string. Asume quarter

	/* Set coin based on which difference was the lowest */
	switch (coinN){
	case 0:
		coin = "Penny";//Set to Penny
		penC++;//Add 1 Penny to the Penny count
		break;
	case 1:
		coin = "Nickel";//Set to Nickel
		nicC++;//Add 1 Nickel to the Nickel count
		break;
	case 2:
		coin = "Dime";//Set to Dime
		dimC++;//Add 1 Dime to the Dime count
		break;
	case 3:
		coin = "Quarter";//Set to Quarter
		quaC++;//Add 1 Quarter to the Quarter count
		break;
	}

	/* Debug Stuff, commented out because we are not debugging */
	//writeDebugStreamLine("Encoder %i", nMotorEncoder[motorA]);
	//writeDebugStreamLine("%s | %f %f %f %f | %i", coin, penD, nicD, dimD, quaD, coinN);

	/* Display values on the screen */
	nxtDisplayTextLine(1, "Pennies:     %i", penC);//Display amount of Pennies on line 1
	nxtDisplayTextLine(2, "Nickels:     %i", nicC);//Display amount of Nickels on line 2
	nxtDisplayTextLine(3, "Dimes:       %i", dimC);//Display amount of Dimes on line 3
	nxtDisplayTextLine(4, "Quarters:    %i", quaC);//Display amount of Quarters on line 4
	nxtDisplayTextLine(5, "Total Coins: %i", (penC + nicC + dimC + quaC));//Display total amount of coins on line 5
	nxtDisplayTextLine(6, "Money: $%.2f", (0.01 * penC) + (0.05 * nicC) + (0.1 * dimC) + (0.25 * quaC));//Display total value of coins on line 6
	nxtDisplayTextLine(7, "Coin: %s", coin);//Display cion type on line 7
}


task main(){
	wait1Msec(100);//Wait 100 Msec for sensors to init

	while (true){//Main loop
		if (setClose()){//setClose() will close the chamber, if the chamber was not already closed when it was called it returns true
			playSound(soundBeepBeep);//Play sound to tell user it is ready for a new coin
		}

		if (touched(2)){//If touch sensor B is touched start proccess of coin counting
			countCoin();//Start coin counting
		}
	}
}
