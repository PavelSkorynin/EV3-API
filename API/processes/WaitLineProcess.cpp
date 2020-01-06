/*
 * WaitLineProcess.cpp
 *
 *  Created on: 2 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include "WaitLineProcess.hpp"

namespace ev3 {

WaitLineProcess::WaitLineProcess(const SensorPtr &lightSensor)
	: lightSensor(lightSensor)
	, threshold(50) {

}

bool WaitLineProcess::isCompleted() const {
	return lightSensor->getValue() < threshold;
}

void WaitLineProcess::setThreshold(int threshold) {
	this->threshold = threshold;
}

}
