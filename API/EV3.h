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

/**
 * Основной класс для взаимодействия с блоком EV3. Следует создавать только один экземпляр на программу.
 */
	class EV3 {
	public:
		/**
		 * Цвета, используемые при выводе на экран
		 */
		enum class Color : char {
			BLACK = 1,//!< BLACK
			WHITE = 0,//!< WHITE
		};

		/**
		 * Идентификатор кнопки на блоке EV3
		 */
		enum class ButtonID : char {
			UP = 0,    //!< UP
			ENTER = 1, //!< ENTER
			DOWN = 2,  //!< DOWN
			RIGHT = 3, //!< RIGHT
			LEFT = 4,  //!< LEFT
			ESCAPE = 5,//!< ESCAPE
		};

		EV3();
		~EV3();

		/**
		 * Метод для получения датчика, подключенного к блоку EV3.
		 * При повторном запросе новый экземпляр класса Sensor не создаётся, вместо это возвращается
		 * ранее созданный.
		 * @param port номер порта на блоке EV3, к которому подключен датчик
		 * @param mode тип и режим работы датчика
		 * @return умный указатель на датчик
		 */
		std::shared_ptr<Sensor> getSensor(Sensor::Port port, Sensor::Mode mode);

		/**
		 * Метод для получения мотора, подключенного к блоку EV3.
		 * При повторном запросе новый экземпляр класса Motor не создаётся, вместо это возвращается
		 * ранее созданный.
		 * @param port номер порта на блоке EV3, к которому подключен мотор
		 * @return умный указатель на мотор
		 */
		std::shared_ptr<Motor> getMotor(Motor::Port port);

		/**
		 * Метод для получения времени с начала работы с блоком EV3 (создания экземпляра класса)
		 * @return время в секундах
		 */
		float timestamp();
		/**
		 * Ожидание в течение определённого времени. При этом происходит обновление входных и выходных данных
		 * @param seconds время в секундах
		 */
		void wait(float seconds);

		/**
		 * Вывод на экран текста
		 * @param color цвет
		 * @param x координата x
		 * @param y координата y
		 * @param format строка с форматированным выводом
		 * @param args аргументы форматированного вывода
		 * @return true в случае успешного вывода на экран
		 */
		template<typename... Args>
		bool lcdTextf(Color color, short x, short y, const char *format, Args... args) {
			return LcdTextf((char)color, x, y, format, args...);
		}
		/**
		 * Последовательный вывод на экран текста
		 * @param color цвет
		 * @param format строка с форматированным выводом
		 * @param args аргументы форматированного вывода
		 * @return true в случае успешного вывода на экран
		 */
		template<typename... Args>
		int lcdPrintf(Color color, const char *format, Args... args) {
			return LcdPrintf((char)color, format, args...);
		}
		/**
		 * Запускает цикл на синхронное выполнение в текущем потоке. Остановка происходит,
		 * если передаваемая функция возвращает false.
		 * Внутри цикла происходит обновление входных и выходных данных.
		 * @param update функция, вызываемая на каждой итерации цикла. На вход функции передаётся
		 * текущее время в секундах
		 */
		void runLoop(std::function<bool(float)> update);

		/**
		 * Проверка, нажата ли какая-либо кнопка на блоке
		 * @param buttonId идентификатор кнопки
		 * @return true, если кнопка нажата
		 */
		bool isButtonDown(const ButtonID & buttonId);

		/**
		 * Обновляет все входные данные (энкодеры моторов, показания датчиков, состояние кнопок)
		 * @param timestampSeconds текущее время в секундах
		 */
		void updateInputs(float timestampSeconds);
		/**
		 * Обновляет все выходные данные (мощность моторов)
		 * @param timestampSeconds текущее время в секундах
		 */
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
