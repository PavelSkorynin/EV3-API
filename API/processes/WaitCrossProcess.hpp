/*
 * WaitCrossProcess.hpp
 *
 *  Created on: 30 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>
#include <Motor.h>
#include <Sensor.h>

namespace ev3 {

class WaitCrossProcess: public Process {
public:
	WaitCrossProcess(MotorPtr leftMotor, MotorPtr rightMotor, SensorPtr leftLight, SensorPtr rightLight);
	virtual ~WaitCrossProcess() = default;

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual bool isCompleted() const;

	// разница показаний между текущей яркостью и яркостью в начале окна, default = -30
	void setDeltaThreshold(int deltaThreshold);
	// среднее значение яркости в окне, default = 50
	void setMeanThreshold(int deltaThreshold);

protected:
	MotorPtr leftMotor;
	MotorPtr rightMotor;
	SensorPtr leftLight;
	SensorPtr rightLight;

	bool foundCross;
	int prevLeftEncoder;
	int prevRightEncoder;

	static const int WINDOW_WIDTH = 25;
	int leftLightValues[WINDOW_WIDTH];
	int rightLightValues[WINDOW_WIDTH];

	int deltaThreshold;
	int meanThreshold;

private:
	bool checkSensor(SensorPtr lightSensor, MotorPtr motor, int lightValues[WINDOW_WIDTH], int &prevEncoder);
};

}
