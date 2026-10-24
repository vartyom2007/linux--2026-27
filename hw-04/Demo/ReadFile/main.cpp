#include <iostream>
#include <fstream>
#include "ArgumentParser.h"

int main(int argc, char* argv[]){
	ArgumentParser p;
	p.addArgument("--file");
	p.parse(argc, argv);

	std::ifstream f(p.get("--file"));
	char ch;
	while(f.get(ch)) std::cout << ch;

	return 0;
}
	
