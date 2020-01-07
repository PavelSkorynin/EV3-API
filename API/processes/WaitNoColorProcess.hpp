/*
 * WaitNoColorProcess.hpp
 *
 *  Created on: 6 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>

#include <Process.h>
#include <Sensor.h>


namespace ev3 {

class WaitNoColorProcess: public Process {
public:
	explicit WaitNoColorProcess(const std::shared_ptr<ColorSensor> &colorSensor);
	virtual ~WaitNoColorProcess() = default;

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
	float lastColorTimestamp;
	bool foundNoColor;
	float timeout;
};

}
