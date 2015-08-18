#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include "Graph.h"

namespace GraphLib
{
#ifdef GRAPH_ENABLE_DEBUG
	size_t Graph::instanceCount = 0;
#endif

	double Graph::dampingFactor = 0.85;

	Graph::Graph(bool directed) 
	{
		this->directed = directed;
		adjMatrix = NULL;
		comment = NULL;
#ifdef GRAPH_ENABLE_DEBUG
		std::cout << "Creating an instance of Graph\n";
		instanceCount++;
#endif
	}

	Graph::~Graph()
	{
		for(std::size_t i=0; i<vertexes.size(); i++)
		{
			Node* n = vertexes[i];
			delete n;
		}

		delete adjMatrix;

		vertexes.clear();
		edges.clear();

#ifdef GRAPH_ENABLE_DEBUG
		std::cout << "Removing an instance of Graph\n";
		instanceCount--;
#endif
	}

	Graph::EdgeSet Graph::getEdges()
	{
		return edges;
	}

	/* number of nodes */
	std::size_t Graph::getNodesCount()const
	{
		return vertexes.size();
	}

	/* number of edges */
	std::size_t Graph::getEdgesCount()const
	{
		return edges.size();
	}

	size_t& Graph::createNode()
	{
		Node* n = new Node();
		addNode(n);
		return n->getIndex();
	}

	
	Node* Graph::nodeAt(std::size_t n)
	{
		assert(n<vertexes.size());
		return vertexes[n];
	}

	bool Graph::link(std::size_t n1, std::size_t n2)
	{
		assert((n1<vertexes.size()) && (n2<vertexes.size()));
		assert(n1!=n2);

		Node *node1 = vertexes[n1], *node2 = vertexes[n2];
		Edge *e = NULL;

		e = new Edge(node1, node2);

		if(!addEdge(e))
		{
			delete e;
			return false;
		}
		return true;
	}

	bool Graph::unlink(std::size_t n1, std::size_t n2)
	{
		assert(n1<vertexes.size());
		assert(n2<vertexes.size());
		assert(n1!=n2);

		Node *node1 = vertexes[n1], *node2 = vertexes[n2];

		Edge *e = NULL;

		e = new Edge(vertexes[n1], vertexes[n2]);

		EdgeSet::const_iterator it = edges.find(e);

		//std::cout << (it==edges.end()) << " X\n";

		if(it == edges.end())
			return false;

		edges.erase(e);
		delete e;

		return true;
	}

	bool Graph::areLinked(std::size_t n1, std::size_t n2)
	{
		assert(n1<vertexes.size());
		assert(n2<vertexes.size());
		assert(n1!=n2);

		Edge *e = NULL;

		e = new Edge(vertexes[n1], vertexes[n2]);

		EdgeSet::const_iterator it = edges.find(e);
		if(it == edges.end())
			return false;

		return true;


	}

	void Graph::removeNode(size_t ref)
	{
		assert(ref<vertexes.size());
		Node *n = vertexes[ref];
		removeNode(n);
	}

	/* Add a node to the graph, used internally */
	bool Graph::addNode(Node *node)
	{
		node->setIndex(vertexes.size());
		for( std::size_t i = 0; i < vertexes.size(); i++)
			if (node == vertexes[i])
				return false;

		vertexes.push_back(node);
		return true;
	}

	/* Removes a node from the graph, used internally  */
	void Graph::removeNode(Node *node)
	{
		std::size_t size = vertexes.size();
		std::size_t pos = node->getIndex();

		vertexes.erase(vertexes.begin()+pos);

		std::vector<Edge*> toRemove;

		for(auto e: edges)
		{
			if(e->contains(node))
				toRemove.push_back(e);
		}

		std::size_t s = toRemove.size();

		for(std::size_t i=0; i<s; i++)
		{
			Edge *e  = toRemove[i];
			edges.erase(e);
			delete e;
		}

		/* Update other nodes indexes */
		for(std::size_t i=pos; i<size-1; i++)
			vertexes[i]->setIndex(i);

		node->setIndex(-1);
		delete node;
	}

