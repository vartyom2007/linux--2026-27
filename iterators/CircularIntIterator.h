#pragma once
#include <cstddef>

class CircularIntIterator {
private:
	const int* data_;
	std::size_t size_;
	std::size_t current_index_;
public:
CircularIntIterator(const int* data, std::size_t size)
	:data_(data), size_(size), current_index_(0) {}
	

	int operator*() const {
	return data_[current_index_];
	}
	CircularIntIterator& operator++() {
	if(size_ > 0){
	current_index_ = (current_index_ + 1) % size_;
	}
	return *this;
	}
};

class CircularIntIteratorBuilder : public CircularIntIterator{
public:
	CircularIntIteratorBuilder(const int* data, std::size_t size)
	:CircularIntIterator(data, size) {}
 };
