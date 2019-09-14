/*
 * EV3.cpp
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#include "EV3.h"

#include "core/ev3_core.h"
#include <chrono>
#include <thread>

namespace ev3 {

	const float debounceTime = 0.01f;

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

	float EV3::timestamp() {
		return current_timestamp() - zeroTimestamp;
	}

	void EV3::wait(float seconds) {
		float timestamp = ev3::EV3::timestamp();
		float startTimestamp = timestamp;
		while (timestamp - startTimestamp < seconds) {
			timestamp = ev3::EV3::timestamp();
			updateInputs(timestamp);
			updateOutputs(timestamp);
		}
	}

	void EV3::runLoop(const std::function<bool(float)> &update) {
		auto process = std::make_shared<LambdaProcess>(update);
		runProcess(std::dynamic_pointer_cast<Process>(process));
	}

	void EV3::runProcess(const std::shared_ptr<Process> &process) {
		float timestamp;
		while (!process->isComplete()) {
			timestamp = this->timestamp();
			updateInputs(timestamp);
			process->update(timestamp);
			updateOutputs(timestamp);
		}
	}

	bool EV3::isButtonDown(const ButtonID & buttonId) {
		return buttonIsDown[(int)buttonId];
	}

	std::shared_ptr<Sensor> EV3::getSensor(Sensor::Port port, Sensor::Mode mode) {
		auto& sensorPtr = sensors[port];
		if (!sensorPtr) {
			sensorPtr.reset(new Sensor(port));
		}
		sensorPtr->setMode(mode);
		return sensorPtr;
	}

	std::shared_ptr<Motor> EV3::getMotor(Motor::Port port) {
		auto& motorPtr = motors[port];
		if (!motorPtr) {
			motorPtr.reset(new Motor(port));
		}
		return motorPtr;
	}

	void EV3::updateInputs(float timestampSeconds) {
		for (auto& pair : sensors) {
			if (pair.second) {
				pair.second->updateInputs(timestampSeconds);
			}
		}
		for (auto& pair : motors) {
			if (pair.second) {
				pair.second->updateInputs(timestampSeconds);
			}
		}

		auto buttonsState = CheckButtonsNoWait();
		for (int buttonId = 0; buttonId < buttonsCount; ++buttonId) {
			if (buttonIsDown[buttonId] == ((buttonsState & (1 << buttonId)) != 0)) {
				buttonStateChangingTimestamp[buttonId] = timestampSeconds;
			}

			if (timestampSeconds - buttonStateChangingTimestamp[buttonId] > debounceTime) {
				buttonStateChangingTimestamp[buttonId] = timestampSeconds;
				buttonIsDown[buttonId] = !buttonIsDown[buttonId];
			}
		}

		if (buttonIsDown[(int)ButtonID::ESCAPE]) {
			FreeEV3();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			exit(0);
		}
	}

	void EV3::updateOutputs(float timestampSeconds) {
		for (auto& pair : sensors) {
			if (pair.second) {
				pair.second->updateOutputs(timestampSeconds);
			}
		}
		for (auto& pair : motors) {
			if (pair.second) {
				pair.second->updateOutputs(timestampSeconds);
			}
		}
	}

}


