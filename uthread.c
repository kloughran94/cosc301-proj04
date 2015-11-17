#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"



#define PGSIZE 	4096
#define NPROC		64

void *refs[NPROC];
/*
 * This is where you'll need to implement the user-level functions
 */

void lock_init(lock_t *lock) {
	lock->is_locked = 0;
}

void lock_acquire(lock_t *lock) {
	while(xchg(&lock->is_locked, 1) != 0);
}

void lock_release(lock_t *lock) {
	xchg(&lock->is_locked, 0);
}

int thread_join(int pid) {
	int n_pid = join(pid);
	if (n_pid != -1){
		free(refs[n_pid]);
	}
	return n_pid;
}

int thread_create(void (*start_routine)(void *), void *arg) {
	void *stack = malloc(PGSIZE*2);
	if ((uint)stack %PGSIZE){
		stack = stack + (PGSIZE - (uint)stack % PGSIZE);
	}
	int n_pid = clone(start_routine, arg, stack);
	refs[n_pid] = stack;
	
	return n_pid;
}
