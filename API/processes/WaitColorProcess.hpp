/*
 * WaitColorProcess.hpp
 *
 *  Created on: 2 янв. 2020 г.
 *      Author: Pavel Skorynin
 */
#pragma once

#include <Process.h>
#include <Sensor.h>

namespace ev3 {

class WaitColorProcess: public Process {
public:
	WaitColorProcess(const std::shared_ptr<ColorSensor> &colorSensor);
	virtual ~WaitColorProcess() = default;

	virtual bool isCompleted() const;
	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;

protected:
	std::shared_ptr<ColorSensor> colorSensor;
	float lastNoColorTimestamp;
	bool foundColor;
};

}