	bool Graph::addEdge(Edge *edge)
	{
		EdgeSet::const_iterator e = edges.find(edge);

		if(e != edges.end())
			return false;

		std::size_t start = edge->start->getIndex();
		std::size_t end   = edge->end->getIndex();

		if (start == -1 || end == -1)
			return false;

		edges.insert(edge);
		return true;
	}

	bool Graph::removeEdge(Edge *edge)
	{
		EdgeSet::const_iterator e = edges.find(edge);
		if(e == edges.end())
			return false;

		edges.erase(edge);
		return true;
	}

	void Graph::calculateAjdMatrix()
	{
		std::size_t SIZE = vertexes.size();

		adjMatrix = new MatrixXb(SIZE, SIZE);

		for(std::size_t i = 0; i < SIZE; i++)
		{
			for(std::size_t j = 0; j < SIZE; j++)
			{
				(*adjMatrix)(i, j) = false;
			}
		}

		for(const auto& e: edges)
		{
			std::size_t i1 = e->start->getIndex();
			std::size_t i2 = e->end->getIndex();
			(*adjMatrix)(i1, i2) = true;
			if(!this->directed)
				(*adjMatrix)(i2, i1) = true;
		}
	}

	void Graph::printAdjMatrix()
	{
		assert(adjMatrix != NULL);

		auto adjMat = *adjMatrix;

		std::size_t SIZE = vertexes.size();
		for( std::size_t i = 0; i < SIZE; i++)
		{
			for(std::size_t j = 0; j < SIZE; j++)
			{
				std::cout << "\t" << adjMat(i, j);
			}
			std::cout << "\n";
		}
	}

	bool Graph::get(std::size_t n1, std::size_t n2)
	{
		assert(adjMatrix != NULL);
		assert(n1<vertexes.size());
		assert(n2<vertexes.size());

		return (*adjMatrix)(n1, n2);
	}

	/* Initializes every node with a rank value 1/numberOfNodes */

	void Graph::calculatePageRank()
	{
		assert(adjMatrix != NULL);

		auto adjMat = *adjMatrix;

		/* initial value */
		double ival = 1.f/vertexes.size();


		/* This vector will contain the number of edges in which each node participates */
		std::vector <std::size_t> nodeEdgesCount;

		/* Step 0 (Optimisation) */
		for(std::size_t i = 0; i < vertexes.size(); i++)
		{
			nodeEdgesCount.push_back(0);

			for(std::size_t j = 0; j < vertexes.size(); j++)
			{
				if (adjMat(i, j)){
					nodeEdgesCount[i] ++;
				}
			}
		}

		/* Step 1 Initialize all nodes PR to 1/numberOfNodes */
		for(std::size_t i = 0; i < vertexes.size(); i++)
		{
			vertexes[i]->setRank(ival);
		}


		/* Step 2 Loop 100x to get a good page rank value. */
		for(std::size_t k = 0; k <100; k++)
		{
			/* Calculate PR of all nodes */
			for(std::size_t i = 0; i < vertexes.size(); i++)
			{

				double sum = 0;

				/* calculate the sum of RP(Ti)/L(Ti) where Ti links to the current node */
				for(std::size_t j = 0; j < vertexes.size(); j++)
				{
					/* node i must be targeted not the source */
					if (adjMat(j, i))
					{
						sum += vertexes[j]->getRank()/nodeEdgesCount[j];
					}
				}
				vertexes[i]->setRank( (1-dampingFactor) + dampingFactor * sum );
			}

		}
	}

