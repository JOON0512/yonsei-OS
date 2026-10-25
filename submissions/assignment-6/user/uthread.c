#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/uthread.h"

int thread_create(thread_t *t, void*(*func)(void*), void *arg) {
  
  *t = tfork(func, arg);

  return !(*t);
  
}

int thread_join(thread_t t, void **ret) {
  
  return !(twait(t, ret));
}

void thread_exit(void *ret) {

  texit(ret);
}

void thread_mutex_init(thread_mutex_t *mtx) {
  // Initialize the mutex lock as zero.
  mtx->locked = 0;
}

void thread_mutex_destroy(thread_mutex_t *mtx) {
}

void thread_mutex_lock(thread_mutex_t *mtx) {
  // Use the spinlock's implementation.
    while(__sync_lock_test_and_set(&mtx->locked, 1) != 0)
    ;
  __sync_synchronize();
}

void thread_mutex_unlock(thread_mutex_t *mtx) {
  // Use the spinclock's implementation.
  __sync_synchronize(); // synchronize (riscv fence instruction)
  __sync_lock_release(&mtx -> locked); // release lock (set to 0)  
}
