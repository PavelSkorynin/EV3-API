//============================================================================
// Name        : ev3libDemo.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++
//============================================================================

#include <EV3.h>
#include <PD.h>

#include "CrossCheck.h"

int main()
{
	ev3::EV3::init();

	// 1. Setup devices
	auto light2 = ev3::EV3::getSensor(ev3::Sensor::Port::P2, ev3::Sensor::Mode::COLOR_REFLECT);
	auto light1 = ev3::EV3::getSensor(ev3::Sensor::Port::P1, ev3::Sensor::Mode::COLOR_REFLECT);

	auto motorA = ev3::EV3::getMotor(ev3::Motor::Port::A);
	auto motorB = ev3::EV3::getMotor(ev3::Motor::Port::B);
	motorA->setDirection(ev3::Motor::Direction::BACKWARD);
	motorB->setDirection(ev3::Motor::Direction::BACKWARD);
	motorA->setPower(0);
	motorB->setPower(0);
	motorA->resetEncoder();
	motorB->resetEncoder();

	// 2. Setup wires and processes
	ev3::PD pd(0.6f, 10.0f);

	ev3::CrossCheck crossCheck(light1->getValue(), light2->getValue(), (motorA->getEncoder() + motorB->getEncoder()) / 2);

	pd.setError(light2->getValue() - light1->getValue());

	const int MAX_POWER = 100;

	auto outputA = ev3::WireI([power = pd.getPower()]{
		if (power > 0) {
			return (int)(MAX_POWER - power * 2);
		} else {
			return MAX_POWER;
		}
	});

	auto outputB = ev3::WireI([power = pd.getPower()]{
		if (power < 0) {
			return (int)(MAX_POWER + power * 2);
		} else {
			return MAX_POWER;
		}
	});
	motorA->setMaxAccelleration(2500.0f);
	motorB->setMaxAccelleration(2500.0f);
	motorA->setPower(outputA);
	motorB->setPower(outputB);
	auto encoderA = motorA->getEncoder();
	auto encoderB = motorB->getEncoder();

	int maxSpeedA = 0;
	int maxSpeedB = 0;

	auto timestamp = ev3::EV3::timestamp();
	// 3. main loop

	for (int i = 0; i < 5; ++i) {

		motorA->setPower(outputA);
		motorB->setPower(outputB);
		motorA->resetEncoder();
		motorB->resetEncoder();
		crossCheck.resetCounters();
		while(true) {
			timestamp = ev3::EV3::timestamp();
			ev3::EV3::lcdTextf(ev3::EV3::Color::BLACK, 0, 0, "%0.4f", timestamp);

			ev3::EV3::updateInputs(timestamp);

			pd.update(timestamp);
			crossCheck.update(timestamp);

			ev3::EV3::updateOutputs(timestamp);

			ev3::EV3::lcdTextf(ev3::EV3::Color::BLACK, 0, 10, "%2d %2d %2d",
					crossCheck.getLeftCrossCount(),
					crossCheck.getRightCrossCount(),
					crossCheck.getXCrossCount());
			int speedA = motorA->getActualSpeed();
			int speedB = motorB->getActualSpeed();
			maxSpeedA = std::max(maxSpeedA, speedA);
			maxSpeedB = std::max(maxSpeedB, speedB);
			ev3::EV3::lcdTextf(ev3::EV3::Color::BLACK, 0, 20, "%4d %4d %4d %4d", speedA, speedB, maxSpeedA, maxSpeedB);
			ev3::EV3::lcdTextf(ev3::EV3::Color::BLACK, 0, 30, "%4d %4d", (int)motorA->getTacho(), (int)motorB->getTacho());
			ev3::EV3::lcdTextf(ev3::EV3::Color::BLACK, 0, 40, "%4d %4d", (int)motorA->getEncoder(), (int)motorB->getEncoder());
			if (encoderA + encoderB >= 5000 || timestamp > 10.0f
					|| crossCheck.getTotalCrossCount() >= 1
					) {
				break;
			}
		}

		motorA->setPower(0);
		motorB->setPower(0);

		// stop
		while (motorA->getActualSpeed() > 0 && motorB->getActualSpeed() > 0) {
			timestamp = ev3::EV3::timestamp();
			ev3::EV3::updateInputs(timestamp);
			ev3::EV3::updateOutputs(timestamp);
		}
	}

	motorA->setPower(0);
	motorB->setPower(0);

	ev3::EV3::wait(3);

	ev3::EV3::deinit();

	return 0;
}

