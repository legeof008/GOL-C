#include "sm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char **argv ) {
	int n = argc > 1 ? atoi(argv[1]) : 10;  
	int m = argc > 2 ? atoi(argv[2]) :  7;
	int nit = argc > 3 ? atoi(argv[3]) :  n/10;

	int (*solve)( sm_t *, double *, double *, double, int, double * ) = sm_cg_solve;
	if( argc > 4 && strcmp( argv[4], "GS" ) == 0 ) solve = sm_gs_solve;

	// Build FD matrix for n by m grid ordered by columns
	int nn = n*m;
	int nz = (n-1)*(m-1)*5; // internal nodes: 5 entries by each
  nz += 2*m+2*(n-2); // boundary: 1 entry per each (as for Dirichlet BC)

	sm_t *A = alloc_sm( nn, nz );
	double *r = malloc( nn * sizeof *r );
	double *x = calloc( nn, sizeof *x );
	int neq = 0;
	nz = 0;
	for( int i= 0; i < n; i++ ) {
		for( int j= 0; j < m; j++ ) {
			A->ia[neq] = nz;
			if( i == 0 || j == 0 || i == n-1 || j == m-1 ) {
				A->ja[nz] = neq;
			  A->a[nz++] = 1.0;
				r[neq] = 0;
			} else {
			  A->ja[nz] = neq-m; // left
				A->a[nz++] = -1.0;
        A->ja[nz] = neq-1; // below
				A->a[nz++] = -1.0;
				A->ja[nz] = neq;
				A->a[nz++] = 4.0;
				A->ja[nz] = neq+1;
				A->a[nz++] = -1.0;
				A->ja[nz] = neq+m;	
				A->a[nz++] = -1.0;
				r[neq] = -1.0;
			}
			neq++;
		}
	}
	A->ia[neq]= nz;

	if( nn < 10 ) {
		for( int i= 0; i < A->n; i++ )
			printf( "%g\n", r[i] );
		print_sm( A, stdout );
		write_sm( A, stdout );
	}

	double err;
  int result= solve( A, r, x, 1e-6, nit, &err );

	fprintf( stderr, "solver made %d iterations, err=%g\n", result, err );

	for( int i= 0; i < n; i++ ) {
		for( int j= 0; j < m; j++ ) {
			neq = i*m+j;
			fprintf( stdout, "%g %g %g\n", i*0.1, j*0.1, x[neq] );
		}
	}	

	free( x );
	free( r );
	free_sm( A );

	return 0;
}
	
