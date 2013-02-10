/*
* (C) Iain Fraser - GPLv3
* iainkfraser.blogspot.co.uk
* Code for #sundayfun article 2. On the original google PageRank algorithm.
*/

#ifndef _EXAMPLES_H_
#define _EXAMPLES_H_

#define PERIODIC_N	2
#define PERIODIC	{			\
	{ 0.0f, 1.0f },				\
	{ 1.0f, 0.0f }				\
	}

#define MARKOV1_N	2
#define MARKOV1		{			\
		{ 0.2, 0.8 },			\
		{ 0.7, 0.3 }			\
	}

#define EXAMPLE1_N	3
#define EXAMPLE1	{			\
		{ 0.3f, 0.3f, 0.4f },		\
		{ 0.4f, 0.4f, 0.2f },		\
		{ 0.5f, 0.3f, 0.2f }		\
	}	

#define EXAMPLE2_N	3
#define EXAMPLE2	{			\
		{ 0.0f, 0.5f, 0.5f },		\
		{ 0.0f, 0.0f, 1.0f },		\
		{ 1.0f, 0.0f, 0.0f }		\
	}	

#define EXAMPLE3_N	3
#define EXAMPLE3 	{			\
		{ 0.0f, 0.5f, 0.5f },		\
		{ 0.0f, 0.0f, 1.0f },		\
		{ 1.0f, 0.0f, 0.0f }		\
	};


#define TWITTER_N	12
#define TWITTER		{				\
	{ 0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1 },	\
	{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },	\
	{ 0,	1,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0 },	\
	{ 0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0 },	\
	{ 1,	0,	0,	1,	0,	0,	0,	1,	0,	0,	0,	0 },	\
	{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },	\
	{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },	\
	{ 0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0 },	\
	{ 0,	0,	0,	0,	0,	0,	1,	0,	0,	1,	1,	0 },	\
	{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },	\
	{ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },	\
	{ 0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0 } \
}

#define SIMPLE_N	4
#define SIMPLE {			\
	{ 0, 1, 1, 0 },			\
	{ 0, 0, 0, 1 },			\
	{ 0, 0, 0, 1 },			\
	{ 1, 0, 0, 0 }			\
}

#define MATRIX_SIZE( m ) 	m ## _N
#endif
