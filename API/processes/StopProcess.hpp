/*
 * StopProcess.hpp
 *
 *  Created on: 6 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>
#include <Motor.h>

namespace ev3 {

class StopProcess: public Process {
public:
	explicit StopProcess(const MotorPtr &motor);
	virtual ~StopProcess() = default;

	virtual void onStarted(float secondsFromStart) override;
	virtual bool isCompleted() const;

	/**
	 * Установить скорость на моторе (actual speed), при которой завершается процесс
	 * @param speedThreshold минимальная скорость, значение по умолчанию 3
	 */
	void setSpeedThreshold(int speedThreshold);

protected:
	MotorPtr motor;
	int speedThreshold;
};

} /* namespace ev3 */

