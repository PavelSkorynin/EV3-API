/*
 * EV3.cpp
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#include "EV3.h"

#include "core/ev3_core.h"

namespace ev3 {

	std::shared_ptr<EV3> EV3::instance = std::shared_ptr<EV3>();

	float current_timestamp() {
	    struct timeval te;
	    gettimeofday(&te, NULL); // get current time
	    long long usec = te.tv_sec*1000000LL + te.tv_usec; // calculate nanoseconds
	    return usec / 10e9f;
	}

	EV3::EV3()
		: zeroTimestamp(current_timestamp())
	{
		InitEV3();
	}

	EV3::~EV3() {
		FreeEV3();
	}

	void EV3::init() {
		if (!instance) {
			instance = std::shared_ptr<EV3>(new EV3());
		}
	}

	void EV3::deinit() {
		if (instance) {
			instance.reset();
		}
	}

	float EV3::timestamp() {
		return current_timestamp() - instance->zeroTimestamp;
	}

	std::shared_ptr<Sensor> EV3::getSensor(Sensor::Port port, Sensor::Mode mode) {
		auto sensorPtr = instance->sensors[port];
		if (!sensorPtr) {
			sensorPtr.reset(new Sensor(port));
		}
		if (sensorPtr->getMode() != mode) {
			sensorPtr->setMode(mode);
		}
		return sensorPtr;
	}

	std::shared_ptr<Motor> EV3::getMotor(Motor::Port port) {
		auto motorPtr = instance->motors[port];
		if (!motorPtr) {
			motorPtr.reset(new Motor(port));
		}
		return motorPtr;
	}

	void EV3::updateInputs() {
		for (const auto& pair : instance->sensors) {
			pair.second->updateInputs();
		}
		for (const auto& pair : instance->motors) {
			pair.second->updateInputs();
		}
	}

	void EV3::updateOutputs() {
		for (const auto& pair : instance->sensors) {
			pair.second->updateOutputs();
		}
		for (const auto& pair : instance->motors) {
			pair.second->updateOutputs();
		}
	}

}


