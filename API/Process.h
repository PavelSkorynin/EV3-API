/*
 * Process.h
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#pragma once

#include <functional>

namespace ev3 {
/**
 * Класс для организации процесса обработки данных
 */
	class Process {
	protected:
		bool isStarted;
	public:
		Process() : isStarted(false) {}
		virtual ~Process() = default;

		/**
		 * Обновление
		 * @param secondsFromStart текущее время в секундах
		 */
		virtual void update(float secondsFromStart);

		/**
		 * Метод вызывается, когда первый раз вызывается update
		 * @param secondsFromStart
		 */
		virtual void onStarted(float secondsFromStart);

		/**
		 * Метод вызывается, когда процесс завершён
		 * @param secondsFromStart
		 */
		virtual void onCompleted(float secondsFromStart);

		/**
		 * Признак завершённости процесса
		 * @return true, если процесс завершён и больше вызывать update не имеет смысла
		 */
		virtual bool isCompleted() const = 0;
	};

	class LambdaProcess : public virtual Process {
	public:
		LambdaProcess(const std::function<bool(float)> &updateFunc);
		LambdaProcess(const std::function<bool(float)> &updateFunc, const std::function<void(float)> &onCompletedFunc);

		virtual void update(float secondsFromStart) override;
		virtual void onCompleted(float secondsFromStart) override;
		virtual bool isCompleted() const override;

	protected:
		std::function<bool(float)> updateFunc;
		std::function<void(float)> onCompletedFunc;
		bool completed;
	};

	class TimeProcess : public virtual Process {
	public:
		TimeProcess(const std::function<void(float)> &updateFunc, float duration, float delay = 0.0f);
		TimeProcess(const std::function<void(float)> &updateFunc, const std::function<void(float)> &onCompletedFunc, float duration, float delay = 0.0f);

		virtual void onStarted(float secondsFromStart) override;
		virtual void update(float secondsFromStart) override;
		virtual void onCompleted(float secondsFromStart) override;
		virtual bool isCompleted() const override;

	protected:
		std::function<void(float)> updateFunc;
		std::function<void(float)> onCompletedFunc;
		bool completed;
		float startTime;
		float duration;
		float delay;
	};
}
