/*
 * Wire.h
 *
 *  Created on: 09 May 2019
 *      Author: Pavel Skorynin
 */

#ifndef WIRE_H_
#define WIRE_H_

#include <functional>
#include <memory>

namespace ev3 {

/**
 * Поток данных. Аналог провода в EV3-G. Позволяет соединять между собой датчики, моторы и процессы.
 */
template<typename T>
class Wire {
public:
	/**
	 * Конструктор от функции (лямбды). Функция будет вызываться каждый раз при вызове getValue.
	 * @param provider функция, возвращающая текущее значение на проводе
	 */
	Wire(const std::function<T()> & provider)
		: provider(provider)
	{
	}

	/**
	 * Конструктор от фиксированного значения. getValue всегда будет возвращать это значение.
	 * @param value фиксированное значение
	 */
	Wire(const T & value)
		: provider([value]() -> T { return value; })
	{
	}

	/**
	 * Конструктор копирования. Провод будет использовать ту же функцию, что и копируемый объект.
	 * @param w провод для копирования
	 */
	Wire(const Wire<T> & w)
		: provider(w.provider)
	{
	}

	/**
	 * Конструктор копирования. Провод будет использовать ту же функцию, что и копируемый объект.
	 * @p
	 */
	Wire(Wire<T> && w)
		: provider(w.provider)
	{
	}

	/**
	 * Конструктор копирования с приведением типов.
	 * Провод будет использовать ту же функцию, что и копируемый объект.
	 * @p
	 */
	template<typename V>
	Wire(const Wire<V> & w)
		: provider([w] { return (T)w.getValue(); } )
	{
	}

	/**
	 * Возвращает текущее значение на проводе
	 * @return текущее значение
	 */
	inline T getValue() const
	{
		return provider();
	}

	/**
	 * Оператор приведения к типу данных значения на проводе
	 */
	inline operator T() const
	{
		return provider();
	}

protected:
	const std::function<T()> provider;
};

/**
 * Оператор сложения для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий сумму значений на проводах
 */
template<typename T>
Wire<T> operator+(const Wire<T> & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() + w2.getValue(); } );
}

/**
 * Оператор сложения для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий сумму значений на проводах
 */
template<typename T>
Wire<T> operator+(const Wire<T> & w1, const T & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() + w2; } );
}

/**
 * Оператор сложения для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий сумму значений на проводах
 */
template<typename T>
Wire<T> operator+(const T & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1 + w2.getValue(); } );
}

/// operator -
/**
 * Оператор вычитания для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий разность значений на проводах
 */
template<typename T>
Wire<T> operator-(const Wire<T> & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() - w2.getValue(); } );
}

/**
 * Оператор вычитания для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий разность значений на проводах
 */
template<typename T>
Wire<T> operator-(const Wire<T> & w1, const T & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() - w2; } );
}

/**
 * Оператор вычитания для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий разность значений на проводах
 */
template<typename T>
Wire<T> operator-(const T & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1 - w2.getValue(); } );
}

/// operator *

/**
 * Оператор умножения для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий произведение значений на проводах
 */
template<typename T>
Wire<T> operator*(const Wire<T> & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() * w2.getValue(); } );
}

/**
 * Оператор умножения для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий произведение значений на проводах
 */
template<typename T>
Wire<T> operator*(const Wire<T> & w1, const T & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() * w2; } );
}

/**
 * Оператор умножения для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий произведение значений на проводах
 */
template<typename T>
Wire<T> operator*(const T & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1 * w2.getValue(); } );
}

/// operator /

/**
 * Оператор деления для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий частное значений на проводах
 */
template<typename T>
Wire<T> operator/(const Wire<T> & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() / w2.getValue(); } );
}

/**
 * Оператор деления для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий частное значений на проводах
 */
template<typename T>
Wire<T> operator/(const Wire<T> & w1, const T & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() / w2; } );
}

/**
 * Оператор деления для проводов
 * @param w1
 * @param w2
 * @return провод, возвращающий частное значений на проводах
 */
template<typename T>
Wire<T> operator/(const T & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1 / w2.getValue(); } );
}


typedef Wire<float> WireF;
typedef Wire<int> WireI;
typedef Wire<bool> WireB;
typedef Wire<std::string> WireS;

} /* namespace ev3 */

#endif /* WIRE_H_ */
