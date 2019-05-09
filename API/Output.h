/*
 * Output.h
 *
 *  Created on: 09 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

namespace ev3 {

class Output {
public:
	Output();
	virtual ~Output();

	virtual float getValue() = 0;
};

} /* namespace ev3 */

#endif /* OUTPUT_H_ */
