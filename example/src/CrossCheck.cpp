/*
 * CrossCheck.cpp
 *
 *  Created on: May 15, 2019
 *      Author: pavel
 */

#include "CrossCheck.h"

#include <cmath>

namespace ev3 {

CrossCheck::CrossCheck(const WireF & left, const WireF & right, const WireF & encoder)
	: left(left)
	, right(right)
	, encoder(encoder)
	, leftCrossCount(0)
	, rightCrossCount(0)
	, xCrossCount(0)
	, lastWhiteLeftEncoder(encoder)
	, lastWhiteRightEncoder(encoder)
	, lastCrossEncoder(encoder)
	, state(State::NO_CROSS)
	, greyThreshold(30)
	, lineWidth(30)
{

}

CrossCheck::~CrossCheck() {
}

void CrossCheck::update(float seconds) {
	if (left > greyThreshold) {
		lastWhiteLeftEncoder = encoder;
	}
	if (right > greyThreshold) {
		lastWhiteRightEncoder = encoder;
	}

	switch (state) {
	case State::NO_CROSS:
		if ((fabs(encoder - lastWhiteLeftEncoder) > lineWidth
				&& fabs(encoder - lastWhiteRightEncoder) > lineWidth / 2)
				|| (fabs(encoder - lastWhiteLeftEncoder) > lineWidth / 2
					&& fabs(encoder - lastWhiteRightEncoder) > lineWidth)) {
			state = State::X_FOUND;
			xCrossCount++;
		} else if (fabs(encoder - lastWhiteLeftEncoder) > lineWidth) {
			state = State::LEFT_FOUND;
			leftCrossCount++;
		} else if (fabs(encoder - lastWhiteRightEncoder) > lineWidth) {
			state = State::RIGHT_FOUND;
			rightCrossCount++;
		}
		break;
	case State::LEFT_FOUND:
	case State::RIGHT_FOUND:
	case State::X_FOUND:
		if (fabs(encoder - lastWhiteLeftEncoder) < lineWidth / 2
				&& fabs(encoder - lastWhiteRightEncoder) < lineWidth / 2) {
			state = State::GO_OUT;
			lastCrossEncoder = encoder;
		}
		break;
	case State::GO_OUT:
		if (fabs(encoder - lastCrossEncoder) > lineWidth * 6) {
			state = State::NO_CROSS;
		}
	}
}

void CrossCheck::setGreyThreshold(float threshold) {
	greyThreshold = threshold;
}

void CrossCheck::setLineWidth(float width) {
	lineWidth = width;
}


void CrossCheck::resetCounters() {
	leftCrossCount = rightCrossCount = xCrossCount = 0;
	lastWhiteLeftEncoder = lastWhiteRightEncoder = lastCrossEncoder = encoder;
	state = State::GO_OUT;
}


}

/* namespace ev3 */
