/*
 * ProcessGroup.h
 *
 *  Created on: 12 сент. 2019 г.
 *      Author: Pavel
 */

#ifndef PROCESSGROUP_H_
#define PROCESSGROUP_H_

#include "Process.h"
#include <set>
#include <vector>
#include <memory>
#include <type_traits>

namespace ev3 {
class ProcessGroup: public Process {
public:
	ProcessGroup();
	virtual ~ProcessGroup();

	virtual void update(float secondsFromStart) override;
	virtual bool isCompleted() const override;

	template<class ProcessClass>
	void addProcess(const std::shared_ptr<ProcessClass> &process) {
		static_assert(std::is_base_of<Process, ProcessClass>::value);
		processesToAdd.push_back(std::dynamic_pointer_cast<Process>(process));
	}

protected:
	std::set<std::shared_ptr<Process>> group;
	std::vector<std::shared_ptr<Process>> processesToAdd;
};
}

#endif /* PROCESSGROUP_H_ */
