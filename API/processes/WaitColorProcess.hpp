/*
 * WaitColorProcess.hpp
 *
 *  Created on: 2 янв. 2020 г.
 *      Author: Pavel Skorynin
 */
#pragma once

#include <Process.h>
#include <Sensor.h>

#include <vector>

namespace ev3 {

class WaitColorProcess: public virtual Process {
public:
	/**
	 * Ожидает любого цвета, в том числе белый и чёрный
	 */
	explicit WaitColorProcess(const std::shared_ptr<ColorSensor> &colorSensor);
	/**
	 * Ожидает конкретный цвет, в том числе белый и чёрный
	 */
	explicit WaitColorProcess(const std::shared_ptr<ColorSensor> &colorSensor, std::vector<int> colors, int colorToWait);

	virtual bool isCompleted(float secondsFromStart) override;
	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;

	/**
	 * Установить минимальное время, в течение которого должен быть виден цвет, чтобы процесс завершился
	 * @param timeout время в секундах, значение по умолчанию 0.01 сек
	 */
	void setTimeout(float timeout);
	/**
	 * Пороговые значения для чёрного цвета. Чёрным считается цвет, у которого компонента V меньше заданного значения
	 * @param vThreshold порог для Value, по умолчанию 10
	 */
	void setBlackThresholds(unsigned char vThreshold);
	/**
	 * Пороговые значения для белого цвета. Белым считается цвет, у которого компонента S меньше заданного значения и
	 * V больше определённого значения
	 * @param sThreshold порог для Saturation, по умолчанию 20
	 * @param vThreshold порог для Value, по умолчанию 60
	 */
	void setWhiteThresholds(unsigned char sThreshold, unsigned char vThreshold);

protected:
	std::shared_ptr<ColorSensor> colorSensor;
	float lastNoColorTimestamp;
	bool foundColor;
	float timeout;
	std::vector<int> colors;
	int colorToWait;

	unsigned char blackVThreshold = 10;
	unsigned char whiteSThreshold = 20;
	unsigned char whiteVThreshold = 60;
};

}
