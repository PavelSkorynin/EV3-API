/*
 * GetColorProcess.cpp
 *
 *  Created on: 6 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include "GetColorProcess.hpp"

namespace ev3 {

GetColorProcess::GetColorProcess(const std::shared_ptr<ColorSensor> &colorSensor, std::vector<int> colors, float duration)
	: colorSensor(colorSensor)
	, colors(std::move(colors))
	, duration(duration)
	, modes()
	, startTimestamp(0)
	, completed(false) {

}

void GetColorProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	modes.clear();
	modes.resize(colors.size() + 3, 0);
	startTimestamp = secondsFromStart;
}

void GetColorProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);

	auto colorIndex = colorSensor->getColorIndex(colors, blackVThreshold, whiteSThreshold, whiteVThreshold);
	if (colorIndex < 0) {
		colorIndex = colors.size() - 1 - colorIndex;
	}
	modes[colorIndex]++;
	completed = (secondsFromStart - startTimestamp >= duration);
}

bool GetColorProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	return completed;
}

int GetColorProcess::getColor() const {
	// среди найденных цветов ищем наиболее встречаемый
	int maxCount = 0;
	int detectedColor = modes.size() - 1 - NO_COLOR;
	for (size_t i = 0; i < modes.size(); ++i) {
		if (modes[i] > maxCount) {
			maxCount = modes[i];
			detectedColor = i;
		}
	}
	if (detectedColor >= (int)colors.size()) {
		detectedColor = (int)colors.size() - 1 - detectedColor;
	}
	return detectedColor;
}

void GetColorProcess::setBlackThresholds(unsigned char vThreshold) {
	blackVThreshold = vThreshold;
}

void GetColorProcess::setWhiteThresholds(unsigned char sThreshold, unsigned char vThreshold) {
	whiteSThreshold = sThreshold;
	whiteVThreshold = vThreshold;
}

}
