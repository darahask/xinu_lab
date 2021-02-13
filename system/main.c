/*  main.c  - main */

#include <xinu.h>

void myfunc(){
	while (1)
	{
		
	}
	
}

process	main(void)
{
	/* Run the Xinu shell */

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	pid32 pid = create_user_proc(myfunc,2048,20,"myfunc",0);
	kprintf("Custom processs pid:%d",pid);
	resume(pid);
	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
