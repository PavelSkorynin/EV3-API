/*
 * Sensor.cpp
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#include "Sensor.h"
#include "core/ev3_sensor.h"

#include "EV3Math.h"
#include <cmath>
#include <functional>

namespace ev3 {

template<typename T>
inline uint8_t P(const T &value) {
	return static_cast<uint8_t>(value);
}

Sensor::Sensor(Port port)
	: port(port)
	, mode(Mode::NO_SENSOR)
	, value(0)
	, valueInput([this] { return this->value; }) {
}

Sensor::~Sensor() {
}

int Sensor::getValue() const {
	return valueInput.getValue();
}
WireI Sensor::getValueWire() const {
	return valueInput;
}

void Sensor::setMode(const Mode & mode) {
	if (this->mode == mode) {
		return;
	}

	this->mode = mode;
}

void Sensor::updateInputs(float timestampSeconds) {
	value = ReadSensor(P(port));
}

void Sensor::updateOutputs(float timestampSeconds) {

}

//////////////////////////////////////////////////////

ReflectedLightSensor::ReflectedLightSensor(Port port)
	: Sensor(port)
	, minValue(0)
	, maxValue(100)
{
	setMode(ev3::Sensor::Mode::COLOR_REFLECT);
	valueInput = WireI(std::function<int()>([this] () -> int {
		return (int)clamp<float>(round(map<float>(this->value, this->minValue, this->maxValue, 0, 100)), 0, 100);
	}));
}

void ReflectedLightSensor::setMinValue(int minValue) {
	this->minValue = minValue;
}

void ReflectedLightSensor::setMaxValue(int maxValue) {
	this->maxValue = maxValue;
}

///////////////////////////////////////////////////////

HSV rgbToHsv(const RGB &rgb) {
	float normR = rgb.r / 255.0f;
	float normG = rgb.g / 255.0f;
	float normB = rgb.b / 255.0f;
	float max = ev3::max(ev3::max(normR, normG), normB);
	float min = ev3::min(ev3::min(normR, normG), normB);

	float h = 0;
	if (max == min) {
		h = 0;
	} else if (max == normR && normG >= normB) {
		h = 60 * (normG - normB) / (max - min);
	} else if (max == normR && normG < normB) {
		h = 60 * (normG - normB) / (max - min) + 360;
	} else if (max == normG) {
		h = 60 * (normB - normR) / (max - min) + 120;
	} else if (max == normB) {
		h = 60 * (normR - normG) / (max - min) + 240;
	}

	float s = max == 0 ? 0 : (1 - min / max);
	float v = max;

	return HSV((short)round(h), (unsigned char)round(s * 100), (unsigned char)round(v * 100));
}

RGB hsvToRgb(const HSV &hsv) {
	float C = (float)hsv.s * (float)hsv.v / 10000;
	float X = C * (1 - fabs(fmod(hsv.h / 60.0f, 2) - 1));
	float m = hsv.v / 100 - C;
	float Rs, Gs, Bs;

	if(hsv.h >= 0 && hsv.h < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if(hsv.h >= 60 && hsv.h < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if(hsv.h >= 120 && hsv.h < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if(hsv.h >= 180 && hsv.h < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if(hsv.h >= 240 && hsv.h < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	return RGB(round((Rs + m) * 255), round((Gs + m) * 255), round((Bs + m) * 255));
}

ColorSensor::ColorSensor(Port port)
	: Sensor(port)
	, minColor(0, 0, 0)
	, maxColor(255, 255, 255)
{
	setMode(ev3::Sensor::Mode::COLOR_RGB);
	valueInput = WireI([this] () {
		auto value = this->value;
		auto rgbColor = *(RGB*)(&value);
		rgbColor.r = (unsigned char)clamp<float>(round(map<float>(rgbColor.r, this->minColor.r, this->maxColor.r, 0, 255)), 0, 255);
		rgbColor.g = (unsigned char)clamp<float>(round(map<float>(rgbColor.g, this->minColor.g, this->maxColor.g, 0, 255)), 0, 255);
		rgbColor.b = (unsigned char)clamp<float>(round(map<float>(rgbColor.b, this->minColor.b, this->maxColor.b, 0, 255)), 0, 255);
		return *(int*)(&rgbColor);
	});
}

RGB ColorSensor::getRGBColor() const {
	auto value = valueInput.getValue();
	return *(RGB*)(&value);
}

Wire<RGB> ColorSensor::getRGBColorWire() const  {
	return Wire<RGB>([this] () -> RGB {
		return this->getRGBColor();
	});
}

HSV ColorSensor::getHSVColor() const {
	return rgbToHsv(getRGBColor());
}

Wire<HSV> ColorSensor::getHSVColorWire() const {
	return Wire<HSV>([this] () -> HSV {
		return this->getHSVColor();
	});
}

int ColorSensor::getColorIndex(const std::vector<int> &colors, int blackVThreshold, int whiteSThreshold, int whiteVThreshold) const {
	auto hsv = getHSVColor();

	if (hsv.h == 0 && hsv.s == 0 && hsv.v == 0) {
		return NO_COLOR;
	} else if (hsv.v < blackVThreshold) {
		return BLACK_COLOR;
	} else if (hsv.s < whiteSThreshold && hsv.v > whiteVThreshold) {
		return WHITE_COLOR;
	}

	int minDistance = 1000;
	int colorIndex = 0;
	for (auto color : colors) {
		// ищем, к какому цвету ближе всего увиденный
		// находим минимальное расстояние от h до цвета на циклической шкале
		int dist1 = abs(hsv.h - color);
		int dist2 = abs(hsv.h - color + 360);
		int dist3 = abs(hsv.h - color - 360);
		int minDist = std::min(dist1, std::min(dist2, dist3));
		if (minDist < minDistance) {
			colorIndex = color;
			minDistance = minDist;
		}
	}

	return colorIndex;
}


void ColorSensor::setMinRValue(unsigned char minValue) {
	minColor.r = minValue;
}

void ColorSensor::setMaxRValue(unsigned char maxValue) {
	maxColor.r = maxValue;
}

void ColorSensor::setMinGValue(unsigned char minValue) {
	minColor.g = minValue;
}

void ColorSensor::setMaxGValue(unsigned char maxValue) {
	maxColor.g = maxValue;
}

void ColorSensor::setMinBValue(unsigned char minValue) {
	minColor.b = minValue;
}

void ColorSensor::setMaxBValue(unsigned char maxValue) {
	maxColor.b = maxValue;
}

//// FakeSensor

FakeSensor::FakeSensor(const WireI &valueInput)
	: Sensor(ev3::Sensor::Port::FAKE) {
	this->valueInput = valueInput;
	setMode(ev3::Sensor::Mode::FAKE);
}

FakeSensor::FakeSensor(int value)
	: Sensor(ev3::Sensor::Port::FAKE) {
	this->valueInput = ev3::WireI([value] { return value; });
	setMode(ev3::Sensor::Mode::FAKE);
}

void FakeSensor::updateInputs(float timestampSeconds) {
}

void FakeSensor::updateOutputs(float timestampSeconds) {
}



} /* namespace ev3 */
