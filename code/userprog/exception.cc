// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------


void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
    case SyscallException:
      switch(type) {
      case SC_Halt:
	DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

	SysHalt();

	ASSERTNOTREACHED();
	break;

      case SC_Add:
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;
	
	ASSERTNOTREACHED();

	break;

	//added by lg
      case SC_Square:

	DEBUG(dbgSys, "Square " << kernel->machine->ReadRegister(4) << "^2" << "\n");
	
	/* Process SysSquare Systemcall*/
	result = SysSquare(/* int op1 */(int)kernel->machine->ReadRegister(4));

	DEBUG(dbgSys, "Square returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;

	ASSERTNOTREACHED();

	break;
    
     case SC_Max:

	DEBUG(dbgSys, "Max " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	
	/* Process SysMax Systemcall*/
	result = SysMax(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);
	
	/* Modify return point */
	{
	  /* set previous programm counter (debugging only)*/
	  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	  /* set next programm counter for brach execution */
	  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
	}

	return;

	ASSERTNOTREACHED();

	break;

	case SC_Write: 
		DEBUG(dbgSys, "Write " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << " + " << kernel->machine->ReadRegister(6) << "\n");
		/* Process SysWrite Systemcall*/
		result = SysWrite(/* int op1 */(int)kernel->machine->ReadRegister(4),
						              /* int op2 */(int)kernel->machine->ReadRegister(5),
						              /* int op32 */(int)kernel->machine->ReadRegister(6));
		DEBUG(dbgSys, "Write returning with " << result << "\n");
					/* Prepare Result */
		kernel->machine->WriteRegister(2, (int)result);
		/* Modify return point */
		{
		/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
		/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;

	case SC_Read:
		DEBUG(dbgSys, "Read " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5)<< " + " << kernel->machine->ReadRegister(6) << "\n");
		/* Process SysRead Systemcall*/
		result = SysRead(/* int op1 */(int)kernel->machine->ReadRegister(4),
							/* int op2 */(int)kernel->machine->ReadRegister(5),
							/* int op3 */(int)kernel->machine->ReadRegister(6));
		DEBUG(dbgSys, "Read returning with " << result << "\n");
		/* Prepare Result */
		kernel->machine->WriteRegister(2, (int)result);
		/* Modify return point */
		{
		/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
		/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;

     

	case SC_Exec:
		DEBUG(dbgSys, "Exec " << kernel->machine->ReadRegister(4)<< "\n");
		/* Process SysExec Systemcall*/
		result = SysExec(/* int op1 */(int)kernel->machine->ReadRegister(4));
		DEBUG(dbgSys, "Exec returning with " << result << "\n");
		/* Prepare Result */
		kernel->machine->WriteRegister(2, (int)result);
		/* Modify return point */
		{
		/* set previous programm counter (debugging only)*/
		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
		/* set next programm counter for brach execution */
		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;


	case SC_Join://added by zk, 3/14/2009
		DEBUG(dbgSys, "Join " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
		/* Process SysJoin Systemcall*/
		result = SysJoin(/* int op1 */(int)kernel->machine->ReadRegister(4));
		DEBUG(dbgSys, "Join returning with " << result << "\n");
		/* Prepare Result */
		kernel->machine->WriteRegister(2, (int)result);
		/* Modify return point */
		{
		/* set previous programm counter (debugging only)*/
		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
		/* set next programm counter for brach execution */
		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;

// 	case SC_Create:
//		DEBUG(dbgSys, "Create " << kernel->machine->ReadRegister(4)<<"\n");
//		/* Process SysWrite Systemcall*/
//
//		char buf[32];
//		int addr;
//		int count=0; 
//		int value;
//
//		addr = (int)kernel->machine->ReadRegister(4);
//
//		/* get the filename from the addr*/
//		for(count=0;count<32;count++)
//		{
//			kernel->machine->ReadMem(addr+count,1,&value);
//			buf[count]=(char)value;
//
//
//		}
//		/*create the file*/
//		result = SysCreate(buf);
//		DEBUG(dbgSys, "the return value for create file  with " << result << "\n");
//
//					/* Prepare Result */
//		kernel->machine->WriteRegister(2, (int)result);
//		/* Modify return point */
//		{
//		/* set previous programm counter (debugging only)*/
//			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
//		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
//			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
//		/* set next programm counter for brach execution */
//			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
//		}
//		return;
//		ASSERTNOTREACHED();
//		break;			

	case SC_Create:
		
                DEBUG(dbgSys, "Create " << kernel->machine->ReadRegister(4)<< "\n");
                /* Process SysExec Systemcall*/
		
		char buf[32];
		int count;
		int addr;
		int value;

		addr=(int)kernel->machine->ReadRegister(4);
	
		for(count=0;count<31;count++)
		{
			kernel->machine->ReadMem(addr+count,1,&value);
			buf[count]=(char)value;
		}
		result=SysCreate(buf);

		
                DEBUG(dbgSys, "create returning with " << result << "\n");
                /* Prepare Result */
                kernel->machine->WriteRegister(2, (int)result);
                /* Modify return point */
                {
                /* set previous programm counter (debugging only)*/
                kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
                /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
                kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
                /* set next programm counter for brach execution */
                kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
                }
                return;
                ASSERTNOTREACHED();
                break;


	case SC_Open:

		DEBUG(dbgSys, "Open " << kernel->machine->ReadRegister(4)<<"\n");
		/* Process SysWrite Systemcall*/

		addr = (int)kernel->machine->ReadRegister(4);
		for(count=0;count<32;count++)
		{
			kernel->machine->ReadMem(addr+count,1,&value);
			buf[count]=(char)value;
		}
		printf("filename:%s\n",buf);
		result = SysOpen(buf);
		DEBUG(dbgSys, "the return value for Open file  with " << result << "\n");
					/* Prepare Result */
		kernel->machine->WriteRegister(2, (int)result);
		/* Modify return point */
		{
		/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
		/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;

	case SC_Close:

		DEBUG(dbgSys, "close " << kernel->machine->ReadRegister(4)<<"\n");
		/* Process SysWrite Systemcall*/

		result = SysClose(kernel->machine->ReadRegister(4));
		DEBUG(dbgSys, "the return value for Close file  with " << result << "\n");
					/* Prepare Result */
		kernel->machine->WriteRegister(2, (int)result);
		/* Modify return point */
		{
		/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
		/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;

	

	case SC_Delete:

		DEBUG(dbgSys, "Delete " << kernel->machine->ReadRegister(4)<<"\n");
		/* Process SysWrite Systemcall*/

		addr=(int)kernel->machine->ReadRegister(4);
	
		for(count=0;count<31;count++)
		{
			kernel->machine->ReadMem(addr+count,1,&value);
			buf[count]=(char)value;
		}
		result = SysDelete(buf);
		DEBUG(dbgSys, "the return value for Delete file  with " << result << "\n");
					/* Prepare Result */
		kernel->machine->WriteRegister(2, (int)result);
		/* Modify return point */
		{
		/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
		/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
		/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;
	 default:
		cerr << "Unexpected system call " << type << "\n";
		break;
      }
    break;
    default:
      cerr << "Unexpected user mode exception" << (int)which << "\n";
      break;
    }
    ASSERTNOTREACHED();

}
