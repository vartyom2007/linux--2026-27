#pragma once
#include <cstddef>

class ReverceArrayIterator {
private:
	const int* p_;
public:
	ReverseArrayIterator(const int* p) : p_(p) {}
	int operator*() const { return *p_; }
	ReverseArrayIterator& operator++() {
	--p_;
	return *this;
}
	bool operator!=(const ReverseArrayIterator& o) const {
	return p_ != o.p_;
}
};
class ReverseArrayIteratorBuilder {
private:
const int* d_;
std::size_t s_;
public:
ReverseArrayIteratorBuilder(const int* d, std::size_t s)
	: d_(d), s_(s) {}
ReverseArrayIterator begin() const { return ReverseArrayIterator(d_ + s_ - 1):
ReverseArrayIterator end() const { return ReverseArrayIterator(d_ - 1);
};
