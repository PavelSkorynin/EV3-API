/*
 * Devices.cpp
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#include "Devices.h"

namespace ev3 {

	Devices Devices::instance = Devices();

	std::shared_ptr<Sensor> Devices::getSensor(Sensor::Port port) {
		auto sensorPtr = instance.sensors[port];
		if (!sensorPtr) {
			sensorPtr.reset(new Sensor(port));
		}
		return sensorPtr;
	}

	std::shared_ptr<Motor> Devices::getMotor(Motor::Port port) {
		auto motorPtr = instance.motors[port];
		if (!motorPtr) {
			motorPtr.reset(new Motor(port));
		}
		return motorPtr;
	}

	void Devices::updateInputs() {
		for (const auto& pair : instance.sensors) {
			pair.second->updateInputs();
		}
		for (const auto& pair : instance.motors) {
			pair.second->updateInputs();
		}
	}

	void Devices::updateOutputs() {
		for (const auto& pair : instance.sensors) {
			pair.second->updateOutputs();
		}
		for (const auto& pair : instance.motors) {
			pair.second->updateOutputs();
		}
	}

}


