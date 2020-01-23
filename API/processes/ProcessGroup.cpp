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
 		if (process->isCompleted(secondsFromStart)) {
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
 			process->update(secondsFromStart);
 			++it;
 		}
	}
}

bool ProcessGroup::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	return group.empty();
}

ProcessGroupOr::ProcessGroupOr() : ProcessGroup(false) {}

ProcessGroupOr& ProcessGroupOr::operator=(ProcessGroupOr&& other) {
	isStarted = other.isStarted;
	group = std::move(other.group);
	completeIfAnyIsCompleted = other.completeIfAnyIsCompleted;
	return *this;
}

ProcessGroupAnd::ProcessGroupAnd() : ProcessGroup(true) {}

ProcessGroupAnd& ProcessGroupAnd::operator=(ProcessGroupAnd&& other) {
	isStarted = other.isStarted;
	group = std::move(other.group);
	completeIfAnyIsCompleted = other.completeIfAnyIsCompleted;
	return *this;
}

}

