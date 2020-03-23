/*
 * BenchmarkProcess.hpp
 *
 *  Created on: 24 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>

namespace ev3 {

class BenchmarkProcess: public virtual Process {
public:
	BenchmarkProcess();
	virtual ~BenchmarkProcess() = default;

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;

	float getStartTime() const { return startTime; }
	float getCompleteTime() const { return completeTime; }
	float getDuration() const { return completeTime - startTime; }
	int getNumberOfIterations() const { return numberOfIterations; }
	float getNumberOfIterationsPerSecond() const {
		float duration = getDuration();
		if (duration > 0) {
			return numberOfIterations / duration;
		}
		return 0;
	}

protected:
	float startTime = 0;
	float completeTime = 0;
	int numberOfIterations = 0;
};

} /* namespace ev3 */

