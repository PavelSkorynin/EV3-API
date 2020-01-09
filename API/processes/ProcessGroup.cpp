/*
 * ProcessGroup.cpp
 *
 *  Created on: 12 сент. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "ProcessGroup.hpp"

namespace ev3 {
ProcessGroup::ProcessGroup(bool completeIfAnyIsCompleted)
	: completeIfAnyIsCompleted(completeIfAnyIsCompleted) {
}

void ProcessGroup::update(float secondsFromStart) {
	Process::update(secondsFromStart);

	for (auto it = group.begin(); it != group.end(); ) {
		auto process = *it;
		process->update(secondsFromStart);
 		if (process->isCompleted()) {
 			if (completeIfAnyIsCompleted) {
 				for (const auto &process : group) {
 					process->onCompleted(secondsFromStart);
 				}
 				group.clear();
 				return;
 			}
 			process->onCompleted(secondsFromStart);
 			it = group.erase(it);
 		} else {
 			++it;
 		}
	}
}

bool ProcessGroup::isCompleted() const {
	return group.empty();
}

ProcessGroupOr::ProcessGroupOr() : ProcessGroup(false) {}

ProcessGroupAnd::ProcessGroupAnd() : ProcessGroup(true) {}

}
