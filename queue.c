/* queue.c - enqueue, dequeue */

#include <xinu.h>

struct qentry	queuetab[NQENT];	/* Table of process queues	*/

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	qid16	tail, prev;		/* Tail & previous node indexes	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = queuetail(q);
	prev = queuetab[tail].qprev;

	queuetab[pid].qnext  = tail;	/* Insert just before tail node	*/
	queuetab[pid].qprev  = prev;
	queuetab[prev].qnext = pid;
	queuetab[tail].qprev = pid;
	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	dequeue(
	  qid16		q		/* ID of queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}

	pid = getfirst(q);
	queuetab[pid].qprev = EMPTY;
	queuetab[pid].qnext = EMPTY;
	return pid;
}

void	dequeue_list(
	pid32		pid,		/* ID queue to use		*/
	qid16		q		/* ID queue to use		*/
	)
{
	//kprintf("Request to dequeue %d\n", pid);
	pid32 pid_lookup;
	pid32 pid_prev;
	pid32 pid_next;
	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}
	pid_lookup = firstid(q);
	while(pid_lookup != pid)
	{
		//printf("Trying to dequeue ");
		pid_lookup = queuetab[pid_lookup].qnext;
	}
	//struct procent* entry = &proctab[pid_lookup];
	//total_tickets = total_tickets - entry->tickets_num;
	pid_prev = queuetab[pid].qprev;
	pid_next = queuetab[pid].qnext;	/* Following node in list	*/
	//kprintf("Next pid is %d and prev pid is \n", pid_next, pid_prev);
	queuetab[pid_prev].qnext = pid_next;
	queuetab[pid_next].qprev = pid_prev;
	return;
}
