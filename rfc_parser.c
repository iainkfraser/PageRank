/*
* (C) Iain Fraser - GPLv3
* iainkfraser.blogspot.co.uk
* Code for parsing directory of RFCs and finding citations. Used by
* #sundayfun #a2 and #a3. 
*/

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "rfc_parser.h"

/*
* Traverse a directory looking for rfc[0-9]+.txt. Pass found files & dir
* to a callback. 
*/
int traverse_rfc_dir( const char* dir, rfc_file_callback cb, void* arg ){
	DIR* dirp;
	struct dirent* dp;
	int i, n;
	char ext[3];

	if( !( dirp = opendir( dir ) ) )
		return errno;		


	do {
		if( ( dp = readdir( dirp ) )  ){
			n = 0;					
			if( sscanf( dp->d_name, "rfc%d.txt%n", &i, &n ) ==  1 && dp->d_name[n] == '\0'  )
				cb( dir, dp->d_name, i, arg ); 
		}
	} while( dp );


	closedir( dirp );
	return 0;

}


/*
* Open file and search for words and RFC links. Pass on information to a callback. 
*/
void parse_rfc( const char* dir, const char* filename, int i, void* arg ){
	char filepath[ 255 ];
	char *word,*failtest;
	int c, len,link, count = 0;

	int buflen = 32, wordlen = 0;
	word = (char*)malloc( buflen );

	struct parse_arg* pa = ( struct parse_arg* )arg;

	if( ( len = strlen( dir ) ) && dir[ len - 1 ] != '/' )
		sprintf( filepath, "%s/%s", dir, filename );
	else
		sprintf( filepath, "%s%s", dir, filename );

	FILE* f = fopen( filepath, "r" );
	if( !f ) 	// fail silently for now
		return;
	
	do {
		c = fgetc( f );

		if( isspace( c ) || ispunct( c )  ){
			if( wordlen == 0 )
				continue;

			// check for rfc naively 
			if( wordlen == 3 && !strncmp( word, "RFC", wordlen ) && fscanf( f, "%d", &link ) == 1 )
				pa->callback( i, true, word, wordlen, link, pa->arg ); 
			else
				pa->callback( i, false, word, wordlen, count, pa->arg );	
		

			wordlen = 0;
			count++;
			continue;
		} else {	

			if( ++wordlen > buflen ){
				failtest = (char*)realloc( word, ( buflen <<= 1 ) );
				if( !failtest )
					goto error;	
				else	
					word = failtest;	
			}

			word[ wordlen - 1 ] = c;
		} 
	} while( !feof( f ) );
	

error:
	free( word );
	fclose( f );
}
