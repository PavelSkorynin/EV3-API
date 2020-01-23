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
 		if (front->isCompleted(secondsFromStart)) {
 			front->onCompleted(secondsFromStart);
 			sequence.pop();
 		} else {
 	 		front->update(secondsFromStart);
 			break;
 		}
 	}
}

bool ProcessSequence::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	return sequence.empty();
}


}
