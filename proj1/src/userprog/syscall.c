#include "userprog/syscall.h"
#include "userprog/pagedir.h"

#include <stdio.h>
#include <syscall-nr.h>
#include <string.h>

#include "threads/thread.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
static uint32_t *esp;

void
syscall_init (void) {
    intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

bool is_valid_ptr(const void *usr_addr) {
    if (usr_addr == NULL) return false;
    if (!is_user_vaddr(usr_addr)) return false;

    struct thread *cur = thread_current();
    return (pagedir_get_page(cur->pagedir, usr_addr)) != NULL;
}

int usr_to_kernel_ptr(const void *usr_addr) {
    if (!is_user_vaddr(usr_addr)) sys_exit(-1);
    if (usr_addr < USER_VADDR_BOTTOM) sys_exit(-1);

    void *k_virtaddr = (void *)pagedir_get_page(thread_current()->pagedir, usr_addr);
    if (!k_virtaddr) sys_exit(-1);

    return (int)k_virtaddr;
}

int get_arg (struct intr_frame *f) {
    int *addr = (int *) f->esp + 1;
    if (!is_user_vaddr((const void *) addr)) sys_exit(-1);
    if ((const void *) addr < USER_VADDR_BOTTOM) sys_exit(-1);

    return *addr;
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
    esp = f->esp;

    if (!is_valid_ptr(esp)) sys_exit(-1);
    if (!is_valid_ptr(esp + 1)) sys_exit(-1);
    if (!is_valid_ptr(esp + 2)) sys_exit(-1);
    if (!is_valid_ptr(esp + 3)) sys_exit(-1);

    switch (*(uint32_t*)(f->esp)){
        case SYS_HALT:
            shutdown_power_off();
            break;
        case SYS_EXIT:
            sys_exit(*(uint32_t*)(f->esp+4));
            break;
        case SYS_EXEC :
        {
            int arg = get_arg(f);
            arg = usr_to_kernel_ptr((const void *) arg);
            f->eax = process_execute((const char *) arg);
            break;
        }
        case SYS_WAIT :
            f->eax = process_wait(*(uint32_t*)(f->esp+4));
            break;
        case SYS_READ:
            f->eax = sys_read ((int)*(uint32_t*)(f->esp+4), (void*)*(uint32_t*)(f->esp+8),
                               (unsigned)*(uint32_t*)(f->esp+12));
            break;
        case SYS_WRITE:
            f->eax = sys_write((int)*(uint32_t*)(f->esp+4), (const void*)*(uint32_t*)(f->esp+8),
                               (unsigned)*(uint32_t*)(f->esp+12));
            break;
        case SYS_FIB:
            f->eax = sys_fibonacci((int)*(uint32_t*)(f->esp+4));
            break;
        case SYS_MAX:
            f->eax = sys_max_of_four_int((int)*(uint32_t*)(f->esp+4), (int)*(uint32_t*)(f->esp+8),
                                         (int)*(uint32_t*)(f->esp+12), (int)*(uint32_t*)(f->esp+16));
            break;
        default:
            break;
    }
    return;
}

int sys_read (int fd, void *buffer, unsigned length){
    int cnt;
    if (fd == 0) {
        for (cnt = 0; cnt < (int)length; cnt++) {
            if (input_getc() == '\0') break;
        }
        return cnt;
    }
    return -1;
}

int sys_write (int fd, const void *buffer, unsigned length){
    if (fd == 1) {
        putbuf((char*)buffer, (size_t)length);
        return length;
    }
    return -1;
}

int sys_max_of_four_int(int a, int b, int c, int d) {
    int mxv1 = a > b? a: b;
    int mxv2 = c > d? c: d;
    int res = mxv1 > mxv2? mxv1: mxv2;
    return res;
}

int sys_fibonacci (int n) {
    if (n == 1 || n == 2) return 1;

    int a, b;
    int tmp;

    a = b = 1;
    for (int i = 2; i < n; i++){
        tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

void sys_exit (int status){
    printf("%s: exit(%d)\n", thread_name(), status);
    thread_current()->exit_status = status;
    thread_exit();
}