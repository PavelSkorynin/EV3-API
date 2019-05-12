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

#include "core/ev3_lcd.h"

#include <map>

namespace ev3 {

	class EV3 {
	public:
		enum class Color : char {
			BLACK = 1,
			WHITE = 0,
		};


		~EV3();

		static std::shared_ptr<Sensor> getSensor(Sensor::Port port, Sensor::Mode);
		static std::shared_ptr<Motor> getMotor(Motor::Port port);

		static void init();
		static void deinit();

		/**
		 * Timestamp from init() in seconds
		 */
		static float timestamp();
		static void wait(float seconds);

		template<typename... Args>
		static bool lcdTextf(Color color, short x, short y, const char *fmt, Args... args) {
			return LcdTextf((char)color, x, y, fmt, args...);
		}
		template<typename... Args>
		static int lcdPrintf(Color color, const char * fmt, Args... args) {
			return LcdPrintf((char)color, fmt, args...);
		}

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
