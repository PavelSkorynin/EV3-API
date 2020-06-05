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
#include "Process.h"

#include "core/ev3_lcd.h"

#include <string>
#include <map>
#include <functional>

namespace ev3 {

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
enum class ButtonID : uint8_t {
	UP = 0,    //!< UP
	ENTER = 1, //!< ENTER
	DOWN = 2,  //!< DOWN
	RIGHT = 3, //!< RIGHT
	LEFT = 4,  //!< LEFT
	ESCAPE = 5,//!< ESCAPE
};

enum class LEDPattern : uint8_t {
	OFF = 0,  //!< LED black pattern
	GREEN = 1,  //!< LED green pattern
	RED = 2,  //!< LED red pattern
	ORANGE = 3,  //!< LED orange pattern
	GREEN_FLASH = 4,  //!< LED green flash pattern
	RED_FLASH = 5,  //!< LED red flash pattern
	ORANGE_FLASH = 6,  //!< LED orange flash pattern
	GREEN_PULSE = 7,  //!< LED green pulse pattern
	RED_PULSE = 8,  //!< LED red pulse pattern
	ORANGE_PULSE = 9,  //!< LED orange pulse pattern
	COUNT = 10, //!< The number of LED patterns
};

/**
 * Основной класс для взаимодействия с блоком EV3. Следует создавать только один экземпляр на программу.
 */
	class EV3 {
	public:
		EV3();
		~EV3();

		/**
		 * Инициализация сенсоров. Установка режимов работы для каждого порта.
		 * @param p1 режима работы сенсора, подключенного к порту 1
		 * @param p2 режима работы сенсора, подключенного к порту 2
		 * @param p3 режима работы сенсора, подключенного к порту 3
		 * @param p4 режима работы сенсора, подключенного к порту 4
		 */
		void initSensors(Sensor::Mode p1 = Sensor::Mode::NO_SENSOR, Sensor::Mode p2 = Sensor::Mode::NO_SENSOR, Sensor::Mode p3 = Sensor::Mode::NO_SENSOR, Sensor::Mode p4 = Sensor::Mode::NO_SENSOR);

		/**
		 * Метод для получения датчика, подключенного к блоку EV3.
		 * При повторном запросе новый экземпляр класса Sensor не создаётся, вместо это возвращается
		 * ранее созданный.
		 * @param port номер порта на блоке EV3, к которому подключен датчик
		 * @param mode тип и режим работы датчика
		 * @return умный указатель на датчик
		 */
		std::shared_ptr<Sensor> getSensor(Sensor::Port port);
		/**
		 * Метод для получения подключенного к определённому порту датчика отражённого света
		 * @param port номер порта на блоке EV3, к которому подключен датчик
		 * @return умный указатель на датчик
		 */
		std::shared_ptr<ReflectedLightSensor> getReflectedLightSensor(Sensor::Port port);
		/**
		 * Метод для получения подключенного к определённому порту датчика отражённого света
		 * @param port номер порта на блоке EV3, к которому подключен датчик
		 * @return умный указатель на датчик
		 */
		std::shared_ptr<RawReflectedLightSensor> getRawReflectedLightSensor(Sensor::Port port);
		/**
		 * Метод для получения подключенного к определённому порту датчика цвета
		 * @param port номер порта на блоке EV3, к которому подключен датчик
		 * @return умный указатель на датчик
		 */
		std::shared_ptr<ColorSensor> getColorSensor(Sensor::Port port);

		/**
		 * Метод для получения "фейкового" сенсора.
		 * @param valueInput провод со значениями, которые будет выдавать датчик
		 * @return
		 */
		std::shared_ptr<FakeSensor> getFakeSensor(const ev3::WireI &valueInput);

		/**
		 * Метод для получения "фейкового" сенсора. Может использоваться для реализации движения по линии на одном датчике.
		 * @param input значение, которое будет выдавать датчик
		 * @return
		 */
		std::shared_ptr<FakeSensor> getFakeSensor(int value);

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
		 * Очистка экрана
		 * @return true в случае успешной очистки
		 */
		bool lcdClean();
		/**
		 * Воспроизводит звук определённой частоты. Не дожидается окончания
		 * @param frequency частота звука
		 * @param duration длительность звука в секундах
		 * @param volume громкость звука (от 0 до 1)
		 */
		void playSound(unsigned short frequency, float duration, float volume);
		/**
		 * Запускает цикл на синхронное выполнение в текущем потоке. Остановка происходит,
		 * если передаваемая функция возвращает false.
		 * Внутри цикла происходит обновление входных и выходных данных.
		 * @param update функция, вызываемая на каждой итерации цикла. На вход функции передаётся
		 * текущее время в секундах
		 */
		void runLoop(const std::function<bool(float)> &update);

