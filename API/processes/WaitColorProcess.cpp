/*
 * WaitColorProcess.cpp
 *
 *  Created on: 2 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include "WaitColorProcess.hpp"

namespace ev3 {

WaitColorProcess::WaitColorProcess(const std::shared_ptr<ColorSensor> &colorSensor)
	: colorSensor(colorSensor)
	, lastNoColorTimestamp(0)
	, foundColor(false)
	, timeout(0.01f) {
}

void WaitColorProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);

	lastNoColorTimestamp = secondsFromStart;
}

void WaitColorProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);

	auto rgb = colorSensor->getRGBColor();
	if (rgb.r == 0 && rgb.g == 0 && rgb.b == 0) {
		lastNoColorTimestamp = secondsFromStart;
	}

	foundColor = (secondsFromStart - lastNoColorTimestamp >= timeout);
}

bool WaitColorProcess::isCompleted() const {
	return foundColor;
}

void WaitColorProcess::setTimeout(float timeout) {
	this->timeout = timeout;
}

}
