#include <reent.h>
//#include <sys/errno.h>
#include <time.h>

#include "ametal.h"


//#include "aw_mem.h"
//
//#include "aw_time.h"
//#include "aw_serial.h"

#include "fcntl.h"
#include "unistd.h"
#include "sys/stat.h"

struct timeval {
  long    tv_sec;         /* seconds */
  long    tv_usec;        /* and microseconds */
};

/* Reentrant versions of system calls.  */

int
_close_r(struct _reent *ptr, int fd)
{
	return close(fd);
}

int
_execve_r(struct _reent *ptr, const char * name, char *const *argv, char *const *env)
{
	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

int
_fcntl_r(struct _reent *ptr, int fd, int cmd, int arg)
{
	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

int
_fork_r(struct _reent *ptr)
{
	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

int
_fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

int
_getpid_r(struct _reent *ptr)
{
	return 0;
}

int
_isatty_r(struct _reent *ptr, int fd)
{
	if (fd >=0 && fd < 3) return 1;

	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

int
_kill_r(struct _reent *ptr, int pid, int sig)
{
	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

int
_link_r(struct _reent *ptr, const char *old, const char *new)
{
	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

_off_t
_lseek_r(struct _reent *ptr, int fd, _off_t pos, int whence)
{
	return lseek(fd, pos, whence);
}

int
_mkdir_r(struct _reent *ptr, const char *name, int mode)
{
	return 0;
}

int
_open_r(struct _reent *ptr, const char *file, int flags, int mode)
{
	return open(file, flags, mode);
}

_ssize_t 
_read_r(struct _reent *ptr, int fd, void *buf, size_t nbytes)
{
    if (fd < 3) {
        return 0;
    }
	return 0;
}

int
_rename_r(struct _reent *ptr, const char *old, const char *new)
{
	return 0;
}

extern char __heap_start__;		/* Defined by the linker */
extern char __heap_end__;		/* Defined by the linker */

void *
_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	(void)(__heap_end__);
	if (heap == NULL) {
	    heap = (unsigned char *)&__heap_start__;
	}
	prev_heap = heap;

	heap += incr;

	return (caddr_t) prev_heap;
}



int
_stat_r(struct _reent *ptr, const char *file, struct stat *pstat)
{
	return stat(file, pstat);
}

_CLOCK_T_
_times_r(struct _reent *ptr, struct tms *ptms)
{
	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

int
_unlink_r(struct _reent *ptr, const char *file)
{
	return unlink(file);
}

int
_wait_r(struct _reent *ptr, int *status)
{
	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

extern int am_debug_output_str (char *buf, size_t nbytes);

_ssize_t
_write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
	if (fd < 3)	{
	    return am_debug_output_str((char *)buf, nbytes);
	} else	{
	    return write(fd, buf, nbytes);
	}
}

int
_gettimeofday_r(struct _reent *ptr, struct timeval *__tp, void *__tzp)
{
	/* return "not supported" */
	ptr->_errno = AM_ENOTSUP;
	return -1;
}

void
_exit (int status)
{
#ifdef RT_USING_MODULE
	rt_module_t module;

	module = rt_module_self();
	if (module != RT_NULL)
	{
		struct rt_list_node *list;
		struct rt_object *object;

		rt_enter_critical();
		
        /* delete all threads in the module */
        list = &module->module_object[RT_Object_Class_Thread].object_list;
        while (list->next != list)
        {
            object = rt_list_entry(list->next, struct rt_object, list);
            if (rt_object_is_systemobject(object) == RT_TRUE)
            {
                /* detach static object */
                rt_thread_detach((rt_thread_t)object);
            }
            else
            {
                /* delete dynamic object */
                rt_thread_delete((rt_thread_t)object);
            }
        }
		/* delete main thread */
		rt_thread_delete(module->module_thread);
		rt_exit_critical();

		/* re-schedule */
		rt_schedule();
	}
#endif
	
//	rt_kprintf("thread:%s exit with %d\n", rt_thread_self()->name, status);

	while (1);
}

void 
_system(const char *s)
{
    /* not support this call */
    return;
}
