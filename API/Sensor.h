/*
 * Sensor.h
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "Device.h"

#include "core/ev3_constants.h"
#include "core/ev3_sensor.h"
#include "Wire.h"

namespace ev3 {

/**
 * Класс, отвечающий за взаимодействие с датчиком, подключенным к блоку EV3.
 */
class Sensor: public Device {
public:
	/**
	 * Порт подключения датчика к блоку EV3
	 */
	enum class Port {
		P1 = IN_1,//!< P1
		P2 = IN_2,//!< P2
		P3 = IN_3,//!< P3
		P4 = IN_4 //!< P4
	};

	/**
	 * Тип датчика и режим его работы
	 */
	enum class Mode {
		NO_SENSOR = NO_SEN,		// No sensor connected / Unknown sensor
		//Touchsenor
		TOUCH = TOUCH_PRESS,	// Press

		//Lightsensor
		COLOR_REFLECT = COL_REFLECT,	// Reflect
		COLOR_AMBIENT = COL_AMBIENT,	// Ambient
		COLOR_COLOR = COL_COLOR,		// Color
		COLOR_RGB = COL_COLOR_RGB,		// RGB Color

		//Ultrasonic
		ULTRASONIC_DISTANCE_CM = US_DIST_CM,	// Dist in cm
		ULTRASONIC_DISTANCE_MM = US_DIST_MM,	// Dist in mm
		ULTRASONIC_DISTANCE_INCHES =  US_DIST_IN,	// Dist in inch

		//Gyroscope
		GYROSCOPE_ANGLE = GYRO_ANG,		// angle
		GYROSCOPE_RATE = GYRO_RATE,		// rate

		//Infrared
		INFRARED_PROXIMITY = IR_PROX,		// Proximity
		INFRARED_SEEK = IR_SEEK,		// Seek
		INFRARED_REMOTE = IR_REMOTE,	// Remote Control

		//NXT
		NXT_INFRARED_SEEK = NXT_IR_SEEKER, // Infrared Seeker
		NXT_TEMPERATURE_C = NXT_TEMP_C, 	// Temperature in C
		NXT_TEMPERATURE_F = NXT_TEMP_F, 	// Temperature in F
	};

	~Sensor();

	/**
	 * Порт, к которому подключен датчик
	 * @return порт
	 */
	inline Port getPort() const {
		return port;
	}
	/**
	 * Тип датчика и режим его работы
	 * @return
	 */
	inline Mode getMode() const {
		return mode;
	}

	/**
	 * Текущее значение на датчике
	 * @return целочисленное значение
	 */
	int getValue() const;
	/**
	 * Текущее значение на датчике
	 * @return поток данных
	 */
	WireI getValueWire() const;

	/**
	 * Утановка режима работы датчика
	 * @param mode режим работы датчика
	 */
	void setMode(const Mode & mode);

	/**
	 * Обновление данных, получаемых с датчика
	 * @param timestampSeconds текущее время в секундах
	 */
	void updateInputs(float timestampSeconds) override;
	/**
	 * Наследуемый метод. Ничего не делает
	 * @param timestampSeconds текущее время в секундах
	 */
	void updateOutputs(float timestampSeconds) override;

protected:
	Sensor(Port port);

	Port port;
	Mode mode;

	int value;

	WireI valueInput;

	friend class EV3;
};

/**
 * Датчик отражённого света
 */
class ReflectedLightSensor : public Sensor {
public:
	/**
	 * Конструктор
	 * @param port
	 */
	ReflectedLightSensor(Port port);

	/**
	 * Минимальное исходное значение на датчике
	 * Используется для калибровки. getValue будет возвращать нормализованное значение
	 * @param minValue
	 */
	void setMinValue(int minValue);
	/**
	 * Максимальное исходное значение на датчике
	 * Используется для калибровки. getValue будет возвращать нормализованное значение
	 * @param maxValue
	 */
	void setMaxValue(int maxValue);

protected:
	int minValue;
	int maxValue;
};

#pragma pack(push, 1)
/**
 * Цвет в компонентах R (красный), G (зелёный), B (синий)
 * Каждая компонента может принимать значения в итервале [0, 255]
 */
struct RGB {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char _unused;

	RGB(unsigned char r, unsigned char g, unsigned char b)
	: b(b), g(g), r(r), _unused(0) {

	}
};

/**
 * Цвет в компонентах H (hue, тон), S (saturation, насыщенность), V (value, значение)
 * Допустимые значения:
 * H - [0, 360]
 * S - [0, 100]
 * V - [0, 100]
 */
struct HSV {
	short h;
	unsigned char s;
	unsigned char v;

	HSV(short h, unsigned char s, unsigned char v)
	: h(h), s(s), v(v) {

	}
};
#pragma pack(pop)

/**
 * Преобразование цвета из модели RGB в HSV
 * @param rgb
 * @return hsv
 */
HSV rgbToHsv(const RGB &rgb);

/**
 * Преобразование цвета из модели HSV в RGB
 * @param hsv
 * @return rgb
 */
RGB hsvToRgb(const HSV &hsv);

/**
 * Датчик цвета. Возвращает цвет в двух форматах: RGB и HSV
 */
class ColorSensor : public Sensor {
public:
	/**
	 * Конструктор
	 * @param port
	 */
	ColorSensor(Port port);

	/**
	 * Цвет в формате RGB
	 * @return цвет
	 */
	RGB getRGBColor() const;
	/**
	 * Цвет в формате RGB
	 * @return цвет
	 */
	Wire<RGB> getRGBColorWire() const;

	/**
	 * Цвет в формате HSV
	 * @return цвет
	 */
	HSV getHSVColor() const;
	/**
	 * Цвет в формате HSV
	 * @return цвет
	 */
	Wire<HSV> getHSVColorWire() const;

	/**
	 * Минимальное исходное значение компоненты R на датчике
	 * Используется для калибровки. getColor будет возвращать нормализованное значение
	 * @param minValue
	 */
	void setMinRValue(unsigned char minValue);
	/**
	 * Максимальное исходное значение компоненты R на датчике
	 * Используется для калибровки. getColor будет возвращать нормализованное значение
	 * @param maxValue
	 */
	void setMaxRValue(unsigned char maxValue);

	/**
	 * Минимальное исходное значение компоненты G на датчике
	 * Используется для калибровки. getColor будет возвращать нормализованное значение
	 * @param minValue
	 */
	void setMinGValue(unsigned char minValue);
	/**
	 * Максимальное исходное значение компоненты G на датчике
	 * Используется для калибровки. getColor будет возвращать нормализованное значение
	 * @param maxValue
	 */
	void setMaxGValue(unsigned char maxValue);

	/**
	 * Минимальное исходное значение компоненты B на датчике
	 * Используется для калибровки. getColor будет возвращать нормализованное значение
	 * @param minValue
	 */
	void setMinBValue(unsigned char minValue);
	/**
	 * Максимальное исходное значение компоненты B на датчике
	 * Используется для калибровки. getColor будет возвращать нормализованное значение
	 * @param maxValue
	 */
	void setMaxBValue(unsigned char maxValue);

protected:
	RGB minColor;
	RGB maxColor;
};

typedef std::shared_ptr<Sensor> SensorPtr;

} /* namespace ev3 */

#endif /* SENSOR_H_ */
