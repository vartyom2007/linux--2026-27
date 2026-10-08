#ifndef FILTER_INTEGER_ITERATOR_H
#define FILTER_INTEGER_ITERATOR_H

#include <cstddef>

class FilterIntegerIterator{
private:
	const int* data;
	size_t size;
	size_t index;
	bool (*filter)(int);

	void advanceToNextValid(){
		while(index < size && !filter(data[index])) {
			index++;
}
}
public:

	FilterIntegerIterator(const int* d, size_t s, size_t i, bool (*f)(int))
		: data(d), size(s), index(i), filter(f) {
		if(index < size){
			advanceToNextValid();
}
}
	int operator*() const {
		return data[index];
}
	FilterIntegerIterator& operator++(){
		if(index < size){
		index++;
		advanceToNextValid();
}
		return *this;
}
	bool operator !=(const FilterIntegerIterator& other){
		return index != other.index;
}
};
class FilterIntegerIteratorBuilder{
private:
	const int* data;
	std::size_t size;
	bool (*filter)(int);
public:
	FilterIntegerIteratorBuilder(const int* d, size_t s, bool (*f)(int))
	: data(d), size(s), filter(f) {}
	FilterIntegerIterator begin() const{
	return FilterIntegerIterator(data, size, 0, filter);
	}
	FilterIntegerIterator end() const{
	return FilterIntegerIterator(data, size, size, filter);
}
};
#endif
