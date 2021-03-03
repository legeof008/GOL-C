#ifndef _SM_H_
#define _SM_H_

// Sparse matrix - Compressed Row Storage

#include <stdio.h>

typedef struct {
	int n;         // # of equations
	int *ia;       // beginings of rows in ja and ia vectors (length == n+1)
	int *ja;       // ja - column indices, each row starts from ja[ia[row]] and ends before ja[ia[row+1]]
	double *a;     // a - nonzero entries, row storage, each rows starts from a[ia[row]] etc.
} sm_t;

sm_t *alloc_sm( int n, int nonzeroes );

sm_t *read_sm( FILE * );

void write_sm( sm_t *, FILE *);

void sm_vec_dot( sm_t *, double *, double * );

int sm_gs_solve( sm_t *, double *, double *, double, int, double * );

int sm_cg_solve( sm_t *, double *, double *, double, int, double * );

void print_sm( sm_t *, FILE * );

int sm_put( sm_t *, int, int, double );

int sm_add( sm_t *, int, int, double );

double sm_get( sm_t *, int, int );

void free_sm( sm_t * );

#endif
