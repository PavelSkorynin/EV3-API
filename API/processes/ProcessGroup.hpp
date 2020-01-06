/*
 * ProcessGroup.h
 *
 *  Created on: 12 сент. 2019 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include "Process.h"
#include <set>
#include <vector>
#include <memory>
#include <type_traits>

namespace ev3 {
class ProcessGroup: public Process {
public:
	explicit ProcessGroup(bool completeIfAnyIsCompleted = false);
	ProcessGroup(ProcessGroup&&) = default;
	ProcessGroup(const ProcessGroup&) = default;
	virtual ~ProcessGroup() = default;

	ProcessGroup& operator=(const ProcessGroup&) = default;
	ProcessGroup& operator=(ProcessGroup&&) = default;

	virtual void update(float secondsFromStart) override;
	virtual bool isCompleted() const override;

	template<class ProcessClass>
	void addProcess(ProcessClass process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		processesToAdd.emplace_back(std::make_shared<ProcessClass>(std::move(process)));
	}
	template<class ProcessClass>
	void addProcess(std::shared_ptr<ProcessClass> process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		processesToAdd.emplace_back(process);
	}
protected:
	std::vector<std::shared_ptr<Process>> group;
	std::vector<std::shared_ptr<Process>> processesToAdd;
	bool completeIfAnyIsCompleted;
};

class ProcessGroupOr: public ProcessGroup {
public:
	ProcessGroupOr();
	ProcessGroupOr(ProcessGroupOr&&) = default;
	ProcessGroupOr(const ProcessGroupOr&) = default;
	virtual ~ProcessGroupOr() = default;

	ProcessGroupOr& operator=(const ProcessGroupOr&) = default;
	ProcessGroupOr& operator=(ProcessGroupOr&&) = default;
};

class ProcessGroupAnd: public ProcessGroup {
public:
	ProcessGroupAnd();
	ProcessGroupAnd(ProcessGroupAnd&&) = default;
	ProcessGroupAnd(const ProcessGroupAnd&) = default;
	virtual ~ProcessGroupAnd() = default;

	ProcessGroupAnd& operator=(const ProcessGroupAnd&) = default;
	ProcessGroupAnd& operator=(ProcessGroupAnd&&) = default;
};

template<class ProcessClassA, class ProcessClassB>
ProcessGroupOr operator|(ProcessClassA&& processA, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessGroupOr group;
	group.addProcess(processA);
	group.addProcess(processB);
	return group;
}

template<class ProcessClassA, class ProcessClassB>
ProcessGroupOr operator|(ProcessClassA& processA, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessGroupOr group;
	group.addProcess(processA);
	group.addProcess(processB);
	return group;
}

template<class ProcessClassA, class ProcessClassB>
ProcessGroupOr operator|(ProcessClassA&& processA, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessGroupOr group;
	group.addProcess(processA);
	group.addProcess(processB);
	return group;
}

template<class ProcessClassA, class ProcessClassB>
ProcessGroupOr operator|(ProcessClassA& processA, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessGroupOr group;
	group.addProcess(processA);
	group.addProcess(processB);
	return group;
}


template<class ProcessClassB>
ProcessGroupOr&& operator|(ProcessGroupOr&& processA, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	processA.addProcess(processB);
	return std::move(processA);
}

template<class ProcessClassB>
ProcessGroupOr& operator|(ProcessGroupOr& processA, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	processA.addProcess(processB);
	return processA;
}

template<class ProcessClassB>
ProcessGroupOr&& operator|(ProcessGroupOr&& processA, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	processA.addProcess(processB);
	return std::move(processA);
}

template<class ProcessClassB>
ProcessGroupOr& operator|(ProcessGroupOr& processA, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	processA.addProcess(processB);
	return processA;
}



template<class ProcessClassA>
ProcessGroupOr&& operator|(ProcessClassA&& processA, ProcessGroupOr&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	processB.addProcess(processA);
	return std::move(processB);
}

template<class ProcessClassA>
ProcessGroupOr&& operator|(ProcessClassA& processA, ProcessGroupOr&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	processB.addProcess(processA);
	return std::move(processB);
}

template<class ProcessClassA>
ProcessGroupOr& operator|(ProcessClassA&& processA, ProcessGroupOr& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	processB.addProcess(processA);
	return processB;
}

template<class ProcessClassA>
ProcessGroupOr& operator|(ProcessClassA& processA, ProcessGroupOr& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	processB.addProcess(processA);
	return processB;
}



template<class ProcessClassA, class ProcessClassB>
ProcessGroupAnd operator&(ProcessClassA&& processA, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessGroupAnd group;
	group.addProcess(processA);
	group.addProcess(processB);
	return group;
}
template<class ProcessClassA, class ProcessClassB>
ProcessGroupAnd operator&(ProcessClassA&& processA, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessGroupAnd group;
	group.addProcess(processA);
	group.addProcess(processB);
	return group;
}
template<class ProcessClassA, class ProcessClassB>
ProcessGroupAnd operator&(ProcessClassA& processA, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessGroupAnd group;
	group.addProcess(processA);
	group.addProcess(processB);
	return group;
}
template<class ProcessClassA, class ProcessClassB>
ProcessGroupAnd operator&(ProcessClassA& processA, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	ProcessGroupAnd group;
	group.addProcess(processA);
	group.addProcess(processB);
	return group;
}



template<class ProcessClassB>
ProcessGroupAnd&& operator&(ProcessGroupAnd&& processA, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	processA.addProcess(processB);
	return std::move(processA);
}
template<class ProcessClassB>
ProcessGroupAnd& operator&(ProcessGroupAnd& processA, ProcessClassB&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	processA.addProcess(processB);
	return processA;
}
template<class ProcessClassB>
ProcessGroupAnd&& operator&(ProcessGroupAnd&& processA, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	processA.addProcess(processB);
	return std::move(processA);
}
template<class ProcessClassB>
ProcessGroupAnd& operator&(ProcessGroupAnd& processA, ProcessClassB& processB) {
	static_assert(std::is_base_of<Process, ProcessClassB>::value);
	processA.addProcess(processB);
	return processA;
}


template<class ProcessClassA>
ProcessGroup&& operator&(ProcessClassA&& processA, ProcessGroupAnd&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	processB.addProcess(processA);
	return std::move(processB);
}
template<class ProcessClassA>
ProcessGroup&& operator&(ProcessClassA& processA, ProcessGroupAnd&& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	processB.addProcess(processA);
	return std::move(processB);
}
template<class ProcessClassA>
ProcessGroup& operator&(ProcessClassA&& processA, ProcessGroupAnd& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	processB.addProcess(processA);
	return processB;
}
template<class ProcessClassA>
ProcessGroup& operator&(ProcessClassA& processA, ProcessGroupAnd& processB) {
	static_assert(std::is_base_of<Process, ProcessClassA>::value);
	processB.addProcess(processA);
	return processB;
}
}

