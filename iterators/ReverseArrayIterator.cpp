#include <iostream>
#include <Utils/Iterator/ReverseArrayIterator.h>

int main(){
int arr[] = {1, 2, 3, 4, 5};

ReverseArrayIteratorBuilder rit(
	arr,
	sizeof(arr) / sizeof(arr[0])
);

for(auto it = rit.begin(); it != rit.end(); ++it)
{
	std::cout << *it << " ";
}
return 0;
}


