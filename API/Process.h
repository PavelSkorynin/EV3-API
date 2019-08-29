/*
 * Process.h
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef PROCESS_H_
#define PROCESS_H_

namespace ev3 {
/**
 * Класс для организации процесса обработки данных
 */
	class Process {
	public:
		virtual ~Process() {}

		/**
		 * Обновление
		 * @param secondsFromStart текущее время в секундах
		 */
		virtual void update(float secondsFromStart) = 0;
	};
}



#endif /* PROCESS_H_ */
