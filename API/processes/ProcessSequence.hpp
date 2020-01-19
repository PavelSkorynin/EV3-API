/*
 * ProcessSequence.h
 *
 *  Created on: 12 сент. 2019 г.
 *      Author: Pavel Skorynin
 */

#ifndef PROCESSSEQUENCE_H_
#define PROCESSSEQUENCE_H_

#include "Process.h"
#include <queue>
#include <memory>

namespace ev3 {
class ProcessSequence: public virtual Process {
public:
	ProcessSequence();

	virtual void update(float secondsFromStart) override;
	virtual bool isCompleted() const override;

	template<class ProcessClassA>
	ProcessSequence& operator>>=(ProcessClassA &&processA) {
		static_assert(std::is_base_of<Process, ProcessClassA>::value);
		addProcess(processA);
		return *this;
	}

	template<class ProcessClassA>
	ProcessSequence& operator>>=(ProcessClassA &processA) {
		static_assert(std::is_base_of<Process, ProcessClassA>::value);
		addProcess(processA);
		return *this;
	}

	template<class ProcessClass>
	void addProcess(ProcessClass &&process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		sequence.emplace(std::make_shared<ProcessClass>(process));
	}
	template<class ProcessClass>
	void addProcess(ProcessClass &process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		sequence.emplace(std::make_shared<ProcessClass>(process));
	}
	template<class ProcessClass>
	void addProcess(std::shared_ptr<ProcessClass> process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		sequence.emplace(process);
	}

protected:
	std::queue<std::shared_ptr<Process>> sequence;
};

template<class ProcessClassA, class ProcessClassB>
ProcessSequence operator>>(ProcessClassA &&processA, ProcessClassB &&processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessSequence sequence;
	sequence.addProcess(processA);
	sequence.addProcess(processB);
	return sequence;
}

template<class ProcessClassA, class ProcessClassB>
ProcessSequence operator>>(ProcessClassA &processA, ProcessClassB &&processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessSequence sequence;
	sequence.addProcess(processA);
	sequence.addProcess(processB);
	return sequence;
}

template<class ProcessClassA, class ProcessClassB>
ProcessSequence operator>>(ProcessClassA &&processA, ProcessClassB &processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessSequence sequence;
	sequence.addProcess(processA);
	sequence.addProcess(processB);
	return sequence;
}

template<class ProcessClassA, class ProcessClassB>
ProcessSequence operator>>(ProcessClassA &processA, ProcessClassB &processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessSequence sequence;
	sequence.addProcess(processA);
	sequence.addProcess(processB);
	return sequence;
}

template<class ProcessClassB>
ProcessSequence& operator>>(ProcessSequence& sequence, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	sequence.addProcess(processB);
	return sequence;
}

template<class ProcessClassB>
ProcessSequence&& operator>>(ProcessSequence&& sequence, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	sequence.addProcess(processB);
	return std::move(sequence);
}

template<class ProcessClassB>
ProcessSequence& operator>>(ProcessSequence& sequence, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	sequence.addProcess(processB);
	return sequence;
}

template<class ProcessClassB>
ProcessSequence&& operator>>(ProcessSequence&& sequence, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	sequence.addProcess(processB);
	return std::move(sequence);
}

}
#endif /* PROCESSSEQUENCE_H_ */
