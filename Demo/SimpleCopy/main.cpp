#include <iostream>
#include <fstream>
#include "ArgumentParser.h"

int main(int argc, char* argv[]){
	ArgumentParser p;
	p.addArgument("--src");
	p.addArgument("--dst");
	p.parse(argc, argv);

	std::ifstream in(p.get("--src"));
	std::ofstream out(p.get("--dst"));
	
	char ch;
	while(in.get(ch)){
		out.put(ch);
}
return 0;
}

