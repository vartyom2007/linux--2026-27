#include <fstream>
#include <cstdio>
#include "ArgumentParser.h"

int main(int argc, char* argv[]){
	ArgumentParser p;
	p.addArgument("--file");
	p.parse(argc, argv);

	std::string path = p.get("--file");
	std::ofstream f(path, std::ios::in | std::ios::binary);
	
	f.seekp(0, std::ios::end);
	std::streampos size = f.tellp();
	f.seekp(0);
	for(int i = 0; i < size; ++i) f.put('\0');
	
	f.close();
	std::remove(path.c_str());
}

