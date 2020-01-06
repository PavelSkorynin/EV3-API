/*
 * ProcessSequence.cpp
 *
 *  Created on: 12 сент. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "ProcessSequence.hpp"

namespace ev3 {
ProcessSequence::ProcessSequence() {
}

void ProcessSequence::update(float secondsFromStart) {
	Process::update(secondsFromStart);
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
