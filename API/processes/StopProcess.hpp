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

protected:
	MotorPtr motor;
};

} /* namespace ev3 */