		/**
		 * Запускает цикл на синхронное выполнение процесса в текущем потоке. Остановка происходит,
		 * когда завершается процесс.
		 * Внутри цикла происходит обновление входных и выходных данных.
		 * @param process процесс на выполнение
		 */
		template<class ProcessClass>
		void runProcess(ProcessClass *process) {
			static_assert(std::is_base_of<Process, ProcessClass>::value);
			float timestamp = this->timestamp();;
			while (!process->isCompleted(timestamp)) {
				timestamp = this->timestamp();
				updateInputs(timestamp);
				process->update(timestamp);
				updateOutputs(timestamp);
			}
			process->onCompleted(timestamp);
		}

		/**
		 * Запускает цикл на синхронное выполнение процесса в текущем потоке. Остановка происходит,
		 * когда завершается процесс.
		 * Внутри цикла происходит обновление входных и выходных данных.
		 * @param process процесс на выполнение
		 */
		template<class ProcessClass>
		void runProcess(std::shared_ptr<ProcessClass> process) {
			static_assert(std::is_base_of<Process, ProcessClass>::value);
			float timestamp = this->timestamp();;
			while (!process->isCompleted(timestamp)) {
				timestamp = this->timestamp();
				updateInputs(timestamp);
				process->update(timestamp);
				updateOutputs(timestamp);
			}
			process->onCompleted(timestamp);
		}

		/**
		 * Запускает цикл на синхронное выполнение процесса в текущем потоке. Остановка происходит,
		 * когда завершается процесс.
		 * Внутри цикла происходит обновление входных и выходных данных.
		 * @param process процесс на выполнение
		 */
		template<class ProcessClass>
		void runProcess(ProcessClass &process) {
			static_assert(std::is_base_of<Process, ProcessClass>::value);
			float timestamp = this->timestamp();;
			while (!process.isCompleted(timestamp)) {
				timestamp = this->timestamp();
				updateInputs(timestamp);
				process.update(timestamp);
				updateOutputs(timestamp);
			}
			process.onCompleted(timestamp);
		}

		/**
		 * Запускает цикл на синхронное выполнение процесса в текущем потоке. Остановка происходит,
		 * когда завершается процесс.
		 * Внутри цикла происходит обновление входных и выходных данных.
		 * @param process процесс на выполнение
		 */
		template<class ProcessClass>
		void runProcess(ProcessClass &&process) {
			static_assert(std::is_base_of<Process, ProcessClass>::value);
			float timestamp = this->timestamp();;
			while (!process.isCompleted(timestamp)) {
				timestamp = this->timestamp();
				updateInputs(timestamp);
				process.update(timestamp);
				updateOutputs(timestamp);
			}
			process.onCompleted(timestamp);
		}

		/**
		 * Проверка, нажата ли какая-либо кнопка на блоке
		 * @param buttonId идентификатор кнопки
		 * @return true, если кнопка нажата
		 */
		bool isButtonDown(ButtonID buttonId);

		/**
		 * Установить обработчик нажатия на одну из кнопок блока
		 * @param buttonId идентификатор кнопки
		 * @param buttonId onCLick обработчик нажатия
		 */
		void setOnButtonClickListener(ButtonID buttonId, const std::function<void()> &onClick);

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

		/**
		 * Возращает версию программного обеспечения, установленного на блок EV3
		 * @return версия прошивки
		 */
		std::string getHardwareVersion();

		/**
		 * Устанавливает режим работы светодиода на блоке EV3
		 * @param ledPattern режим работы
		 */
		void setLEDPattern(LEDPattern ledPattern);

		/**
		 * Возвращает текущее положение робота (X)
		 */
		float getX();

		/**
		 * Возвращает текущее положение робота (Y)
		 */
		float getY();

		/**
		 * Возвращает текущее положение робота (угол поворота в радианах)
		 */
		float getRotation();

		/**
		 * Устанавливает расстояние между колёсами (колёсная база) в градусах енкодера
		 */
		void setDistanceBetweenWheels(float encoderDistance);

		/**
		 * Устанавливает текущее положение робота (X)
		 */
		void setX(float x);

		/**
		 * Устанавливает текущее положение робота (Y)
		 */
		void setY(float y);

		/**
		 * Устанавливает текущее положение робота (угол поворота в радианах)
		 */
		void setRotation(float rotation);

	private:
		std::map<Sensor::Port, std::shared_ptr<Sensor>> sensors;
		std::map<Motor::Port, std::shared_ptr<Motor>> motors;

		float zeroTimestamp;

		static const int buttonsCount = 6;
		float buttonStateChangingTimestamp[buttonsCount];
		bool buttonIsDown[buttonsCount];
		std::vector<std::function<void()>> onButtonClickListeners;

		// encoder
		float distanceBetweenWheels;
		// encoder
		float x;
		// encoder
		float y;
		// radians
		float rotation;
		// encoder
		int prevLeftEncoder;
		// encoder
		int prevRightEncoder;
	};
}

#endif /* EV3_H_ */
