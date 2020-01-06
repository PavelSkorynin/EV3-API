/*
 * WaitLineProcess.hpp
 *
 *  Created on: 2 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>
#include <Sensor.h>

namespace ev3 {

class WaitLineProcess: public Process {
public:
	explicit WaitLineProcess(const SensorPtr &lightSensor);
	virtual ~WaitLineProcess() = default;

	virtual bool isCompleted() const;

	void setThreshold(int threshold);

protected:
	SensorPtr lightSensor;

	// default = 50
	int threshold;
};

}
