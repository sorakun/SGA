#include <iostream>
#include "Edge.h"
#include "Node.h"

namespace GraphLib
{

#ifdef GRAPH_ENABLE_DEBUG
	size_t Edge::instanceCount = 0;
#endif

	Edge::Edge(Node * start, Node * end)
	{
		this->index = -1;
		this->start = start;
		this->end = end;

#ifdef GRAPH_ENABLE_DEBUG
		std::cout << "Creating an instance of Edge between " << start->getIndex() << " and " << end->getIndex() << '\n';
		instanceCount++;
#endif
	}

	Edge::~Edge()
	{
#ifdef GRAPH_ENABLE_DEBUG
		std::cout << "Removing an instance of Edge between " << start->getIndex() << " and " << end->getIndex() << '\n';
		instanceCount--;
#endif
	}

	bool Edge::equals(Node *node1, Node *node2)const
	{
		if (node1 == this->start)
			return (node2 == this->end);
		return false;
	}

	bool Edge::equals(const Edge *e)const
	{
		Node *node1 = e->start, *node2 = e->end;
		if (node1 == this->start)
			return (node2 == this->end);
		return false;
	}

	bool Edge::contains(Node * node)const
	{
		return start == node || end == node;
	}
}