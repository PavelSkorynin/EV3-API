/*
 * PD.h
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef PID_H_
#define PID_H_

#include <functional>
#include "Process.h"
#include "Wire.h"

namespace ev3 {

/**
 * Пропорционально-интегрально-дифференциальный регулятор. Используется в задаче движения по линии и
 * во многих других прикладных задачах.
 */
class PID: public virtual Process {
public:
	/**
	 * Конструктор ПИД-регулятора
	 * @param kp коэффициент пропорциональной составляющей
	 * @param ki коэффициент интегральной составляющей
	 * @param kd коэффициент дифференциальной составляющей
	 * @param isCompleted функция, которая определяет, когда необходимо завершить процесс ПИД-регулирования
	 */
	explicit PID(float kp = 0.4f, float ki = 0.00001f, float kd = 1.2f);

	/**
	 * Установка ошибки, передаваемой в ПД-регулятор
	 * @param errorWire поток данных
	 */
	void setError(const WireF & errorWire);
	/**
	 * Возвращает мощность выходного компенсирующего сигнала с учётом ошибки и параметров.
	 * @return текущее воздействие
	 */
	float getPower() const;

	/**
	 * Обновление выходной мощности
	 * @param secondsFromStart текущее время в секундах
	 */
	void update(float secondsFromStart) override;

	/**
	 * Признак завершённости процесса
	 * @return возвращает результат функции isCompleted, переданной в конструктор
	 */
	bool isCompleted() const;

	/**
	 * Установить коэффициенты ПИД-регулятора
	 * @param kP пропорциональная составляющая
	 * @param kI интегральная составляющая
	 * @param kD дифференциальная составляющая
	 */
	void setPID(float kP, float kI, float kD);

protected:
	float kp;
	float ki;
	float kd;
	float lastError;
	float lastIntegralPart;
	float lastUpdateTime;
	float power;

	WireF errorWire;
};

} /* namespace ev3 */

#endif /* PID_H_ */
