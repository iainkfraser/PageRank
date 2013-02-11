/*
* (C) Iain Fraser - GPLv3
* iainkfraser.blogspot.co.uk
* Interface for parsing directory of RFCs and finding citations. Used by
* #sundayfun #a2 and #a3. 
*/

#ifndef _RFC_PARSER_H_
#define _RFC_PARSER_H_

#include <stdbool.h>

typedef void (*rfc_file_callback)( const char*, const char*, int, void* );
typedef void (*rfc_word_callback)( int, bool, const char*, int, int, void* arg );

struct parse_arg {
	rfc_word_callback callback;
	void* arg;
};

int traverse_rfc_dir( const char* dir, rfc_file_callback cb, void* arg );
void parse_rfc( const char* dir, const char* filename, int i, void* arg );
#endif 
