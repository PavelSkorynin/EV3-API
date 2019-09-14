/*
 * ProcessSequence.h
 *
 *  Created on: 12 сент. 2019 г.
 *      Author: Pavel
 */

#ifndef PROCESSSEQUENCE_H_
#define PROCESSSEQUENCE_H_

#include "Process.h"
#include <deque>
#include <memory>

namespace ev3 {
class ProcessSequence: public Process {
public:
	ProcessSequence();
	virtual ~ProcessSequence();

	virtual void update(float secondsFromStart) override;
	virtual bool isComplete() const override;

	void addProcess(const std::shared_ptr<Process> &process);

protected:
	std::deque<std::shared_ptr<Process>> sequence;
};
}
#endif /* PROCESSSEQUENCE_H_ */
