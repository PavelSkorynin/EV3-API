/*
 * WaitNoColorProcess.cpp
 *
 *  Created on: 6 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include "WaitNoColorProcess.hpp"

namespace ev3 {

WaitNoColorProcess::WaitNoColorProcess(const std::shared_ptr<ColorSensor> &colorSensor)
	: colorSensor(colorSensor)
	, lastColorTimestamp(0)
	, foundNoColor(false)
	, timeout(0.01f) {
}

void WaitNoColorProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);

	lastColorTimestamp = secondsFromStart;
}

void WaitNoColorProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);

	auto rgb = colorSensor->getRGBColor();
	if (rgb.r != 0 || rgb.g != 0 || rgb.b != 0) {
		lastColorTimestamp = secondsFromStart;
	}

	foundNoColor = (secondsFromStart - lastColorTimestamp >= timeout);
}

bool WaitNoColorProcess::isCompleted() const {
	return foundNoColor;
}

void WaitNoColorProcess::setTimeout(float timeout) {
	this->timeout = timeout;
}


}
