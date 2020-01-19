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

class WaitLineProcess: public virtual Process {
public:
	explicit WaitLineProcess(const SensorPtr &lightSensor);

	virtual bool isCompleted() const;

	/**
	 * Установить порог яркости, при котором завершится процесс
	 * @param threshold, значение по умолчанию 50
	 */
	void setThreshold(int threshold);

protected:
	SensorPtr lightSensor;

	// default = 50
	int threshold;
};

}
