/*
 * Sensor.cpp
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#include "Sensor.h"
#include "core/ev3_sensor.h"

namespace ev3 {

Sensor::Sensor(Port port)
	: port(port)
	, mode(NO_SENSOR)
	, value(0)
	, valueInput([this] { return value; }) {

}

Sensor::~Sensor() {
}

WireI Sensor::getValue() const {
	return valueInput;
}

void Sensor::setMode(const Mode & mode) {
	if (this->mode == mode) {
		return;
	}

	this->mode = mode;
	if (SetSensorMode(port, mode) != 0)
	{
		printf("Failed to set mode %d for sensor at port %d", mode, port);
		return;
	}

	updateInputs();
}

void Sensor::updateInputs() {
	value = ReadSensor(port);
}

void Sensor::updateOutputs() {

}



} /* namespace ev3 */
