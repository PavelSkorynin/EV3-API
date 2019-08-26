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

		enum class ButtonID : char {
			UP = 0,
			ENTER = 1,
			DOWN = 2,
			RIGHT = 3,
			LEFT = 4,
			ESCAPE = 5,
		};

		EV3();
		~EV3();

		/**
		 * Метод для получения доступа к датчику. Новый экземпляр каждый раз не
		 * создаётся. Вместо этого все запрошенные когда-либо датчики хранятся
		 * в классе EV3.
		 * @param port номер порта на блоке EV3
		 * @param mode типа датчика
		 * @return Умный указатель на датчик
		 */
		std::shared_ptr<Sensor> getSensor(Sensor::Port port, Sensor::Mode mode);
		std::shared_ptr<Motor> getMotor(Motor::Port port);

		/**
		 * Timestamp from init() in seconds
		 */
		float timestamp();
		void wait(float seconds);

		template<typename... Args>
		bool lcdTextf(Color color, short x, short y, const char *fmt, Args... args) {
			return LcdTextf((char)color, x, y, fmt, args...);
		}
		template<typename... Args>
		int lcdPrintf(Color color, const char * fmt, Args... args) {
			return LcdPrintf((char)color, fmt, args...);
		}
		void runLoop(std::function<bool(float)> update);

		bool isButtonDown(const ButtonID & buttonId);

		void updateInputs(float timestampSeconds);
		void updateOutputs(float timestampSeconds);
	private:
		std::map<Sensor::Port, std::shared_ptr<Sensor>> sensors;
		std::map<Motor::Port, std::shared_ptr<Motor>> motors;

		float zeroTimestamp;

		static const int buttonsCount = 6;
		float buttonStateChangingTimestamp[buttonsCount];
		bool buttonIsDown[buttonsCount];
	};
}



#endif /* EV3_H_ */
