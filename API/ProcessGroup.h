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
	ProcessGroup(bool completeIfAnyIsCompleted = false);
	ProcessGroup(ProcessGroup &&) = default;
	ProcessGroup(const ProcessGroup &) = default;
	virtual ~ProcessGroup() = default;

	ProcessGroup& operator=(const ProcessGroup&) = default;
	ProcessGroup& operator=(ProcessGroup&&) = default;

	virtual void update(float secondsFromStart) override;
	virtual bool isCompleted() const override;

	template<class ProcessClass>
	void addProcess(ProcessClass &&process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		processesToAdd.push_back(std::make_shared<ProcessClass>(process));
	}
	template<class ProcessClass>
	void addProcess(ProcessClass &process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		processesToAdd.push_back(std::make_shared<ProcessClass>(process));
	}
	template<class ProcessClass>
	void addProcess(std::shared_ptr<ProcessClass> process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		processesToAdd.push_back(process);
	}
protected:
	std::set<std::shared_ptr<Process>> group;
	std::vector<std::shared_ptr<Process>> processesToAdd;
	bool completeIfAnyIsCompleted;

public:
	template<class ProcessClassB>
	friend ProcessGroup operator|(ProcessGroup &&processA, ProcessClassB &&processB);
	template<class ProcessClassA>
	friend ProcessGroup operator|(ProcessClassA &&processA, ProcessGroup &&processB);
	template<class ProcessClassB>
	friend ProcessGroup operator&(ProcessGroup &&processA, ProcessClassB &&processB);
	template<class ProcessClassA>
	friend ProcessGroup operator&(ProcessClassA &&processA, ProcessGroup &&processB);
};

	template<class ProcessClassA, class ProcessClassB>
	ProcessGroup operator|(ProcessClassA &&processA, ProcessClassB &&processB) {
		static_assert(std::is_base_of<Process, ProcessClassA>::value);
		static_assert(std::is_base_of<Process, ProcessClassB>::value);
		ProcessGroup group(true);
		group.addProcess(processA);
		group.addProcess(processB);
		return group;
	}

	template<class ProcessClassB>
	ProcessGroup operator|(ProcessGroup &&processA, ProcessClassB &&processB) {
		static_assert(std::is_base_of<Process, ProcessClassB>::value);
		if (processA.completeIfAnyIsCompleted) {
			processA.addProcess(processB);
			return processA;
		}
		ProcessGroup group(true);
		group.addProcess(processA);
		group.addProcess(processB);
		return group;
	}

	template<class ProcessClassA>
	ProcessGroup operator|(ProcessClassA &&processA, ProcessGroup &&processB) {
		static_assert(std::is_base_of<Process, ProcessClassA>::value);
		if (processB.completeIfAnyIsCompleted) {
			processB.addProcess(processA);
			return processB;
		}
		ProcessGroup group(true);
		group.addProcess(processA);
		group.addProcess(processB);
		return group;
	}


	template<class ProcessClassA, class ProcessClassB>
	ProcessGroup operator&(ProcessClassA &&processA, ProcessClassB &&processB) {
		static_assert(std::is_base_of<Process, ProcessClassA>::value);
		static_assert(std::is_base_of<Process, ProcessClassB>::value);
		ProcessGroup group(false);
		group.addProcess(processA);
		group.addProcess(processB);
		return group;
	}

	template<class ProcessClassB>
	ProcessGroup operator&(ProcessGroup &&processA, ProcessClassB &&processB) {
		static_assert(std::is_base_of<Process, ProcessClassB>::value);
		if (!processA.completeIfAnyIsCompleted) {
			processA.addProcess(processB);
			return processA;
		}
		ProcessGroup group(false);
		group.addProcess(processA);
		group.addProcess(processB);
		return group;
	}

	template<class ProcessClassA>
	ProcessGroup operator&(ProcessClassA &&processA, ProcessGroup &&processB) {
		static_assert(std::is_base_of<Process, ProcessClassA>::value);
		if (!processB.completeIfAnyIsCompleted) {
			processB.addProcess(processA);
			return processB;
		}
		ProcessGroup group(false);
		group.addProcess(processA);
		group.addProcess(processB);
		return group;
	}
}

