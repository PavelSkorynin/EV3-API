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
	virtual bool isCompleted() const override;

	template<class ProcessClass>
	void addProcess(const std::shared_ptr<ProcessClass> &process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		sequence.push_back(std::dynamic_pointer_cast<Process>(process));
	}

protected:
	std::deque<std::shared_ptr<Process>> sequence;
};
}
#endif /* PROCESSSEQUENCE_H_ */
