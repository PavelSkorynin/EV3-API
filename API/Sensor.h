/*
 * Sensor.h
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "Device.h"

#include "core/ev3_constants.h"
#include "core/ev3_sensor.h"
#include "Wire.h"

namespace ev3 {

class Sensor: public Device {
public:
	enum class Port {
		P1 = IN_1,
		P2 = IN_2,
		P3 = IN_3,
		P4 = IN_4
	};

	enum class Mode {
		NO_SENSOR = NO_SEN,		// No sensor connected / Unknown sensor
		//Touchsenor
		TOUCH = TOUCH_PRESS,	// Press

		//Lightsensor
		COLOR_REFLECT = COL_REFLECT,	// Reflect
		COLOR_AMBIENT = COL_AMBIENT,	// Ambient
		COLOR_COLOR = COL_COLOR,		// Color
		COLOR_RGB = COL_COLOR_RGB,		// RGB Color

		//Ultrasonic
		ULTRASONIC_DISTANCE_CM = US_DIST_CM,	// Dist in cm
		ULTRASONIC_DISTANCE_MM = US_DIST_MM,	// Dist in mm
		ULTRASONIC_DISTANCE_INCHES =  US_DIST_IN,	// Dist in inch

		//Gyroscope
		GYROSCOPE_ANGLE = GYRO_ANG,		// angle
		GYROSCOPE_RATE = GYRO_RATE,		// rate

		//Infrared
		INFRARED_PROXIMITY = IR_PROX,		// Proximity
		INFRARED_SEEK = IR_SEEK,		// Seek
		INFRARED_REMOTE = IR_REMOTE,	// Remote Control

		//NXT
		NXT_INFRARED_SEEK = NXT_IR_SEEKER, // Infrared Seeker
		NXT_TEMPERATURE_C = NXT_TEMP_C, 	// Temperature in C
		NXT_TEMPERATURE_F = NXT_TEMP_F, 	// Temperature in F
	};

	~Sensor();

	inline Port getPort() const {
		return port;
	}
	inline Mode getMode() const {
		return mode;
	}

	WireI getValue() const;

	void setMode(const Mode & mode);

	void updateInputs(float timestampSeconds) override;
	void updateOutputs(float timestampSeconds) override;

protected:
	Sensor(Port port);

	Port port;
	Mode mode;

	int value;

	WireI valueInput;

	friend class EV3;
};

} /* namespace ev3 */

#endif /* SENSOR_H_ */
