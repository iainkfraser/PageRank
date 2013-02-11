pagerank : main.c rfc_parser.c rfc_parser.h rfc_graph.c examples.h
	$(CC) main.c rfc_parser.c rfc_graph.c -o pagerank -g3 -lm -std=c99

clean : 
	rm pagerank
