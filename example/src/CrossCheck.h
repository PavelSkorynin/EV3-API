/*
 * CrossCheck.h
 *
 *  Created on: May 15, 2019
 *      Author: pavel
 */

#ifndef CROSSCHECK_H_
#define CROSSCHECK_H_

#include <Process.h>

#include <Wire.h>

namespace ev3 {

class CrossCheck: public Process {
public:
	CrossCheck(const WireF & left, const WireF & right, const WireF & encoder);
	~CrossCheck();

	void update(float seconds) override;

	void setGreyThreshold(float threshold);
	void setLineWidth(float width);
	void resetCounters();

	inline int getLeftCrossCount() const {
		return leftCrossCount;
	}
	inline int getRightCrossCount() const {
		return rightCrossCount;
	}
	inline int getXCrossCount() const {
		return xCrossCount;
	}
	inline int getTotalCrossCount() const {
		return xCrossCount + leftCrossCount + rightCrossCount;
	}

private:

	enum class State {
		NO_CROSS,
		LEFT_FOUND,
		RIGHT_FOUND,
		X_FOUND,
		GO_OUT
	};

	WireF left;
	WireF right;
	WireF encoder;

	int leftCrossCount;
	int rightCrossCount;
	int xCrossCount;

	float lastWhiteLeftEncoder;
	float lastWhiteRightEncoder;
	float lastCrossEncoder;

	State state;

	float greyThreshold;
	float lineWidth;
};

} /* namespace ev3 */

#endif /* CROSSCHECK_H_ */
