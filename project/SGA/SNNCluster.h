/*
 * Shared Nearest Neighbor Clustering
*/

#include <Graph.h>
#include <iostream>

namespace SNNCluster
{
	void start(GraphLib::Graph *g, std::size_t t)
	{
		std::cout << "Applying SNN cluster with t = " << t << '\n';
		for(const auto& e: g->getEdges())
		{
			std::size_t i1 = e->start->getIndex();
			std::size_t i2 = e->end->getIndex();
			std::size_t value = 0;
		
			for(std::size_t i = 0; i < g->getNodesCount(); i++)
			{
				if(g->get(i1, i) == 1 && g->get(i2, i) == 1)
					value++;
			}

			e->value = value;
			if(value < t)
				g->unlink(i1, i2);
		}
		g->calculateAjdMatrix();
	}
}