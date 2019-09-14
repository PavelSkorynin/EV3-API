/*
 * ProcessGroup.cpp
 *
 *  Created on: 12 сент. 2019 г.
 *      Author: Pavel
 */

#include "ProcessGroup.h"

namespace ev3 {
ProcessGroup::ProcessGroup() {
}

ProcessGroup::~ProcessGroup() {
}

void ProcessGroup::update(float secondsFromStart) {
	group.insert(processesToAdd.begin(), processesToAdd.end());
	processesToAdd.clear();

	for (auto it = group.begin(); it != group.end(); ) {
		auto &process = *it;
		process->update(secondsFromStart);
 		if (process->isComplete()) {
 			process->onComplete(secondsFromStart);
 			it = group.erase(it);
 		} else {
 			++it;
 		}
	}
}

bool ProcessGroup::isComplete() const {
	return group.empty() && processesToAdd.empty();
}

void ProcessGroup::addProcess(const std::shared_ptr<Process> &process) {
	processesToAdd.push_back(process);
}
}
