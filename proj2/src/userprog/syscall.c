#include "userprog/syscall.h"
#include "userprog/pagedir.h"

#include <stdio.h>
#include <syscall-nr.h>
#include <string.h>

#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/synch.h"

#include "filesys/filesys.h"
#include "filesys/file.h"


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

    /////// 추가
    char *p = (char *)usr_addr;
    while (*p++ != '\0');
    /////////

    void *k_virtaddr = (void *)pagedir_get_page(thread_current()->pagedir, usr_addr);
    if (!k_virtaddr) sys_exit(-1);

    return (int)k_virtaddr;
}

int get_arg (struct intr_frame *f) {
    int* addr = (int *) f->esp + 1;
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


            // proj 2
        case SYS_OPEN:
        {
            int arg = get_arg(f);
            arg = usr_to_kernel_ptr((const void *) arg);
            f->eax = sys_open((const char *) arg);
            break;
        }
        case SYS_CLOSE:
        {
            int fd = (int)*(uint32_t*)(f->esp+4);
            sys_close(fd);
            break;
        }
        case SYS_CREATE:
        {
            const char* file = (const char*)*(uint32_t*)(f->esp+4);
            if (file == NULL) sys_exit(-1);

            unsigned sz = (unsigned)*(uint32_t*)(f->esp+8);
            f->eax = filesys_create(file, sz);
            break;
        }
        case SYS_REMOVE:
        {
            const char* file = (const char*)*(uint32_t*)(f->esp+4);
            if (file == NULL) sys_exit(-1);

            f->eax = filesys_remove(file);
            break;
        }
        case SYS_FILESIZE:
        {
            int fd = (int)*(uint32_t*)(f->esp+4);
            if (thread_current()->fds[fd] == NULL) sys_exit(-1);

            f->eax = file_length(thread_current()->fds[fd]);
            break;
        }
        case SYS_TELL:
        {
            int fd = (int)*(uint32_t*)(f->esp+4);
            if (thread_current()->fds[fd] == NULL) sys_exit(-1);

            f->eax = file_tell(thread_current()->fds[fd]);
            break;
        }
        case SYS_SEEK:
        {
            int fd = (int)*(uint32_t*)(f->esp+4);
            if (thread_current()->fds[fd] == NULL) sys_exit(-1);

            unsigned pos = (unsigned)*(uint32_t*)(f->esp+8);
            file_seek(thread_current()->fds[fd], pos);
            break;
        }
        default:
            break;
    }
    return;
}

int sys_read (int fd, void *buffer, unsigned length){
    if (!is_valid_ptr(buffer)) sys_exit(-1);

    lock_acquire(&file_lock);
    if (fd == 0) {  // std input
        int cnt;
        for (cnt = 0; cnt < (int)length; cnt++) {
            if (input_getc() == '\0') break;
        }
        lock_release(&file_lock);
        return cnt;
    }

    if (!(thread_current()->fds[fd])) {
        lock_release(&file_lock);
        sys_exit(-1);
    }
    if (fd >= 3) {  // not std input
        int ret;
        ret = file_read(thread_current()->fds[fd], buffer, length);
        lock_release(&file_lock);
        return ret;
    }

    lock_release(&file_lock);
    return -1;
}

int sys_write (int fd, const void *buffer, unsigned length){
    lock_acquire(&file_lock);
    if (fd == 1) {  // std output
        putbuf((char*)buffer, (size_t)length);
        lock_release(&file_lock);
        return length;
    }

    if (thread_current()->fds[fd] == NULL) {
        lock_release(&file_lock);
        sys_exit(-1);
    }
    if (fd >= 3) {  // not std output
        int ret;
        ret = file_write(thread_current()->fds[fd], buffer, length);
        lock_release(&file_lock);
        return ret;
    }

    lock_release(&file_lock);
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

int sys_open(const char* file) {
    lock_acquire(&file_lock);
    struct file *fp = filesys_open(file);
    if (fp == NULL) {
        lock_release(&file_lock);
        return -1;
    }

    for (int fd = 3; fd < FILE_NUM; fd++) {
        if (!(thread_current()->fds[fd])) {
            thread_current()->fds[fd] = fp;
            lock_release(&file_lock);
            return fd;
        }
    }

    lock_release(&file_lock);
    return -1;
}

void sys_close(int fd) {
    if (thread_current()->fds[fd] == NULL) sys_exit(-1);
    file_close(thread_current()->fds[fd]);
    // thread_current()->handling_fp = NULL;
    thread_current()->fds[fd] = NULL;
}

void sys_exit (int status){
    printf("%s: exit(%d)\n", thread_name(), status);
    thread_current()->exit_status = status;

    /////////////////// 추가
    // close
    for (int fd = 3; fd < FILE_NUM; fd++){
        if (thread_current()->fds[fd] != NULL) sys_close(fd);
    }
    // reap
    for (struct list_elem *cur = list_begin(&(thread_current())->child);
         cur != list_end(&(thread_current()->child)); cur = list_next(cur)) {

        process_wait(list_entry(cur, struct thread, child_elem)->tid);
    }
    file_close(thread_current()->handling_fp);
    //////////////////////

    thread_exit();
}
