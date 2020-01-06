/*
 * GetColorProcess.hpp
 *
 *  Created on: 6 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>
#include <Sensor.h>
#include <vector>
#include <memory>

namespace ev3 {

const int BLACK_COLOR = -1;
const int WHITE_COLOR = -2;
const int NO_COLOR = -3;

class GetColorProcess : public Process {
public:
	GetColorProcess(const std::shared_ptr<ColorSensor> &colorSensor, const std::vector<int> &colors, float duration = 0.1f);
	virtual ~GetColorProcess() = default;

	virtual void onCompleted(float secondsFromStart) override;
	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual bool isCompleted() const;

	int getColor() const;

protected:
	std::shared_ptr<ColorSensor> colorSensor;
	std::vector<int> colors;
	float duration;

	// мода
	std::vector<int> modes;
	float startTimestamp;
	int detectedColor;
	bool completed;
};

}
