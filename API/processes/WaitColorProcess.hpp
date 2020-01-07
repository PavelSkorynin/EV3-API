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
	explicit WaitColorProcess(const std::shared_ptr<ColorSensor> &colorSensor);
	virtual ~WaitColorProcess() = default;

	virtual bool isCompleted() const;
	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;

	/**
	 * Установить минимальное время, в течение которого должен быть виден цвет, чтобы процесс завершился
	 * @param timeout время в секундах, значение по умолчанию 0.01 сек
	 */
	void setTimeout(float timeout);
protected:
	std::shared_ptr<ColorSensor> colorSensor;
	float lastNoColorTimestamp;
	bool foundColor;
	float timeout;
};

}
