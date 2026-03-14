#include <stdio.h>
#include <sys/stat.h>
#include "driver_uart.h"

/* Redirect printf output to UART */
int _write(int fd, char *ptr, int len)
{
    (void)fd; // ignore file descriptor

    for (int i = 0; i < len; i++)
    {
        if (ptr[i] == '\n')   // add carriage return for terminals
            uart_write_char('\r');

        uart_write_char(ptr[i]);
    }

    return len;
}

/* Minimal stubs for newlib */
int _fstat(int fd, struct stat *st)
{
    (void)fd;
    st->st_mode = S_IFCHR;  // character device
    return 0;
}

int _close(int fd) { (void)fd; return -1; }
int _isatty(int fd) { (void)fd; return 1; }
int _lseek(int fd, int ptr, int dir) { (void)fd; (void)ptr; (void)dir; return 0; }
int _read(int fd, char *ptr, int len) { (void)fd; (void)ptr; (void)len; return -1; }
void *_sbrk(int incr) { extern char _end; static char *heap = &_end; char *prev = heap; heap += incr; return prev; }