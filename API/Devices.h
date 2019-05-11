/*
 * Devices.h
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#include "Sensor.h"
#include "Motor.h"

#include <map>

namespace ev3 {

	class Devices {
	public:
		static std::shared_ptr<Sensor> getSensor(Sensor::Port port);
		static std::shared_ptr<Motor> getMotor(Motor::Port port);

		static void updateInputs();
		static void updateOutputs();
	private:
		Devices() {}

		static Devices instance;

		std::map<Sensor::Port, std::shared_ptr<Sensor>> sensors;
		std::map<Motor::Port, std::shared_ptr<Motor>> motors;
	};
}



#endif /* DEVICES_H_ */
