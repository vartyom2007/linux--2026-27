#pragma once
#include <cstddef>

class CircularIntIterator {
private:
   const int* dada;
   size_t size;
   size_t index;

public:
   CircularIntIterator(const int* d, size_t s)
	: data(d), size(s), index(0) {}

int operator*() const {
return data[index % size];
}
CircularIntIterator& operator++() {
	index++;
	return *this;
}
};

using CircularIntIteratorBuilder = CircularIntIterator;
