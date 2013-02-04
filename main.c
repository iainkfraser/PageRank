/*
* (C) Iain Fraser - GPLv2
* iainkfraser.blogspot.co.uk
* Code for #sundayfun article 2. On the original google PageRank algorithm.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/*
* The PageRank paper uses manhattan geomtery for distance. However
* other sources use Euclidean. 
*/
#ifdef PR_MAG
#define VEC_MAG		vec_manhattan_mag
#else
#define VEC_MAG		vec_euclid_mag
#endif

#define EPSILON		0.01f		// steady state precision 

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
		iter++;

		if( isr )
			mat_vec_mul( n, t, r, A ), vec_scale( n, t, d ), vec_add( n, t, k);			
		else
			mat_vec_mul( n, r, t, A ), vec_scale( n, r, d ), vec_add( n, r, k );			
		

		isr = !isr;
	} while( VEC_MAG( n, r, t ) > EPSILON );

	if( !isr )
		memcpy( r, t, sizeof( t ) ); 

	printf("found in %d iterations\n", iter );

} 

void PageRank( int n, float r[n], float A[n][n] ){
	float k[n];
	const float d = 0.85;	
	const float c = ( 1 - d ) / n; 

	for( int i = 0; i < n; i++ )
		k[i] = c; 

	do_pagerank( n, r, A, 0.85, k );
}

// simple dominant eigenvector algorithm i.e. PageRank without damping 
void eigenvector( int n, float r[n], float A[n][n] ){
	float k[n];
	memset( k, 0, sizeof( k ) );
	do_pagerank( n, r, A, 1.0f, k );
}

/*
* How to handle pages with no outgoing links ( sinks ) .
*	1) Original paper suggests removing any links to those pages.
*	2) Have an outgoing link from sink to every other page in system. 
* I choose the later.  
*/
void fix_danglelink( int n, float A[n][n] ){
	const float p = 1.0f / ( (float)n-1.0f );
	int i,j;

	for( i = 0; i < n; i++ ){
		for( j = 0; j < n && A[i][j] == 0.0f; j++ );	

		// so row i is a sink, set outlinks to all.
		if( j >= n ){
			for( j = 0; j < n; j++ ){
				if( j == i ) continue; 
				A[i][j] = p; 
			} 
		}
	}
}

#define EXAMPLE1	{			\
		{ 0.3f, 0.3f, 0.4f },		\
		{ 0.4f, 0.4f, 0.2f },		\
		{ 0.5f, 0.3f, 0.2f }		\
	}	

#define EXAMPLE2	{			\
		{ 0.0f, 0.5f, 0.5f },		\
		{ 0.0f, 0.0f, 1.0f },		\
		{ 1.0f, 0.0f, 0.0f }		\
	}	

#define EXAMPLE3 	{			\
		{ 0.0f, 0.5f, 0.5f },		\
		{ 0.0f, 0.0f, 0.0f },		\
		{ 1.0f, 0.0f, 0.0f }		\
	};

int main( int argc, char* argv[] ){
	#define n	3

	float A[n][n] = EXAMPLE2;
	float r[n] = { 0.3333f, 0.33333f, 0.3333f };

	fix_danglelink( n, A );
	mat_print( n, n, A );

//	mat_vec_mul( n, r, A);
//	eigenvector( n, r, A );
	PageRank( n, r, A );

//	mat_print( n, n, A );
	mat_print( 1, n, r );

	return 0;
}


