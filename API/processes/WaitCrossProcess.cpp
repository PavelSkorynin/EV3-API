/*
 * WaitCrossProcess.cpp
 *
 *  Created on: 30 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "WaitCrossProcess.hpp"

namespace ev3 {

WaitCrossProcess::WaitCrossProcess(MotorPtr leftMotor, MotorPtr rightMotor, SensorPtr leftLight, SensorPtr rightLight)
	: leftMotor(leftMotor)
	, rightMotor(rightMotor)
	, leftLight(leftLight)
	, rightLight(rightLight)
	, foundCross(false)
	, prevLeftEncoder(0)
	, prevRightEncoder(0)
	, deltaThreshold(-30)
	, meanThreshold(50)
{

}

void WaitCrossProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	// инициализация окна с данными датчиков
	prevLeftEncoder = leftMotor->getEncoder();
	prevRightEncoder = rightMotor->getEncoder();
	for (int i = 0; i < WINDOW_WIDTH; ++i) {
		leftLightValues[i] = leftLight->getValue();
		rightLightValues[i] = rightLight->getValue();
	}
}

void WaitCrossProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	foundCross = checkSensor(leftLight, leftMotor, leftLightValues, prevLeftEncoder) ||
			checkSensor(rightLight, rightMotor, rightLightValues, prevRightEncoder);
}

bool WaitCrossProcess::isCompleted() const {
	return foundCross;
}

void WaitCrossProcess::setDeltaThreshold(int deltaThreshold) {
	this->deltaThreshold = deltaThreshold;
}

void WaitCrossProcess::setMeanThreshold(int deltaThreshold) {
	this->meanThreshold = meanThreshold;
}

bool WaitCrossProcess::checkSensor(SensorPtr lightSensor, MotorPtr motor, int lightValues[WINDOW_WIDTH], int &prevEncoder) {
	int encoder = motor->getEncoder();
	for (int i = prevEncoder + 1; i <= encoder; ++i) {
		lightValues[i % WINDOW_WIDTH] = lightSensor->getValue();
	}
	prevEncoder = encoder;
	if (prevEncoder < 0) {
		prevEncoder -= (prevEncoder / WINDOW_WIDTH - 1) * WINDOW_WIDTH;
	}
	int delta = lightValues[prevEncoder % WINDOW_WIDTH] - lightValues[(prevEncoder + 1) % WINDOW_WIDTH];
	if ((meanThreshold < 0 && delta < meanThreshold)
			|| (meanThreshold >= 0 && meanThreshold < delta)) {
		// яркость снизилась на meanThreshold за WINDOW_WIDTH градусов енкодера
		int sum = 0;
		for (int i = 0; i < WINDOW_WIDTH; ++i) {
			sum += lightValues[i];
		}
		if (sum / WINDOW_WIDTH < meanThreshold) {
			// среднее значение в окне не превышает meanThreshold
			return true;
		}
	}
	return false;
}

}
