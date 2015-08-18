#include <iostream>
#include "Node.h"

namespace GraphLib
{

#ifdef GRAPH_ENABLE_DEBUG
	size_t Node::instanceCount = 0;
#endif

	Node::Node()
	{
#ifdef GRAPH_ENABLE_DEBUG
		std::cout << "Creating an instance of Node\n";
		instanceCount++;
#endif
		rank = -1;
	}

	Node::~Node()
	{
#ifdef GRAPH_ENABLE_DEBUG
		std::cout << "Removing an instance of Node\n";
		instanceCount--;
#endif
	}

	double Node::getRank()
	{
		return rank;
	}

	void Node::setRank(double rank)
	{
		this->rank = rank;
	}

	std::size_t& Node::getIndex()
	{
		return this->index;
	}

	void Node::setIndex(std::size_t index)
	{
		this->index = index;
	}
}