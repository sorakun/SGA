#ifndef RANDOMGRAPH_H
#define RANDOMGRAPH_H

#include <iostream>
#include <random>
#include "Graph.h"

namespace GraphLib
{
	class RandomGraph: public Graph
	{
	private:
		std::size_t n;
		double p;

	public:
		RandomGraph(std::size_t n, double p, bool directed = true): Graph(directed)
		{
			assert(n > 0);
			assert(p<=1 && p>=0); 

			/* preparing random generator */
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dis(0, 1);

			for(std::size_t i = 0; i<n; i++)
			{
				std::size_t node = createNode();
				//std::cout << "creating node " << i << '\n';
			}

			/* Graph is non-oriented, we can check half of the matrix (ignore diagonals) */
			for(std::size_t i=1; i<n;i++)
			{
				for(std::size_t j=0; j<i;j++)
				{
					/* std::cout << "Attempting to link " << i << " with " << j << '\n'; */
					if(dis(gen)<=p)
						link(i, j);
				}
			}

		}

		~RandomGraph()
		{

		}

	};
}
#endif // RANDOMGRAPH_H