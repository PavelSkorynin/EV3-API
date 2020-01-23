/*
 * WaitColorProcess.cpp
 *
 *  Created on: 2 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include "WaitColorProcess.hpp"

namespace ev3 {

const int WAIT_ANY_COLOR = -127;

WaitColorProcess::WaitColorProcess(const std::shared_ptr<ColorSensor> &colorSensor)
	: colorSensor(colorSensor)
	, lastNoColorTimestamp(0)
	, foundColor(false)
	, timeout(0.01f)
	, colorToWait(WAIT_ANY_COLOR) {
}

WaitColorProcess::WaitColorProcess(const std::shared_ptr<ColorSensor> &colorSensor, std::vector<int> colors, int colorToWait)
	: colorSensor(colorSensor)
	, lastNoColorTimestamp(0)
	, foundColor(false)
	, timeout(0.01f)
	, colors(std::move(colors))
	, colorToWait(colorToWait) {
}

void WaitColorProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);

	lastNoColorTimestamp = secondsFromStart;
}

void WaitColorProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);

	if (colorToWait == WAIT_ANY_COLOR) {
		auto rgb = colorSensor->getRGBColor();
		if (rgb.r == 0 && rgb.g == 0 && rgb.b == 0) {
			lastNoColorTimestamp = secondsFromStart;
		}
	} else {
		auto colorIndex = colorSensor->getColorIndex(colors, blackVThreshold, whiteSThreshold, whiteVThreshold);
		if (colorIndex != colorToWait) {
			lastNoColorTimestamp = secondsFromStart;
		}
	}

	foundColor = (secondsFromStart - lastNoColorTimestamp >= timeout);
}

bool WaitColorProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	return foundColor;
}

void WaitColorProcess::setTimeout(float timeout) {
	this->timeout = timeout;
}

void WaitColorProcess::setBlackThresholds(unsigned char vThreshold) {
	blackVThreshold = vThreshold;
}

void WaitColorProcess::setWhiteThresholds(unsigned char sThreshold, unsigned char vThreshold) {
	whiteSThreshold = sThreshold;
	whiteVThreshold = vThreshold;
}
}
