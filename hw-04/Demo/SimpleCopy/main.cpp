#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "ArgumentParser.h"

int main(int argc, char* argv[]){
	ArgumentParser p;
	p.addArgument("--src");
	p.addArgument("--dst");
	p.parse(argc, argv);
	
	int in_fd = open(p.get("--src").c_str(), O_RDONLY);
	if(in_fd == -1) return 1;
	
	int out_fd = open(p.get("--dst").c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(out_fd == -1) { close(in_fd); return 1; }

	
	
	char ch;
	while(read(in_fd, &ch, 1) > 0){
	write(out_fd, &ch, 1);
}
	close(in_fd);
	close(out_fd);

		

return 0;
}

