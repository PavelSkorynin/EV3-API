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
 		if (front->isCompleted()) {
 			front->onCompleted(secondsFromStart);
 			sequence.pop_front();
 		} else {
 			break;
 		}
 	}
}

bool ProcessSequence::isCompleted() const {
	return sequence.empty();
}


}
