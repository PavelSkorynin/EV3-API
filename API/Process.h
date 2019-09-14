/*
 * Process.h
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <functional>

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

		/**
		 * Метод вызывается, когда процесс завершён
		 * @param secondsFromStart
		 */
		virtual void onComplete(float secondsFromStart);

		/**
		 * Признак завершённости процесса
		 * @return true, если процесс завершён и больше вызывать update не имеет смысла
		 */
		virtual bool isComplete() const = 0;
	};

	class LambdaProcess : public Process {
	public:
		LambdaProcess(const std::function<bool(float)> &updateLambda);
		LambdaProcess(const std::function<bool(float)> &updateLambda, const std::function<void(float)> &onCompleteLambda);

		virtual void update(float secondsFromStart) override;
		virtual void onComplete(float secondsFromStart) override;
		virtual bool isComplete() const override;

	protected:
		std::function<bool(float)> updateLambda;
		std::function<void(float)> onCompleteLambda;
		bool completed;
	};

	class TimeProcess : public Process {
	public:
		TimeProcess(const std::function<void(float)> &updateLambda, float duration, float delay = 0.0f);
		TimeProcess(const std::function<void(float)> &updateLambda, const std::function<void(float)> &onCompleteLambda, float duration, float delay = 0.0f);

		virtual void update(float secondsFromStart) override;
		virtual void onComplete(float secondsFromStart) override;
		virtual bool isComplete() const override;

	protected:
		std::function<void(float)> updateLambda;
		std::function<void(float)> onCompleteLambda;
		bool isInitialized;
		bool completed;
		float startTime;
		float duration;
		float delay;
	};
}

#endif /* PROCESS_H_ */
