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

/**
 * Пропорционально-дифференциальный регулятор. Используется в задаче движения по линии и
 * во многих других прикладных задачах.
 */
class PD: public virtual Process {
public:
	PD(float kp = 0.4f, float kd = 1.2f);
	~PD();

	/**
	 * Установка ошибки, передаваемой в ПД-регулятор
	 * @param errorWire поток данных
	 */
	void setError(const WireF & errorWire);
	/**
	 * Возвращает мощность выходного компенсирующего сигнала с учётом ошибки и параметров.
	 * @return поток данных
	 */
	WireF getPower();

	/**
	 * Обновление выходной мощности
	 * @param secondsFromStart текущее время в секундах
	 */
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
