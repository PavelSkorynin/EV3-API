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
	GetColorProcess(const std::shared_ptr<ColorSensor> &colorSensor, std::vector<int> colors, float duration = 0.1f);
	virtual ~GetColorProcess() = default;

	virtual void onCompleted(float secondsFromStart) override;
	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual bool isCompleted() const;

	/**
	 * Возвращает наиболее встречаемый цвет. Значения >= 0 соответствуют индексам в массиве colors, переданном в кострукторе
	 * Отрицательные значения соответствуют предопределённым цветам:
	 * -1 - чёрный
	 * -2 - белый
	 * -3 - нет цвета (объект далеко от датчика цвета)
	 * @return найденный цвет
	 */
	int getColor() const;

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
	std::vector<int> colors;
	float duration;

	// мода
	std::vector<int> modes;
	float startTimestamp;
	int detectedColor;
	bool completed;

	unsigned char blackVThreshold;
	unsigned char whiteSThreshold;
	unsigned char whiteVThreshold;

};

}
