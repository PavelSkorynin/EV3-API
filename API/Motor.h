/*
 * Motor.h
 *
 *  Created on: 09 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "core/ev3_constants.h"
#include "Device.h"
#include "Wire.h"

#include <memory>

namespace ev3 {

class Motor : public Device {
public:
	enum class Port : uint8_t {
		A = OUT_A,
		B = OUT_B,
		C = OUT_C,
		D = OUT_D
	};

	enum class Direction {
		FORWARD = OUT_FWD,
		BACKWARD = OUT_REV
	};

	~Motor();

	inline Port getPort() const {
		return port;
	}

	inline Direction getDirection() const {
		return direction;
	}
	void setDirection(const Direction & direction);

	WireI getActualSpeed() const;
	WireI getEncoder() const;
	WireI getTacho() const;

	void setPower(const WireI & output);
	void setSpeed(const WireI & output);

	void resetEncoder();
	/**
	 * Maximum acceleration for this motor. Default value 2500.
	 * Valid values lies in interval ~[1000, 10000]
	 */
	void setMaxAccelleration(float maxAcceleration);

	void updateInputs(float timestampSeconds) override;
	void updateOutputs(float timestampSeconds) override;

protected:
	Port port;
	Direction direction;
	float maxAcceleration;
	// max detected speed; used to calibrate power
	float maxSpeed;
	// actual speed measured at lastTimestamp
	float lastSpeed;
	float lastTimestamp;

	int8_t actualSpeed;
	int zeroEncoder;
	int encoder;
	int tacho;

	WireI speedInput;
	WireI encoderInput;
	WireI tachoInput;
	std::shared_ptr<WireI> powerOutput;
	std::shared_ptr<WireI> speedOutput;

	Motor(Port port);

	friend class EV3;
};

} /* namespace ev3 */

#endif /* MOTOR_H_ */
