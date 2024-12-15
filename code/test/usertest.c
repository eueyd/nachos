#include "syscall.h"

int main()
{
	int result=0;
	int ret;
	int fd;

	result=Add(12,13);

	result=Square(5);

	result=Max(32,44);

	ret = Create("lg.txt");
	ret = Create("ly.txt");
	fd = Open("lg.txt");
	
	Write("hello world",12,fd);
	Close(fd);

	Delete("ly.txt");
	Write("Exec testing",12,1);
	Exec("add.noff");
	Halt();

}
