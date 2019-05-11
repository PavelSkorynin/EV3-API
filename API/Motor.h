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

	~Motor();

	inline Port getPort() const {
		return port;
	}

	std::weak_ptr<WireI> getActualSpeed() const;
	std::weak_ptr<WireI> getEncoder() const;

	void setPower(const std::weak_ptr<WireI> & output);
	void setSpeed(const std::weak_ptr<WireI> & output);

	void updateInputs() override;
	void updateOutputs() override;

protected:
	Port port;

	int actualSpeed;
	int encoder;

	std::shared_ptr<WireI> speedInput;
	std::shared_ptr<WireI> encoderInput;
	std::weak_ptr<WireI> powerOutput;
	std::weak_ptr<WireI> speedOutput;

	Motor(Port port);

	friend class Devices;
};

} /* namespace ev3 */

#endif /* MOTOR_H_ */
