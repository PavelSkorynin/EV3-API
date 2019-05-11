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
	enum Port {
		A = OUT_A,
		B = OUT_B,
		C = OUT_C,
		D = OUT_D
	};

	enum Direction {
		FORWARD = OUT_FWD,
		BACKWRAD = OUT_REV
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

	void setPower(const WireI & output);
	void setSpeed(const WireI & output);

	void resetEncoder();

	void updateInputs() override;
	void updateOutputs() override;

protected:
	Port port;
	Direction direction;

	int actualSpeed;
	int zeroEncoder;
	int encoder;

	WireI speedInput;
	WireI encoderInput;
	std::shared_ptr<WireI> powerOutput;
	std::shared_ptr<WireI> speedOutput;

	Motor(Port port);

	friend class EV3;
};

} /* namespace ev3 */

#endif /* MOTOR_H_ */
