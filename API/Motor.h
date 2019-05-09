/*
 * Motor.h
 *
 *  Created on: 09 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "ev3_constants.h"

#include <memory>

namespace ev3 {

class Output;

class Motor {
public:
	enum Port {
		A = OUT_A,
		B = OUT_B,
		C = OUT_C,
		D = OUT_D
	};

	Motor(Port port);
	~Motor();

	std::weak_ptr<Output> getActualPower() const;
	std::weak_ptr<Output> getEncoder() const;

	void setPower(std::weak_ptr<Output> const &output);
};

} /* namespace ev3 */

#endif /* MOTOR_H_ */