	void Graph::calculatePageRankMatrix()
	{
		assert(adjMatrix != NULL);

		auto adjMat = *adjMatrix;

		std::vector<double> R;
		std::size_t SIZE = vertexes.size();
		Eigen::MatrixXd M(SIZE, SIZE);

		/* This vector will contain the number of edges in which each node participates */
		std::vector <std::size_t> nodeEdgesCount;

		/* Step 0 (Optimisation) */
		for(std::size_t i = 0; i < SIZE; i++)
		{
			nodeEdgesCount.push_back(0);

			for(std::size_t j = 0; j < SIZE; j++)
			{
				if (adjMat(i, j)){
					nodeEdgesCount[i] ++;
				}
			}
		}

		/* Creating M matrix */
		for(std::size_t i = 0; i < SIZE; i++)
		{
			for(std::size_t j = 0; j < SIZE; j++)
			{
				M(i, j) = 0;
				if(adjMat(j,i))
					M(i, j) = (1.0f/nodeEdgesCount[j]);
			}
		}

		/* Creating R matrix */
		for(std::size_t i = 0; i < SIZE; i++)
		{
			R.push_back(1./SIZE);
		}

		for(std::size_t k = 0; k < 100; k++)
		{
			std::vector<double> tmp(SIZE);

			for(std::size_t i = 0; i < SIZE; i++)
			{
				double value = 0;
				for(std::size_t j = 0; j < SIZE; j++)
				{
					value += M(i, j) * R[j];
				}
				tmp[i] = value *dampingFactor +(1.0f-dampingFactor);
			}
			R = tmp;
		}


		for(std::size_t i = 0; i < R.size(); i++)
		{
			vertexes[i]->setRank(R[i]);
		}

	}

	double Graph::getRank(std::size_t n)
	{
		assert(n<vertexes.size());
		double rank = vertexes[n]->getRank();
		/* To make sure this is called after calculating PR */
		assert(rank >= 0);
		return rank;
	}

	void Graph::printPageRank() const
	{
		double sum = 0;
		for( std::size_t i = 0; i < vertexes.size(); i++)
		{
			std::cout << "\tNode " << i << ":\t\t" << vertexes[i]->getRank() << "\n";
			sum += vertexes[i]->getRank();
		}
		std::cout << "\n\n\tTotal PR: " << sum << "\n\n";
	}
	
	void Graph::saveGML(wchar_t *url)
	{
		std::cout << "\n\tSaving GML to \""<< url << "\"...\n";
		std::ofstream out;
		out.open(url);
		out << "graph [";

		if(comment != NULL)
			out << "\tcomment \"" << comment << "\"\n";
		out << "\tdirected 0\n";


		for(std::size_t i = 0; i < vertexes.size(); i++)
		{
			out << "\tnode [";
			out << "\t\tid " << i << '\n';
			out << "\t\tlabel \"" << i << "\"\n";
			out << "\t]\n";
		}
		
		for(auto e: edges)
		{
			out << "\tedge [";
			out << "\t\tsource " << e->start->getIndex() << '\n';
			out << "\t\ttarget " << e->end->getIndex() << '\n';
			out << "\t]\n";
		}

		out << "]";

		out.close();
	}

	void Graph::saveDOT(wchar_t *url)
	{
		std::cout << "\n\tSaving DOT to \""<< url << "\"...\n";
		std::ofstream out;
		out.open(url);
		out << "graph g{\n";
		for(auto e: edges)
		{
			out << "\t" << e->start->getIndex() << " -- " << e->end->getIndex() << ";\n";
		}

		out << "}";

		out.close();
	}

	void Graph::saveCSV(wchar_t *url)
	{
		std::cout << "saving file\n";
		std::ofstream out;
		out.open(url);
		out << "X, ";
		for(std::size_t i = 0; i < vertexes.size(); i++)
		{
			out << "Node " << i << ',';
		}

		for(std::size_t i = 0; i < vertexes.size(); i++)
		{
			out << "\nNode " << i << ',';
			for(std::size_t j = 0; j < vertexes.size(); j++)
			{
				out << (*adjMatrix)(i, j) << ',';
			}
		}
		out << "\n\n";

		double sum = 0;
		for( std::size_t i = 0; i < vertexes.size(); i++)
		{
			out << "Node " << i << ',' << vertexes[i]->getRank() << '\n';
			sum += vertexes[i]->getRank();
		}

		out << "\n\nTotal PR," << sum << "\n\n";

		out.close();
	}

}