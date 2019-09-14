/*
 * Process.cpp
 *
 *  Created on: 07 September 2019
 *      Author: Pavel Skorynin
 */

#include "Process.h"

void ev3::Process::onComplete(float secondsFromStart) {

}

ev3::LambdaProcess::LambdaProcess(const std::function<bool(float)> &updateLambda)
	: updateLambda(updateLambda)
	, onCompleteLambda([](float){})
	, completed(false)
{

}

ev3::LambdaProcess::LambdaProcess(const std::function<bool(float)> &updateLambda, const std::function<void(float)> &onCompleteLambda)
	: updateLambda(updateLambda)
	, onCompleteLambda(onCompleteLambda)
	, completed(false)
{

}

void ev3::LambdaProcess::update(float secondsFromStart) {
	if (completed) {
		return;
	}

	completed = !updateLambda(secondsFromStart);
}

void ev3::LambdaProcess::onComplete(float secondsFromStart) {
	onCompleteLambda(secondsFromStart);
}

bool ev3::LambdaProcess::isComplete() const {
	return completed;
}

ev3::TimeProcess::TimeProcess(const std::function<void(float)> &updateLambda, float duration, float delay)
	: updateLambda(updateLambda)
	, onCompleteLambda([](float){})
	, isInitialized(false)
	, completed(false)
	, startTime(0)
	, duration(duration)
	, delay(delay)
{

}

ev3::TimeProcess::TimeProcess(const std::function<void(float)> &updateLambda, const std::function<void(float)> &onCompleteLambda, float duration, float delay)
	: updateLambda(updateLambda)
	, onCompleteLambda(onCompleteLambda)
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

	updateLambda(secondsFromStart);
}

void ev3::TimeProcess::onComplete(float secondsFromStart) {
	onCompleteLambda(secondsFromStart);
}

bool ev3::TimeProcess::isComplete() const {
	return completed;
}


