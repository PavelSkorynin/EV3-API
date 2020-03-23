/*
 * MoveByEncoderAndStopProcess.cpp
 *
 *  Created on: 30 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include <cmath>
#include <algorithm>
#include "EV3Math.h"
#include "../core/ev3_output.h"
#include "MoveByEncoderAndStopProcess.hpp"

namespace ev3 {

MoveByEncoderAndStopProcess::MoveByEncoderAndStopProcess(MotorPtr motor, int deltaEncoder, int power)
	: motor(std::move(motor))
	, deltaEncoder(deltaEncoder)
	, power(power)
	, startTime(0) {
}


void MoveByEncoderAndStopProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
}

void MoveByEncoderAndStopProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);

	auto absPower = deltaEncoder * power < 0 ? -abs(power) : abs(power);
	auto absEncoder = abs(deltaEncoder);
	auto ramp = min((int)(0.4f * absEncoder), 200);

	OutputStepPowerEx(static_cast<uint8_t>(motor->getPort()), absPower, ramp, absEncoder - 2 * ramp, ramp, true, OWNER_NONE);
	startTime = secondsFromStart;
}

void MoveByEncoderAndStopProcess::onCompleted(float secondsFromStart) {
	Process::onCompleted(secondsFromStart);
}

bool MoveByEncoderAndStopProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);

	// wait some time to update busy status
	return secondsFromStart - startTime > 0.001 && !motor->isBusy();
}

} /* namespace ev3 */
