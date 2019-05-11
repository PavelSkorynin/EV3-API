/*
 * EV3.h
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef EV3_H_
#define EV3_H_

#include "Sensor.h"
#include "Motor.h"

#include <map>

namespace ev3 {

	class EV3 {
	public:
		~EV3();

		static std::shared_ptr<Sensor> getSensor(Sensor::Port port, Sensor::Mode);
		static std::shared_ptr<Motor> getMotor(Motor::Port port);

		static void init();
		static void deinit();

		static float timestamp();

		static void updateInputs();
		static void updateOutputs();
	private:
		EV3();

		static std::shared_ptr<EV3> instance;

		std::map<Sensor::Port, std::shared_ptr<Sensor>> sensors;
		std::map<Motor::Port, std::shared_ptr<Motor>> motors;

		float zeroTimestamp;
	};
}



#endif /* EV3_H_ */
