/*
 * EV3.cpp
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#include "EV3.h"

#include "core/ev3_core.h"
#include <chrono>

namespace ev3 {

	std::shared_ptr<EV3> EV3::instance = std::shared_ptr<EV3>();

	using namespace std::chrono;

	float current_timestamp() {
		milliseconds ms = duration_cast< milliseconds >(
		    system_clock::now().time_since_epoch()
		);
	    return ms.count() / 1000.0f;
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

	void EV3::wait(float seconds) {
		float timestamp = ev3::EV3::timestamp();
		float startTimestamp = timestamp;
		while (timestamp - startTimestamp < 3) {
			timestamp = ev3::EV3::timestamp();
			ev3::EV3::updateInputs(timestamp);
			ev3::EV3::updateOutputs(timestamp);
		}
	}

	std::shared_ptr<Sensor> EV3::getSensor(Sensor::Port port, Sensor::Mode mode) {
		auto& sensorPtr = instance->sensors[port];
		if (!sensorPtr) {
			sensorPtr.reset(new Sensor(port));
		}
		sensorPtr->setMode(mode);
		return sensorPtr;
	}

	std::shared_ptr<Motor> EV3::getMotor(Motor::Port port) {
		auto& motorPtr = instance->motors[port];
		if (!motorPtr) {
			motorPtr.reset(new Motor(port));
		}
		return motorPtr;
	}

	void EV3::updateInputs(float timestampSeconds) {
		for (auto& pair : instance->sensors) {
			pair.second->updateInputs(timestampSeconds);
		}
		for (auto& pair : instance->motors) {
			pair.second->updateInputs(timestampSeconds);
		}
	}

	void EV3::updateOutputs(float timestampSeconds) {
		for (auto& pair : instance->sensors) {
			pair.second->updateOutputs(timestampSeconds);
		}
		for (auto& pair : instance->motors) {
			pair.second->updateOutputs(timestampSeconds);
		}
	}

}


