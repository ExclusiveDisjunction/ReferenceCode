#pragma once

template<typename T> class Vector;
template<typename T> class VectorIterator;

template<typename T>
class ContainerSegment
{
private:
	ContainerSegment() : Next(nullptr), Last(nullptr) {}
	ContainerSegment(const T& Value) : Value(Value), Next(nullptr), Last(nullptr) {}

	ContainerSegment<T>* Next;
	ContainerSegment<T>* Last;

	T Value;

	friend Vector<T>;
	friend VectorIterator<T>;
};