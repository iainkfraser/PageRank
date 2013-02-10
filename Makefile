pagerank : main.c examples.h
	$(CC) main.c -o pagerank -lm -std=c99

clean : 
	rm pagerank
