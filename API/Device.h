/*
 * Device.h
 *
 *  Created on: 10 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef DEVICE_H_
#define DEVICE_H_

namespace ev3 {

	class Device {
	public:
		virtual ~Device() {}

		virtual void updateInputs() = 0;
		virtual void updateOutputs() = 0;
	};
}



#endif /* DEVICE_H_ */
