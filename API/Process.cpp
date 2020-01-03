/*
 * Process.cpp
 *
 *  Created on: 07 September 2019
 *      Author: Pavel Skorynin
 */

#include "Process.h"

namespace ev3 {

void Process::update(float secondsFromStart) {
	if (!isStarted) {
		onStarted(secondsFromStart);
		isStarted = true;
	}
}

void Process::onStarted(float secondsFromStart) {

}

void Process::onCompleted(float secondsFromStart) {

}

LambdaProcess::LambdaProcess(const std::function<bool(float)> &updateFunc)
	: updateFunc(updateFunc)
	, onCompletedFunc([](float){})
	, completed(false)
{

}

LambdaProcess::LambdaProcess(const std::function<bool(float)> &updateFunc, const std::function<void(float)> &onCompletedFunc)
	: updateFunc(updateFunc)
	, onCompletedFunc(onCompletedFunc)
	, completed(false)
{

}

void LambdaProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	if (completed) {
		return;
	}

	completed = !updateFunc(secondsFromStart);
}

void LambdaProcess::onCompleted(float secondsFromStart) {
	Process::onCompleted(secondsFromStart);
	onCompletedFunc(secondsFromStart);
}

bool LambdaProcess::isCompleted() const {
	return completed;
}

TimeProcess::TimeProcess(const std::function<void(float)> &updateFunc, float duration, float delay)
	: updateFunc(updateFunc)
	, onCompletedFunc([](float){})
	, completed(false)
	, startTime(0)
	, duration(duration)
	, delay(delay)
{

}

TimeProcess::TimeProcess(const std::function<void(float)> &updateFunc, const std::function<void(float)> &onCompletedFunc, float duration, float delay)
	: updateFunc(updateFunc)
	, onCompletedFunc(onCompletedFunc)
	, completed(false)
	, startTime(0)
	, duration(duration)
	, delay(delay)
{

}

void TimeProcess::onStarted(float secondsFromStart) {
	startTime = secondsFromStart;
}

void TimeProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	if (completed) {
		return;
	}

	if (secondsFromStart >= startTime + delay + duration) {
		completed = true;
		return;
	}

	updateFunc(secondsFromStart);
}

void TimeProcess::onCompleted(float secondsFromStart) {
	Process::onCompleted(secondsFromStart);
	onCompletedFunc(secondsFromStart);
}

bool TimeProcess::isCompleted() const {
	return completed;
}

} // namespace ev3
