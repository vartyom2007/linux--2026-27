#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include "ArgumentParser.h"

int main(int argc, char* argv[]){
	ArgumentParser p;
	p.addArgument("--file");
	p.parse(argc, argv);

	std::string path = p.get("--file");
	int fd = open(path.c_str(), O_RDWR);
	if(fd == -1) return 1;
	
	off_t size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	
	char zero = '\0';
	for(int i = 0; i < size; ++i){
		write(fd, &zero, 1);
}
	close(fd);
	unlink(path.c_str());
	
	
	return 0;
}

