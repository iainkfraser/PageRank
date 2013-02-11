/*
* (C) Iain Fraser - GPLv3
* iainkfraser.blogspot.co.uk
* Code for generating adjacency matrix based on RFC citations. 
*/

#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "rfc_parser.h"


struct rfc_bounds {
	int min;
	int max;
};

struct matrix_info {
	float* 	matrix;
	int	n;
};

static void rfc_matrix_generate( int rfc, bool iscite, const char* word, int len, int ref_os_pos, void* arg ){
	struct matrix_info* mi = ( struct matrix_info* )arg;
	float (*matrix)[mi->n] = ( float(*)[mi->n] ) mi->matrix;
 
	if( iscite && rfc != ref_os_pos )
		matrix[ rfc - 1  ][ ref_os_pos - 1 ] = 1.0;	
	

/*	if( iscite && rfc != ref_os_pos )
		printf("%d -> %d\n", rfc, ref_os_pos ); */
}

static void do_rfc_bounds( const char* dir, const char* filename, int i, void* arg ){
	struct rfc_bounds* rfb = ( struct rfc_bounds* ) arg;		

	
	if( i < rfb->min )
		rfb->min = i;
	
	if( i > rfb->max )
		rfb->max = i;
}


float* generate_matrix( const char* dir, int* n ){
	struct rfc_bounds rfb;
	struct matrix_info mi;

	// calculate matrix size first 
	rfb.min = INT_MAX;
	rfb.max = INT_MIN; 
	traverse_rfc_dir( dir, do_rfc_bounds, (void*)&rfb );
	assert( rfb.max > rfb.min );
	mi.n = ( rfb.max - rfb.min ) + 1;	// inclusive 
	
	printf("There are %d rfc pages\n", mi.n );

	size_t sz = sizeof( float ) * mi.n * mi.n;

	// assume compiler is row-order 
	mi.matrix = malloc( sz ); 
	if( !mi.matrix )
		return NULL;
	memset( mi.matrix, 0, sz );	// not safe according to standard by insane otherwise 


	struct parse_arg pa = { .callback = &rfc_matrix_generate, .arg = &mi }; 
	traverse_rfc_dir( dir, parse_rfc, (void*)&pa );

	*n = mi.n;
	return mi.matrix;
}


