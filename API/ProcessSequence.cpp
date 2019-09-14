/*
 * ProcessSequence.cpp
 *
 *  Created on: 12 сент. 2019 г.
 *      Author: Pavel
 */

#include "ProcessSequence.h"

namespace ev3 {
ProcessSequence::ProcessSequence() {
}

ProcessSequence::~ProcessSequence() {
}

void ProcessSequence::update(float secondsFromStart) {
 	while (!sequence.empty()) {
 		auto& front = sequence.front();
 		front->update(secondsFromStart);
 		if (front->isComplete()) {
 			front->onComplete(secondsFromStart);
 			sequence.pop_front();
 		} else {
 			break;
 		}
 	}
}

bool ProcessSequence::isComplete() const {
	return sequence.empty();
}

void ProcessSequence::addProcess(const std::shared_ptr<Process> &process) {
	sequence.push_back(process);
}

}
