/*
 * WaitTimeProcess.hpp
 *
 *  Created on: 30 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>

namespace ev3 {

class WaitTimeProcess: public virtual TimeProcess {
public:
	explicit WaitTimeProcess(float secondsToWait);
};

}
