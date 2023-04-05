#pragma config(Sensor, dgtl1,  startSwitch,    sensorTouch)
#pragma config(Sensor, dgtl2,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  probeEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl11, leftEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port2,           frontLeft,     tmotorVex393_MC29, openLoop, encoderPort, dgtl11)
#pragma config(Motor,  port3,           backLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           frontRight,    tmotorVex393_MC29, openLoop, reversed, encoderPort, dgtl2)
#pragma config(Motor,  port5,           backRight,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           probeMotor,    tmotorVex393_MC29, openLoop, encoderPort, dgtl7)

// Drive Function
void drive(int revolutions, int speed)
{
	SensorValue[rightEncoder] = 0;
	SensorValue[leftEncoder] = 0;

	int desiredEncoderTicks = revolutions * 360;

	while ((SensorValue[leftEncoder] < desiredEncoderTicks) || (SensorValue[rightEncoder] < desiredEncoderTicks))
	{
		wait1Msec(100);

		motor[backLeft] = speed;
		motor[backRight] = speed;
		motor[frontLeft] = speed;
		motor[frontRight] = speed;

		wait1Msec(1000);
	}

	motor[backLeft]=0;
	motor[backRight]=0;
	motor[frontLeft]=0;
	motor[frontRight]=0;
}

// Probe Function
void insertProbe(int pRevolutions, int pSpeed)
{
	SensorValue[probeEncoder] = 0;

	int desiredPEncoderTicks = pRevolutions * 35;

	while(SensorValue[probeEncoder] < desiredPEncoderTicks)
	{
		wait1Msec(200);
		motor[probeMotor] = pSpeed;
		wait1Msec(200);
	}

	motor[probeMotor] = 0;

	wait1Msec(1000);

	SensorValue[probeEncoder] = 0;

	while(SensorValue[probeEncoder] > -(desiredPEncoderTicks))
	{
		wait1Msec(200);
		motor[probeMotor] = -(pSpeed);
		wait1Msec(200);
	}

	motor[probeMotor] = 0;
}

// Turn Left Function
void turnLeft(int turnLRevolutions, int turnLSpeed)
{
	SensorValue[rightEncoder] = 0;
	SensorValue[leftEncoder] = 0;

	int turnLEncoderTicks = turnLRevolutions * 360;

	while(SensorValue[rightEncoder] < turnLEncoderTicks)
	{
		wait1Msec(100);

		motor[backLeft] = -30;
		motor[frontLeft] = 0;
		motor[backRight] = turnLSpeed;
		motor[frontRight] = turnLSpeed;

		wait1Msec(1000);
	}

		motor[backLeft] = 0;
		motor[frontLeft] = 0;
		motor[backRight] = 0;
		motor[frontRight] = 0;
}

// Turn Right Function
void turnRight(int turnRRevolutions, int turnRSpeed)
{
	SensorValue[rightEncoder] = 0;
	SensorValue[leftEncoder] = 0;

	int turnREncoderTicks = turnRRevolutions * 360;

	while(SensorValue[leftEncoder] < turnREncoderTicks)
	{
		wait1Msec(100);

		motor[backLeft] = turnRSpeed;
		motor[frontLeft] = turnRSpeed;
		motor[backRight] = -30;
		motor[frontRight] = 0;

		wait1Msec(1000);
	}

		motor[backLeft] = 0;
		motor[frontLeft] = 0;
		motor[backRight] = 0;
		motor[frontRight] = 0;
}

task main()
{
	int driver = 0;

	while(true)
	{
		motor[frontLeft] = vexRT[Ch3];
		motor[backLeft] = vexRT[Ch3];
		motor[frontRight] = vexRT[Ch2];
		motor[backRight] = vexRT[Ch2];

		if(vexRT[Btn6U] == 1)
		{
			motor[probeMotor] = 127;
		}
		else if (vexRT[Btn6D] == 1)
		{
			motor[probeMotor] = -127;
		}
		else
		{
			motor[probeMotor] = 0;
		}

		if(vexRT[Btn5D] == 1) // Pressing Button 5D enables self drive mode
		{
			driver = 1;
		}

		while(driver == 1) // Edit this part of code for desired autonomous routine. Demo below.
		{
			drive(3, 100);
			wait1Msec(1000);
			insertProbe(1, 127);
			wait1Msec(1000);
			drive(3, 100);

			wait1Msec(1000);
			turnLeft(6,75);
			wait1Msec(1000);
			drive(3,75);
			wait1Msec(1000);
			turnLeft(6,75);
			wait1Msec(1000);
			drive(3,75);
			wait1Msec(1000);

			drive(3, 100);
			wait1Msec(1000);
			insertProbe(1, 127);
			wait1Msec(1000);
			drive(3, 100);

			wait1Msec(1000);
			drive(3, 100);
			wait1Msec(1000);
			turnRight(3,75);
			wait1Msec(1000);
			drive(3, 100);
			wait1Msec(1000);
			turnRight(3,75);
			wait1Msec(1000);
			drive(3, 100);
			wait1Msec(1000);

			driver = 0; // Returns to RC mode after demo is over
		}
	}
}
