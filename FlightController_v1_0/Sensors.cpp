// Author: Jan Wielgus
// Date: 25.10.2017
// 

#include "Sensors.h"
SensorsClass sensors;

void dmpDataReady()
{
	sensors.mpuInterrupt = true;
}



void SensorsClass::init()
{
	Wire.begin();
	
	//////////////////////////////////////////////////////////////////////////
	// MPU6050
	
	mpu.initialize();
	pinMode(INTERRUPT_PIN, INPUT);
	
	devStatus = mpu.dmpInitialize();
	
	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXAccelOffset(1127);
	mpu.setYAccelOffset(1297);
	mpu.setZAccelOffset(1196);

	mpu.setXGyroOffset(78);
	mpu.setYGyroOffset(-47);
	mpu.setZGyroOffset(3);
	
	if (devStatus == 0)
	{
		mpu.setDMPEnabled(true);
		
		attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();
		
		dmpReady = true;
		
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else
	{
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
	}
	
	//////////////////////////////////////////////////////////////////////////
	// HMC5883L
	
	uint8_t tempCount=0;
	while (!compass.begin())
	{
		if (tempCount > 1) break; // przerwij jezeli nie uda sie wystartowac przez 2 powtorzenia
		delay(500);
		tempCount++;
	}
	
	// W inicjalizacji kompasu jest tez Wire.begin() dlatego ustawienie zegara nastêpuje po ostatniej inicjalizacji
	Wire.setClock(400000L);
	
	if (tempCount <= 1) // jezeli uruchomil sie
	{
		// Set measurement range
		compass.setRange(HMC5883L_RANGE_1_3GA);
		// Set measurement mode
		compass.setMeasurementMode(HMC5883L_CONTINOUS);
		// Set data rate
		compass.setDataRate(HMC5883L_DATARATE_30HZ);
		// Set number of samples averaged
		compass.setSamples(HMC5883L_SAMPLES_8);
		// Set calibration offset. See HMC5883L_calibration.ino
		compass.setOffset(MAG_X_OFFSET, MAG_Y_OFFSET);
	}
}



void SensorsClass::readAngles()
{
	// if programming failed, don't try to do anything
	if (!dmpReady) return;
	
	// wait for MPU interrupt or extra packet(s) available
	while (!mpuInterrupt && fifoCount < packetSize) {
		// other program behavior stuff here
		
		// if you are really paranoid you can frequently test in between other
		// stuff to see if mpuInterrupt is true, and if so, "break;" from the
		// while() loop to immediately process the MPU data
		
		// EWENTUANIE daæ tu break/return !!!
		return;
	}
	
	// reset interrupt flag and get INT_STATUS byte
	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();
	
	// get current FIFO count
	fifoCount = mpu.getFIFOCount();
	
	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) || fifoCount == 1024)
	{
		mpu.resetFIFO();
	}
	else if (mpuIntStatus & 0x02)
	{
		// wait for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

		// read a packet from FIFO
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		
		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;
		
		//////////////////////////////////////////////////////////////////////////

		int16_t qI[4];
		//if (fifoBuffer == 0) fifoBuffer = mpu.dmpPacketBuffer;
		qI[0] = ((fifoBuffer[0] << 8) | fifoBuffer[1]);
		qI[1] = ((fifoBuffer[4] << 8) | fifoBuffer[5]);
		qI[2] = ((fifoBuffer[8] << 8) | fifoBuffer[9]);
		qI[3] = ((fifoBuffer[12] << 8) | fifoBuffer[13]);
		
		q.w = (float)qI[0] / 16384.0f;
		q.x = (float)qI[1] / 16384.0f;
		q.y = (float)qI[2] / 16384.0f;
		q.z = (float)qI[3] / 16384.0f;
		
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		
		angle.pitch = (ypr[1]*(-60))-PITCH_OFFSET;
		angle.roll  = (ypr[2]*60)-ROLL_OFFSET;
		angle.yaw   = ypr[3];
		
		//////////////////////////////////////////////////////////////////////////
	}
}


void SensorsClass::readCompass()
{
	// read vectors
	magReading = compass.readNormalize();
	
	accScaled.XAxis = angle.pitch*0.01;
	accScaled.YAxis = angle.roll*0.01;
	
	// Jezeli za bardzo przechylony to nie kompensuj przechylenia (powoduje to NAN albo INF)
	if (angle.pitch<MAG_COMP_ANGLE && angle.pitch>-MAG_COMP_ANGLE &&
		angle.roll<MAG_COMP_ANGLE && angle.roll>-MAG_COMP_ANGLE)
		headnigComp = tiltCompensate(magReading, accScaled);
	else
		headnigComp = noTiltCompensate(magReading);
	
	headnigComp = correctAngle(headnigComp);
	headnigComp = headnigComp * RAD_TO_DEG;
	
	timeNow = millis();
	timeStep = float(timeNow-lastTime)*0.001;
	lastTime = timeNow;
	
	gyroYaw = -mpu.getRotationZ()-GYRO_Z_OFFSET;
	gyroYaw *= dpsPerDigit;
	
	headnigGyroMagn += gyroYaw * timeStep;
	
	// 0* bug fix
	if (abs(headnigComp-headnigGyroMagn) > 100) // jezeli roznica katow jest za duza
	{
		if (headnigComp > 180) headnigComp-=360;
		else headnigComp+=360;
	}
	
	// complementary filter
	headnigGyroMagn = (0.98 * headnigGyroMagn) + (0.02 * headnigComp);
	
	// angle correction (0-360)
	if (headnigGyroMagn >= 360) headnigGyroMagn -= 360;
	else if (headnigGyroMagn < 0) headnigGyroMagn += 360;
}


float SensorsClass::tiltCompensate(Vector mag, Vector normAccel)
{
	float acP, acR; // acc pitch & roll
	acR = asin(normAccel.YAxis);
	acP = asin(-normAccel.XAxis);
	
	// Some of these are used twice, so rather than computing them twice in the algorithem we precompute them before hand.
	float cosRoll = cos(acR);
	float sinRoll = sin(acR);
	float cosPitch = cos(acP);
	float sinPitch = sin(acP);
	
	// Tilt compensation
	float Xh = mag.XAxis * cosPitch + mag.ZAxis * sinPitch;
	float Yh = mag.XAxis * sinRoll * sinPitch + mag.YAxis * cosRoll - mag.ZAxis * sinRoll * cosPitch;
	
	float heading = atan2(Yh, Xh);
	return heading;
}


float SensorsClass::noTiltCompensate(Vector mag)
{
	float heading = atan2(mag.YAxis, mag.XAxis);
	return heading;
}


float SensorsClass::correctAngle(float heading)
{
	if (heading < 0) { heading += 2 * PI; }
	if (heading > 2 * PI) { heading -= 2 * PI; }

	return heading;
}





