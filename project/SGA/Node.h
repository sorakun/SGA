#ifndef NODE_H
#define NODE_H

#include <vector>

namespace GraphLib
{
	class Node
	{
		/* Node indexes/PRs are not to be modified outside the graph. */

		friend class Graph;
		friend class Edge;
	protected:
		double rank;
		//Cluster parentCluster;
		/* Index in graph */
		std::size_t index;
		/* any other data is to be added here. */

		double getRank();
		void setRank(double);
		void setIndex(std::size_t);

	public:
		Node();
		~Node();

		std::size_t& getIndex();

#ifdef GRAPH_ENABLE_DEBUG
		static size_t instanceCount;
#endif
	};

	class Cluster
	{
		Node *centroid;
	};
}
#endif // NODE_H
