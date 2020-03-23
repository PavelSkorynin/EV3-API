/*
 * BenchmarkProcess.cpp
 *
 *  Created on: 24 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include "BenchmarkProcess.hpp"

namespace ev3 {

BenchmarkProcess::BenchmarkProcess() {
}

void BenchmarkProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	numberOfIterations++;
	completeTime = secondsFromStart;
}

void BenchmarkProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	completeTime = startTime = secondsFromStart;
}

} /* namespace ev3 */
