/*  main.c  - main */

#include <xinu.h>

void myfunc1(){
	while (1)
	{
		
	}
	
}

void myfunc2(){
	while (1)
	{
		
	}
	
}

process	main(void)
{
	/* Run the Xinu shell */

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	pid32 pid1 = create_user_proc(myfunc1,2048,20,"myfunc1",0);
	kprintf("Custom processs pid1:%d\n",pid1);
	resume(pid1);
	pid32 pid2 = create_user_proc(myfunc2,2048,40,"myfunc2",0);
	kprintf("Custom processs pid2:%d\n",pid2);
	resume(pid2);
	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
