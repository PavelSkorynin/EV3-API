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
	, detectedColor(NO_COLOR)
	, completed(false)
	, blackVThreshold(10)
	, whiteSThreshold(20)
	, whiteVThreshold(60) {

}

void GetColorProcess::onCompleted(float secondsFromStart) {
	Process::onCompleted(secondsFromStart);
	// среди найденных цветов ищем наиболее встречаемый
	int maxCount = 0;
	detectedColor = modes.size() - 1 - NO_COLOR;
	for (size_t i = 0; i < modes.size(); ++i) {
		if (modes[i] > maxCount) {
			maxCount = modes[i];
			detectedColor = i;
		}
	}
	if (detectedColor >= (int)colors.size()) {
		detectedColor = (int)colors.size() - 1 - detectedColor;
	}
}

void GetColorProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);

	auto hsv = colorSensor->getHSVColor();

	if (hsv.h == 0 && hsv.s == 0 && hsv.v == 0) {
		modes[colors.size() - 1 - NO_COLOR]++;
	} else if (hsv.v < blackVThreshold) {
		modes[colors.size() - 1 - BLACK_COLOR]++;
	} else if (hsv.s < whiteSThreshold && hsv.v > whiteVThreshold) {
		modes[colors.size() - 1 - WHITE_COLOR]++;
	} else {
		int minDistance = 1000;
		int colorIndex = 0;
		for (auto color : colors) {
			// ищем, к какому цвету ближе всего увиденный
			// находим минимальное расстояние от h до цвета на циклической шкале
			int dist1 = abs(hsv.h - color);
			int dist2 = abs(hsv.h - color + 360);
			int dist3 = abs(hsv.h - color - 360);
			int minDist = std::min(dist1, std::min(dist2, dist3));
			if (minDist < minDistance) {
				colorIndex = color;
				minDistance = minDist;
			}
		}

		modes[colorIndex]++;
	}

	completed = secondsFromStart - startTimestamp >= duration;
}

void GetColorProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	modes.clear();
	modes.resize(colors.size() + 3, 0);
	startTimestamp = secondsFromStart;
}

bool GetColorProcess::isCompleted() const {
	return completed;
}

int GetColorProcess::getColor() const {
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
