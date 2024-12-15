/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"


//added by lg
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/file.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

//added by lg
int SysSquare(int op1)
{
	return op1*op1;
}

int SysMax(int op1, int op2)
{
	if (op1>op2)
	{
		return op1;
	}
	else
	{
		return op2;
	}

}


int SysWrite(int Addr, int Count, int FileID)
{
	int ch;
	int i=0;
	while ( i < Count){
		kernel->machine->ReadMem(Addr,1, &ch);
		kernel->fileSystem->Write(FileID, (char *)&ch, 1);
		Addr ++;
		i++; 
	}
	return i;
}

int SysRead(int Addr, int Count, int FileID)
{
	char ch;
	int i=0;
	while (i < Count){
		kernel->fileSystem->Read(FileID, &ch, 1);
		kernel->machine->WriteMem(Addr,1, ch);
		Addr ++;
		i++; 
	}
	return i;
}

int SysCreate(char *name)
{
	if (NULL==name)
	{
		fprintf(stderr,"invalid file name\n");
		return -1;
	}
	return kernel->fileSystem->Create(name);

}

int  SysOpen(char *name)
{

	int fd;
	printf("filename %s\n",name);
	if (NULL==name);
	//{
	//	fprintf(stderr,"invalid filename\n");
	//	return false;
	//}
	fd=OpenForReadWrite(name,FALSE);
	printf("fd is %d\n",fd);
	return fd;
}

int SysClose(int fd)
{
	return kernel->fileSystem->Close(fd);

}
int SysDelete(char *name)
{

	return int(kernel->fileSystem->Remove(name));	
}

int SysExec(int addr)
{
	int count = 0;
	int ch;
	char cmd[60];
	do {
		kernel->machine->ReadMem(addr, 1, &ch);
		addr ++;
		cmd[count]=(char)ch;
	}while (ch !='\0' && count++ <59);
	cmd[count]='\0';

	if (cmd!=NULL)
	{
		AddrSpace * space= new AddrSpace;
		ASSERT(space!=(AddrSpace*)NULL);
		if (space->Load(cmd))
		{

			space->Execute();

		}
	}

//The following code is writen by zk
//	pid_t child;
//
//	child = vfork();
//	if(child == 0) {
//		execl ("/bin/sh", "/bin/sh", "-c", cmd, NULL);
//		_exit (EXIT_FAILURE);
//	} else if(child < 0){
//		_exit (EXIT_FAILURE);
//		return EPERM;
//	}
//	return child;
}

int SysJoin(int procid)
{
	pid_t retid;
	retid = waitpid((pid_t) procid, (int*) 0, 0);
	return retid;
	
}


#endif /* ! __USERPROG_KSYSCALL_H__ */
