/* Set motors */
void setMotors(int a, int b, int c){
	motor[motorA] = a;//Set motorA
	motor[motorB] = b;//Set motorB
	motor[motorC] = c;//Set motorC
}

/* Encoder Move */
void clawMove(int moveTo, int speed){

	nMotorEncoder[motorC] = 0;//Reset encoder
	nMotorEncoderTarget[motorC] = moveTo;//Set motor encoder target, tell robot how far you want to move

	motor[motorC] = speed;//Start robot moving

	while(nMotorRunState[motorC] != runStateIdle){//Wait for robot to reach target

	}

	motor[motorC] = 0;
}

void encoderMove(int aMoveTo, int bMoveTo, int aSpeed, int bSpeed){
	/* Start pickup object 18" away */

	/* Init vars to specify if motors are done moving */
	bool aComplete = false;
	bool bComplete = false;

	/* Reset encoder values */
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;

	/* Tell robot how far you want to move */
	nMotorEncoderTarget[motorA] = aMoveTo;
	nMotorEncoderTarget[motorB] = bMoveTo;

	/* Start robot moving */
	motor[motorA] = aSpeed;
	motor[motorB] = bSpeed;

	while(!aComplete && !bComplete){//Loop until both motors have reached their target
		if(nMotorRunState[motorA] == runStateIdle && !aComplete){//If motorA is done and it hasnt been set to done before
			motor[motorA] = 0;//Stop motorA
			aComplete = true;//Set motorA to done
		}

		if(nMotorRunState[motorB] == runStateIdle && !bComplete){//If motorB is done and it hasnt been set to done before
			motor[motorB] = 0;//Stop motorB
			bComplete = true;//Set motorB to done
		}
	}
}

task main()
{

	wait1Msec(100);//Wait for sensors and motors to init

	clawMove(500, 20);//Open claw

	encoderMove(500, 500, 30, 30);//Move robot forward 2 rotations of the wheel

	clawMove(500, -20);//Close claw

	encoderMove(400, 400, -20, 20);//Turn robot

	encoderMove(500, 500, 30, 30);//Move robot forward 2 rotations of the wheel

	clawMove(500, 20);//Open claw back up

	encoderMove(150, 150, -20, -20);//Back robot up

	clawMove(500, -20);//Close claw
}
