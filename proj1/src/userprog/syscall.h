#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "process.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include "threads/interrupt.h"

typedef int pid_t;
#define PID_ERROR ((pid_t)-1)
#define USER_VADDR_BOTTOM ((void *) 0x08048000)

void syscall_init (void);

bool is_valid_ptr (const void *usr_ptr);
int usr_to_kernel_ptr(const void *usr_addr);
int get_arg (struct intr_frame *f);

int sys_read(int fd, void *buffer, unsigned length);
int sys_write(int fd, const void *buffer, unsigned length);
void sys_exit(int status);

int sys_fibonacci(int n);
int sys_max_of_four_int(int a, int b, int c, int d);


#endif /* userprog/syscall.h */