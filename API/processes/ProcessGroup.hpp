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

class ProcessGroupOr;
class ProcessGroupAnd;

class ProcessGroup: public virtual Process {
public:
	explicit ProcessGroup(bool completeIfAnyIsCompleted = false);

	virtual void update(float secondsFromStart) override;
	virtual bool isCompleted(float secondsFromStart) override;

protected:
	template<class ProcessClass>
	void addProcess(ProcessClass&& process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		temporaryProcesses.emplace_back(std::make_shared<ProcessClass>(process));
		group.push_back(temporaryProcesses.back().get());
	}
	template<class ProcessClass>
	void addProcess(ProcessClass& process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		group.push_back(&process);
	}
	template<class ProcessClass>
	void addProcess(std::shared_ptr<ProcessClass> process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		group.push_back(process.get());
	}

	template<class ProcessClassA, class ProcessClassB>
	friend ProcessGroupOr operator|(ProcessClassA&& processA, ProcessClassB&& processB);

	template<class ProcessClassA, class ProcessClassB>
	friend ProcessGroupOr operator|(ProcessClassA&& processA, ProcessClassB& processB);

	template<class ProcessClassA, class ProcessClassB>
	friend ProcessGroupOr operator|(ProcessClassA& processA, ProcessClassB&& processB);

	template<class ProcessClassA, class ProcessClassB>
	friend ProcessGroupOr operator|(ProcessClassA& processA, ProcessClassB& processB);

	template<class ProcessClassB>
	friend ProcessGroupOr&& operator|(ProcessGroupOr&& processA, ProcessClassB&& processB);

	template<class ProcessClassB>
	friend ProcessGroupOr& operator|(ProcessGroupOr& processA, ProcessClassB&& processB);

	template<class ProcessClassB>
	friend ProcessGroupOr&& operator|(ProcessGroupOr&& processA, ProcessClassB& processB);

	template<class ProcessClassB>
	friend ProcessGroupOr& operator|(ProcessGroupOr& processA, ProcessClassB& processB);

	template<class ProcessClassA>
	friend ProcessGroupOr&& operator|(ProcessClassA&& processA, ProcessGroupOr&& processB);

	template<class ProcessClassA>
	friend ProcessGroupOr&& operator|(ProcessClassA& processA, ProcessGroupOr&& processB);

	template<class ProcessClassA>
	friend ProcessGroupOr& operator|(ProcessClassA&& processA, ProcessGroupOr& processB);

	template<class ProcessClassA>
	friend ProcessGroupOr& operator|(ProcessClassA& processA, ProcessGroupOr& processB);

	template<class ProcessClassA, class ProcessClassB>
	friend ProcessGroupAnd operator&(ProcessClassA&& processA, ProcessClassB&& processB);

	template<class ProcessClassA, class ProcessClassB>
	friend ProcessGroupAnd operator&(ProcessClassA&& processA, ProcessClassB& processB);

	template<class ProcessClassA, class ProcessClassB>
	friend ProcessGroupAnd operator&(ProcessClassA& processA, ProcessClassB&& processB);

	template<class ProcessClassA, class ProcessClassB>
	friend ProcessGroupAnd operator&(ProcessClassA& processA, ProcessClassB& processB);

	template<class ProcessClassB>
	friend ProcessGroupAnd&& operator&(ProcessGroupAnd&& processA, ProcessClassB&& processB);

	template<class ProcessClassB>
	friend ProcessGroupAnd& operator&(ProcessGroupAnd& processA, ProcessClassB&& processB);

	template<class ProcessClassB>
	friend ProcessGroupAnd&& operator&(ProcessGroupAnd&& processA, ProcessClassB& processB);

	template<class ProcessClassB>
	friend ProcessGroupAnd& operator&(ProcessGroupAnd& processA, ProcessClassB& processB);

	template<class ProcessClassA>
	friend ProcessGroup&& operator&(ProcessClassA&& processA, ProcessGroupAnd&& processB);

	template<class ProcessClassA>
	friend ProcessGroup&& operator&(ProcessClassA& processA, ProcessGroupAnd&& processB);

	template<class ProcessClassA>
	friend ProcessGroup& operator&(ProcessClassA&& processA, ProcessGroupAnd& processB);

	template<class ProcessClassA>
	friend ProcessGroup& operator&(ProcessClassA& processA, ProcessGroupAnd& processB);

protected:
	std::vector<Process*> group;
	std::vector<std::shared_ptr<Process>> temporaryProcesses;
	bool completeIfAnyIsCompleted;
};

class ProcessGroupOr: public virtual ProcessGroup {
public:
	ProcessGroupOr();
	ProcessGroupOr(ProcessGroupOr&&) = default;
	ProcessGroupOr(const ProcessGroupOr&) = default;
	virtual ~ProcessGroupOr() = default;

	ProcessGroupOr& operator=(const ProcessGroupOr&) = default;
	ProcessGroupOr& operator=(ProcessGroupOr&&);
};

class ProcessGroupAnd: public virtual ProcessGroup {
public:
	ProcessGroupAnd();
	ProcessGroupAnd(ProcessGroupAnd&&) = default;
	ProcessGroupAnd(const ProcessGroupAnd&) = default;
	virtual ~ProcessGroupAnd() = default;

	ProcessGroupAnd& operator=(const ProcessGroupAnd&) = default;
	ProcessGroupAnd& operator=(ProcessGroupAnd&&);
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

