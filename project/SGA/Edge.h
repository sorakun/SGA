#ifndef EDGE_H
#define EDGE_H

#include "Node.h"

namespace GraphLib
{
	class Edge
	{
	private:
		std::size_t index;

	public:
		Node *start, *end;

		std::size_t value;

		Edge(Node *, Node *);
		~Edge();
		bool equals(Node *, Node *)const;
		bool equals(const Edge *e)const;
		/* Check if an endge contains a node */
		bool contains(Node *)const;

#ifdef GRAPH_ENABLE_DEBUG
		static size_t instanceCount;
#endif
	};
}
#endif // EDGE_H
