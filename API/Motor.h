/*
 * Motor.h
 *
 *  Created on: 09 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "core/ev3_constants.h"
#include "Device.h"
#include "Wire.h"

#include <memory>

namespace ev3 {

/**
 * Класс, отвечающий за взаимодействие с мотором, подключенным к блоку EV3.
 */
class Motor : public Device {
public:
	/**
	 * Порт подключения мотора
	 */
	enum class Port : uint8_t {
		A = OUT_A,//!< A
		B = OUT_B,//!< B
		C = OUT_C,//!< C
		D = OUT_D //!< D
	};

	/**
	 * Направление вращения мотора.
	 * Если мотор вращается в обратную сторону, getActualSpeed и getEncoder будут возвращать значения,
	 * соответствующие этому направлению.
	 */
	enum class Direction {
		FORWARD = OUT_FWD,//!< FORWARD
		BACKWARD = OUT_REV//!< BACKWARD
	};

	~Motor();

	/**
	 * Порт, к которому подключен мотор
	 * @return порт
	 */
	inline Port getPort() const {
		return port;
	}

	/**
	 * Направление вращения мотора
	 * @return направление
	 */
	inline Direction getDirection() const {
		return direction;
	}
	/**
	 * Устанавливает направление вращения мотора
	 * @param direction направление
	 */
	void setDirection(const Direction & direction);

	/**
	 * Текущая скорость вращения мотора.
	 * Положительная, если фактическое направление вращения мотора совпадает с выбранным.
	 * @return Скорость в интервале [0, 100]
	 */
	int getActualSpeed() const;

	/**
	 * Текущая скорость вращения мотора.
	 * Положительная, если фактическое направление вращения мотора совпадает с выбранным.
	 * @return провод с данными текущей скорости
	 */
	WireI getActualSpeedWire() const;

	/**
	 * Текущее значение енкодера.
	 * Положительное, если фактическое направление вращения мотора совпадает с выбранным.
	 * @return значение в градусах
	 */
	int getEncoder() const;
	/**
	 * Текущее значение енкодера.
	 * Положительное, если фактическое направление вращения мотора совпадает с выбранным.
	 * @return провод с данными текущего значения енкодера
	 */
	WireI getEncoderWire() const;

	/**
	 * Текущее значение енкодера, без учёта выбранного направления.
	 * @return значение в градусах
	 */
	int getTacho() const;
	/**
	 * Текущее значение енкодера, без учёта выбранного направления.
	 * @return провод с данными текущего значения енкодера без учёта направления
	 */
	WireI getTachoWire() const;

	/**
	 * Установка мощности мотора. Нельзя задавать одновременнно с setPower.
	 * Будет работать только последний вызов.
	 * @param power требуемая мощность
	 */
	void setPower(int power);
	/**
	 * Установка мощности мотора. Нельзя задавать одновременнно с setPower.
	 * Будет работать только последний вызов.
	 * @param output поток данных
	 */
	void setPower(const WireI & output);

	/**
	 * Актуальная мощность мотора, установленная на последней итерации
	 * @return мощность мотора в интервале [0, 100]
	 */
	int getPower() const;

	/**
	 * Сбрасывает значение энкодера
	 */
	void resetEncoder();
	/**
	 * Максимальное ускорение мотора в градусах на секунду в квадрате. По умолчанию 2500.
	 * Допустимые значения лежат в интервале примерно [1000, 10000]
	 */
	void setMaxAccelleration(float maxAcceleration);

	/**
	 * Масштабирование показаний енкодера. Используется для калибровки моторов.
	 * При вызове getEncoder исходное значение будет умножаться на scale.
	 * @param scale масштаб
	 */
	void setEncoderScale(float scale);

	/**
	 * Обновляет значение энкодера
	 * @param timestampSeconds текущее время в секундах
	 */
	void updateInputs(float timestampSeconds) override;
	/**
	 * Обновляет выходную мощность или скорость мотора с учётом максимального ускорения.
	 * @param timestampSeconds текущее время в секундах
	 */
	void updateOutputs(float timestampSeconds) override;

protected:
	Port port;
	Direction direction;
	float maxAcceleration;
	// max detected speed; used to calibrate power
	float maxSpeed;
	// actual speed measured at lastTimestamp
	float lastSpeed;
	float lastTimestamp;

	int8_t actualSpeed;
	int zeroEncoder;
	int encoder;
	int tacho;

	float encoderScale;

	WireI speedInput;
	WireI encoderInput;
	WireI tachoInput;
	std::shared_ptr<WireI> powerOutput;
	std::shared_ptr<WireI> speedOutput;

	Motor(Port port);

	friend class EV3;
};

typedef std::shared_ptr<Motor> MotorPtr;

} /* namespace ev3 */

#endif /* MOTOR_H_ */
