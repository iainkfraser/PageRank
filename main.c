/*
* (C) Iain Fraser - GPLv3
* iainkfraser.blogspot.co.uk
* Code for #sundayfun article 2. On the original google PageRank algorithm.
*/

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "examples.h"


#define _LOADRFC	// if defined load RFC graph otherwise just run the matrices in examples.h
#define _VERBOSE
//#define _VVERBOSE
#define _GOOGLEMATRIX			// initilise google matrix then run normal eigenvector alogrithm
#define _LIMIT		50		// iteration limit 
#define EPSILON		0.001f		// steady state precision 
#define DAMP_FACTOR	0.85

/*
* The PageRank paper uses manhattan geomtery for distance. However
* other sources use Euclidean. 
*/
#ifdef PR_MAG
#define VEC_MAG		vec_manhattan_mag
#else
#define VEC_MAG		vec_euclid_mag
#endif


void mat_print( int n, int m, float A[n][m] ){
	for( int i = 0; i < n; i++ ){
		for( int j = 0; j < m; j++ ){
			printf("%f ", A[i][j] );
		}
		printf("\n");
	}
}

void mat_vec_mul( int n, float d[n], float s[n], float A[n][n] ){
	for( int i = 0; i < n; i++ ){
		d[i] = 0;
		for( int j = 0; j < n; j++ ){
			d[i] += s[j] * A[j][i];
		}
	}
}

void vec_scale( int n, float d[n], float s ){
	for( int i = 0; i < n; i++ )
		d[i] *= s;
}

void vec_add( int n, float d[n], float s[n] ){
	for( int i = 0; i < n; i++ )
		d[i] += s[i];
}

float vec_euclid_mag( int n, float a[n], float b[n] ){
	float c = 0;
	for( int i = 0; i < n; i++ )
		c += ( a[i] - b[i] ) * ( a[i] - b[i] ); 
	return sqrtf( c );
} 

float vec_manhattan_mag( int n, float a[n], float b[n] ){
	float c = 0;
	for( int i = 0; i < n; i++ )
		c += fabsf( a[i] - b[i] );
	return c;	
} 


void do_pagerank( int n, float r[n], float A[n][n], float d, float k[n] ){
	float t[n];
	bool isr = true;	
	int iter = 0;		// purely for reporting purposes 

	do 
	{
		if( isr )
			mat_vec_mul( n, t, r, A ), vec_scale( n, t, d ), vec_add( n, t, k);			
		else
			mat_vec_mul( n, r, t, A ), vec_scale( n, r, d ), vec_add( n, r, k );			
		

#ifdef _VERBOSE
		iter++;
		if( iter > _LIMIT )
			break;

#ifdef _VVERBOSE 
		mat_print( 1, n,  ( isr ? r : t ) );
#endif
#endif
		isr = !isr;
	} while( VEC_MAG( n, r, t ) > EPSILON );

	if( !isr )
		memcpy( r, t, sizeof( t ) ); 

#ifdef _VERBOSE
	printf("found in %d iterations\n", iter );
#endif

} 

void PageRank( int n, float r[n], float A[n][n] ){
	float k[n];
	const float d = DAMP_FACTOR;	
	const float c = ( 1 - d ) / (float)n; 

	for( int i = 0; i < n; i++ )
		k[i] = c; 

	do_pagerank( n, r, A, d, k );
}

// simple dominant eigenvector algorithm i.e. PageRank without damping 
void eigenvector( int n, float r[n], float A[n][n] ){
	float k[n];
	memset( k, 0, sizeof( k ) );
	do_pagerank( n, r, A, 1.0f, k );
}


void google_matrix( int n, float A[n][n], float d ){
	const float teleport_prob = ( 1 - d ) / (float)n;

	for( int i = 0; i < n; i++ )
		for( int j = 0; j < n; j++ )
			A[i][j] = d * A[i][j] + teleport_prob;
	
	
}

/*
* How to handle pages with no outgoing links ( sinks ) .
*	1) Original paper suggests removing any links to those pages.
*	2) Have an outgoing link from sink to every other page in system. 
* I choose the later.  
*/
void fix_danglelink( int n, float A[n][n] ){
	const float p = 1.0f /(float)n;
	int i,j;

	for( i = 0; i < n; i++ ){
		for( j = 0; j < n && A[i][j] == 0.0f; j++ );	

		// so row i is a sink, set outlinks to all.
		if( j >= n ){
			for( j = 0; j < n; j++ ){
				A[i][j] = p; 
			} 
		}
	}
}

void init_uniform( int n, int m, float A[n][m] ){
	int count;
	for( int i = 0; i < n; i++ ){
		count = 0;	
		for( int j = 0; j < m; j++ ){
			if( A[i][j] > 0 )
				count++;
		}
		
		const float u = 1.0f / (float)count;
		for( int j = 0; j < m; j++ ){
			if( A[i][j] > 0 )
				A[i][j] = u; 
		}

	}
}


int main( int argc, char* argv[] ){
	// I know this is horrible, but it makes it easy to switch examples so screw it! 
	#define INPUT_MATRIX( m )	\
		const int n = m ## _N;	\
		float A[ m ## _N ][ m ## _N ] = m;	\
		float r[ m ## _N ] = { [ 0 ... ( ( m ## _N ) - 1 ) ] = 1.0f }

#ifdef _LOADRFC
	extern float* generate_matrix( const char*, int* n );
	int n;
	float (*A)[n] = ( float(*)[n] ) generate_matrix( argv[1], &n );
	if( !A )
		return ENOMEM;

	float *r = malloc( sizeof( float ) * n );
	if( !r ){
		free( A );
		return ENOMEM;
	}

	for( int i = 0; i < n; i++ )
		r[i] = 1.0f;

#undef _VERBOSE		// trust me you don't want to print that matrix out :)
#undef _VVERBOSE

#else	
	// TODO: Change this to one of the examples 
	INPUT_MATRIX( SIMPLE );
#endif
	
	// init rows to be uniform distribution ( even if already so this will work ).
	init_uniform( n, n, A );	
	init_uniform( 1, n, r );

#ifdef _GOOGLEMATRIX	 
	google_matrix( n, A, DAMP_FACTOR );
#else		// if not using google matrix we still need to fix dangling links
	fix_danglelink( n, A );	
#endif

#undef _VERBOSE 	
#ifdef _VERBOSE
	printf("T:\n");	
	mat_print( n, n, A );
	printf("\n");
#endif

#ifdef _GOOGLEMATRIX	// incoporate teleportation into intial matrix
	eigenvector( n, r, A );
#else			// do teleportation every iteration
	PageRank( n, r, A );
#endif

	printf("r:\n");
#ifdef _LOADRFC
	FILE* out = fopen("pagerank.txt", "w");
	for( int i = 0; i < n; i++ )
		fprintf( out, "%f,%d\n", r[i], i + 1 );
	fclose( out );
	
	free( A );
	free( r );
#else
	mat_print( 1, n, r );
#endif
	return 0;
}


