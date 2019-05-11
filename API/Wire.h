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

template<typename T>
class Wire {
public:
	Wire(const T & value)
		: provider([value] { return value; })
	{
		printf("create const wire");
	}

	Wire(const std::function<T()> & provider)
		: provider(provider)
	{
		printf("create function wire");
	}

	Wire(const Wire<T> & w)
		: provider(w.provider)
	{
		printf("create copied wire");
	}

	Wire(Wire<T> && w)
		: provider(w.provider)
	{
		printf("create moved wire");
	}

	template<typename V>
	Wire(const Wire<V> & w)
		: provider([w] { return (T)w.getValue(); } )
	{
		printf("create converted wire");
	}

	inline T getValue() const
	{
		return provider();
	}

	inline operator T() const
	{
		return provider();
	}

protected:
	const std::function<T()> provider;
};

/// operator +
template<typename T>
Wire<T> operator+(const Wire<T> & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() + w2.getValue(); } );
}

template<typename T>
Wire<T> operator+(const Wire<T> & w1, const T & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() + w2; } );
}

template<typename T>
Wire<T> operator+(const T & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1 + w2.getValue(); } );
}

/// operator -

template<typename T>
Wire<T> operator-(const Wire<T> & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() - w2.getValue(); } );
}

template<typename T>
Wire<T> operator-(const Wire<T> & w1, const T & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() - w2; } );
}

template<typename T>
Wire<T> operator-(const T & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1 - w2.getValue(); } );
}

/// operator *

template<typename T>
Wire<T> operator*(const Wire<T> & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() * w2.getValue(); } );
}

template<typename T>
Wire<T> operator*(const Wire<T> & w1, const T & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() * w2; } );
}

template<typename T>
Wire<T> operator*(const T & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1 * w2.getValue(); } );
}

/// operator /

template<typename T>
Wire<T> operator/(const Wire<T> & w1, const Wire<T> & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() / w2.getValue(); } );
}

template<typename T>
Wire<T> operator/(const Wire<T> & w1, const T & w2) {
	return Wire<T>([w1, w2] { return w1.getValue() / w2; } );
}

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
