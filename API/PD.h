/*
 * PD.h
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef PD_H_
#define PD_H_

#include "Process.h"
#include "Wire.h"

namespace ev3 {

class PD: public virtual Process {
public:
	PD(float kp = 0.4f, float kd = 1.2f);
	~PD();

	void setError(const WireF & errorWire);
	WireF getPower();

	void update(float secondsFromStart) override;

protected:
	float kp;
	float kd;
	float lastError;
	float lastUpdateTime;
	float power;

	std::shared_ptr<WireF> errorWire;
	WireF powerWire;
};

} /* namespace ev3 */

#endif /* PD_H_ */
