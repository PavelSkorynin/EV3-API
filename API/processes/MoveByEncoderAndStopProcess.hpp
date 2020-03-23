/*
 * MoveByEncoderAndStopProcess.hpp
 *
 *  Created on: 30 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>
#include <Motor.h>

namespace ev3 {

class MoveByEncoderAndStopProcess: public Process {
public:
	/*
	 * Движется к определённому значению энкодера. Останавливает мотор по заверщении.
	 */
	MoveByEncoderAndStopProcess(MotorPtr motor, int deltaEncoder, int power);
	virtual ~MoveByEncoderAndStopProcess() = default;

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual void onCompleted(float secondsFromStart) override;
	virtual bool isCompleted(float secondsFromStart) override;

protected:
	MotorPtr motor;
	int deltaEncoder;
	int power;
	float startTime;
};

} /* namespace ev3 */

