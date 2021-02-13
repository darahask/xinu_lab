/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

pid32 getwinner()
{
	struct procent *entry;
	int random_num = rand()%total_tickets;
	kprintf("Total Tickets: %d and random number is %d\n", total_tickets, random_num);
	int counter = 0;
	int i = 0;
	pid32 pid = firstid(userlist);
	//kprintf("First id is %d\n", firstid(userlist));
	entry = &proctab[pid];
	//kprintf("random_num pid is %d\n", random_num);
	kprintf("Total tickets is %d", total_tickets);
	while(1)
	{
		counter = counter + entry->tickets_num;
		if(counter>random_num){
			return pid;
		}
		//if(queuetab[pid].qnext == EMPTY)
		//	return currpid;
		pid = queuetab[pid].qnext;
		entry = &proctab[pid];
	}
}

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */
  int old_pid = currpid;
	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

			if((strncmp(ptold->prname, "prnull",6)==0))
		{
			if(nonempty(readylist))
			{
				if (ptold->prstate == PR_CURR) { 
					ptold->prstate = PR_READY;
					//kprintf("I am inserting here1");
					insert(currpid, readylist, ptold->prprio);
				}
				currpid = dequeue(readylist);
				ptnew = &proctab[currpid];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;		/* Reset time slice for process	*/
			}
			else
			{
				if(nonempty(userlist))
				{	
					if (ptold->prstate == PR_CURR) { 
						ptold->prstate = PR_READY;
					//	kprintf("I am inserting here2");
						insert(currpid, readylist, ptold->prprio);
					}
				//implement this
				//kprintf("Came here in get userlist2 because %d\n", nonempty(userlist));
				pid32 winner = getwinner();
				ptnew = &proctab[winner];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;
				dequeue_list(winner, userlist);
				currpid = winner;
				}
				else
					return;
			}
		}
		else if(ptold->sys_call == 1){
			if (ptold->prstate == PR_CURR) { 
				if(ptold->prprio > firstkey(readylist)){
					return;}
				ptold->prstate = PR_READY;
				insert(currpid, readylist, ptold->prprio);
				currpid = dequeue(readylist);
				ptnew = &proctab[currpid];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;		/* Reset time slice for process	*/
				}
			else if(firstid(readylist) != NULLPROC){
					currpid = dequeue(readylist);
					ptnew = &proctab[currpid];
					ptnew->prstate = PR_CURR;
					preempt = QUANTUM;		/* Reset time slice for process	*/
				}
			else if(nonempty(userlist))
			{
				if (ptold->prstate == PR_CURR) { 
					ptold->prstate = PR_READY;
					//kprintf("I am inserting here3");
					insert(currpid, readylist, ptold->prprio);
					}
				//kprintf("going to winner from here3 because %d\n", nonempty(userlist));
				pid32 winner = getwinner();
				//kprintf("Got winner as %d\n");
				//kprintf("I am stucke here1\n");
				ptnew = &proctab[winner];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;
				//kprintf("I am stucke here\n");
				//winner=dequeue(userlist);
				dequeue_list(winner, userlist);
				currpid = winner;
				//kprintf("Dequeued %d", currpid);
			}
			else
			{
				if (ptold->prstate == PR_CURR) { 
					if(ptold->prprio > firstkey(readylist))
						return;
					ptold->prstate = PR_READY;
					//kprintf("I am inserting here4");
					insert(currpid, readylist, ptold->prprio);
				}
				currpid = dequeue(readylist);
				ptnew = &proctab[currpid];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;		/* Reset time slice for process	*/
			}
		}
		else{
			//kprintf("I came here after killing %d\n", currpid);
			if(firstid(readylist)!=0)
			{	
				//kprintf("I came here inside after killing %d\n", currpid);
				if(ptold->prstate == PR_CURR){
					ptold->prstate = PR_READY;
					//kprintf("I am inserting here5");
					insert_list(currpid, userlist, ptold->tickets_num);	
				}
				currpid = dequeue(readylist);
				ptnew = &proctab[currpid];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;
			}
			else if (nonempty(userlist))
			{
			//	kprintf("I came here inside after killing %d\n", currpid);
				//kprintf("I came here\n");
				if(ptold->prstate == PR_CURR){
					ptold->prstate = PR_READY;
					insert_list(currpid, userlist, ptold->tickets_num);	
			}
				//insert logic here
				pid32 winner = getwinner();
			//	kprintf("Winner is %d and total_tickets is %d", winner, total_tickets);
				ptnew = &proctab[winner];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;
				currpid = winner;
				//kprintf("Winnder is %d\n", winner);
				dequeue_list(winner, userlist);
			}
			else
			{
				//kprintf("I came here in the end after killing %d\n", currpid);
				//kprintf("I came here coz %d\n", !isempty(userlist));
				if(ptold->prstate == PR_CURR){
				ptold->prstate = PR_READY;
				//kprintf("I am inserting here7");
				insert_list(currpid, userlist, ptold->tickets_num);	
				}
				currpid = dequeue(readylist);
				ptnew = &proctab[currpid];
				ptnew->prstate = PR_CURR;
				preempt = QUANTUM;
			}
		}
	if(old_pid!=currpid)
		#ifndef CTXSW
			kprintf("Old process: %d\tnew process: %d\n", old_pid,currpid);
		#endif

	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
