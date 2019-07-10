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

	const float debounceTime = 0.01f;

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
		, buttonStateChangingTimestamp {0, 0, 0, 0, 0, 0}
		, buttonIsDown {false, false, false, false, false, false}
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

	bool EV3::isButtonDown(const ButtonID & buttonId) {
		return instance->buttonIsDown[(int)buttonId];
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

		auto buttonsState = CheckButtonsNoWait();
		for (int buttonId = 0; buttonId < buttonsCount; ++buttonId) {
			if (instance->buttonIsDown[buttonId] == ((buttonsState & (1 << buttonId)) != 0)) {
				instance->buttonStateChangingTimestamp[buttonId] = timestampSeconds;
			}

			if (timestampSeconds - instance->buttonStateChangingTimestamp[buttonId] > debounceTime) {
				instance->buttonStateChangingTimestamp[buttonId] = timestampSeconds;
				instance->buttonIsDown[buttonId] = !instance->buttonIsDown[buttonId];
			}
		}

		if (instance->buttonIsDown[(int)ButtonID::ESCAPE]) {
			deinit();
			exit(0);
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


