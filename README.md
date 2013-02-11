PageRank
========

The code associated with iainkfrasers #sundayfun article 2. It implements a trivial PageRank 
algorithm. It is unrealistic because 

1. It uses adjacency matrices 
2. Matrices are in RAM
3. No optimisations on matrix/vector operations ( e.g. cache oblivious from #sundayfun #article1 )

Having said that, it has the PageRank implementation and the plain eigenvector algorithm for
homogeneous Markov chains. For more info please read the associated article at 
iainkfraser.blogspot.co.uk titled Surfendipity.

The algorithm is applied to RFC citations. The results of running it on the latest RFCs on Feb 10 2013
are stored in pagerank.txt and order_pagerank.txt. The integer is the RFC number and the floating point
is the PageRank value.

My boy Gianni Tedesco whipped up a program to correlate RFC's with their title - rfc_pagerank_with_titles.txt .

