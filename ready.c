/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/
qid16   userlist;

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	kprintf("In ready : %s,%d\n",prptr->prname,prptr->sys_call);
	if(prptr->sys_call == 1){
		insert(pid, readylist, prptr->prprio);
	}
	else{
		insert(pid,userlist,prptr->tickets_num);
	}
	resched();

	return OK;
}
