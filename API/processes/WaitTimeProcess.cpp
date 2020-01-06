/*
 * WaitTimeProcess.cpp
 *
 *  Created on: 30 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "WaitTimeProcess.hpp"

namespace ev3 {

WaitTimeProcess::WaitTimeProcess(float secondsToWait)
	: TimeProcess([] (float) {}, secondsToWait) {
}

}
