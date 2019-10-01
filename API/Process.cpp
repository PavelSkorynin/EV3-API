/*
 * Process.cpp
 *
 *  Created on: 07 September 2019
 *      Author: Pavel Skorynin
 */

#include "Process.h"

void ev3::Process::onCompleted(float secondsFromStart) {

}

ev3::LambdaProcess::LambdaProcess(const std::function<bool(float)> &updateFunc)
	: updateFunc(updateFunc)
	, onCompletedFunc([](float){})
	, completed(false)
{

}

ev3::LambdaProcess::LambdaProcess(const std::function<bool(float)> &updateFunc, const std::function<void(float)> &onCompletedFunc)
	: updateFunc(updateFunc)
	, onCompletedFunc(onCompletedFunc)
	, completed(false)
{

}

void ev3::LambdaProcess::update(float secondsFromStart) {
	if (completed) {
		return;
	}

	completed = !updateFunc(secondsFromStart);
}

void ev3::LambdaProcess::onCompleted(float secondsFromStart) {
	onCompletedFunc(secondsFromStart);
}

bool ev3::LambdaProcess::isCompleted() const {
	return completed;
}

ev3::TimeProcess::TimeProcess(const std::function<void(float)> &updateFunc, float duration, float delay)
	: updateFunc(updateFunc)
	, onCompletedFunc([](float){})
	, isInitialized(false)
	, completed(false)
	, startTime(0)
	, duration(duration)
	, delay(delay)
{

}

ev3::TimeProcess::TimeProcess(const std::function<void(float)> &updateFunc, const std::function<void(float)> &onCompletedFunc, float duration, float delay)
	: updateFunc(updateFunc)
	, onCompletedFunc(onCompletedFunc)
	, isInitialized(false)
	, completed(false)
	, startTime(0)
	, duration(duration)
	, delay(delay)
{

}

void ev3::TimeProcess::update(float secondsFromStart) {
	if (completed) {
		return;
	}

	if (!isInitialized) {
		startTime = secondsFromStart;
		isInitialized = true;
	}

	if (secondsFromStart >= startTime + delay + duration) {
		completed = true;
		return;
	}

	updateFunc(secondsFromStart);
}

void ev3::TimeProcess::onCompleted(float secondsFromStart) {
	onCompletedFunc(secondsFromStart);
}

bool ev3::TimeProcess::isCompleted() const {
	return completed;
}


