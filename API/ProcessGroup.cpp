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
 		if (process->isCompleted()) {
 			process->onCompleted(secondsFromStart);
 			it = group.erase(it);
 		} else {
 			++it;
 		}
	}
}

bool ProcessGroup::isCompleted() const {
	return group.empty() && processesToAdd.empty();
}

}
