/*
 * Sensor.cpp
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#include "Sensor.h"
#include "core/ev3_sensor.h"

namespace ev3 {

template<typename T>
inline uint8_t P(const T &value) {
	return static_cast<uint8_t>(value);
}

Sensor::Mode Sensor::selectedModes[4] = { Mode::NO_SENSOR, Mode::NO_SENSOR, Mode::NO_SENSOR, Mode::NO_SENSOR };

Sensor::Sensor(Port port)
	: port(port)
	, mode(Mode::NO_SENSOR)
	, value(0)
	, valueInput([this] { return this->value; }) {
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
	selectedModes[(int)port] = mode;

	if (SetAllSensorMode((int)selectedModes[0], (int)selectedModes[1], (int)selectedModes[2], (int)selectedModes[3]) != 0)
	{
//		printf("Failed to set mode %d for sensor at port %d", P(mode), P(port));
		return;
	}
}

void Sensor::updateInputs(float timestampSeconds) {
	value = ReadSensor(P(port));
}

void Sensor::updateOutputs(float timestampSeconds) {

}



} /* namespace ev3 */
