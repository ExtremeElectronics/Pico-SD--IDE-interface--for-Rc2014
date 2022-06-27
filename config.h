#define HAVE_FCNTL_H 1
#define HAVE_LIMITS_H 1
#define HAVE_UNISTD_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_MODE_T 0
#define NEED_NCURSES 0
#define HAVE_NCURSES_NCURSES_H 0

#define CPMFS_DEBUG 1

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifndef _POSIX_PATH_MAX
#define _POSIX_PATH_MAX 260
#endif

#include <time.h>



