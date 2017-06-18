#ifndef ISTREAM_LIBRARY_H
#define ISTREAM_LIBRARY_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXWORDS	20000
#define MAXWORD	20
#define BUCKETSIZE 5
#define BUCKETSIZE_COL 10

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)

typedef struct _word {
    char	s[MAXWORD];	/* the word */
    int	count;		/* number of times word occurs */
    struct _word * next; /* next node */
} word;

void hello(void);

#endif