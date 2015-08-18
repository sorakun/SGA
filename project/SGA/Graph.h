/*
* -------------------------
* About PageRank Algorithm:
* -------------------------
*
* The PageRank Algorithm
* The original PageRank algorithm was described by Lawrence Page and Sergey Brin in several publications. It is given by
* PR(A) = (1-d) + d (PR(T1)/C(T1) + ... + PR(Tn)/C(Tn))
* where
*      PR(A) is the PageRank of page A,
*      PR(Ti) is the PageRank of pages Ti which link to page A,
*      C(Ti) is the number of outbound links on page Ti and
*      d is a damping factor which can be set between 0 and 1.
*      (read more at http://pr.efactory.de/e-pagerank-algorithm.shtml)
*
* -------------------------
* How does the class works:
* -------------------------
*
* 1. Add as much nodes as you can
* 2. Link nodes (create edges) 
* 3. Calculate rage rank
* 4. Preview Page rank and/or adj matrix
*/

#ifndef GRAPH_H
#define GRAPH_H

#include <functional>
#include <vector>
#include <unordered_set>
#include <string>
#include <Eigen/Dense>

#include "Node.h"
#include "Edge.h"

namespace GraphLib
{
	class Graph
	{
	protected:
		/* A special datatype: A matrix of size_t */  
		typedef Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> MatrixXb;
		typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> MatrixXi;
		bool directed;
		static double dampingFactor;
		static size_t nodeIndexCounter;

		MatrixXb *adjMatrix;

		/* Adjacency matrix */
		MatrixXb *degMatrix;

		/* TODO: Laplacian matrix */
		MatrixXb *lapMatrix;

		std::vector<Node*> vertexes;

		struct hash_Edge
		{
			std::size_t operator()(const Edge* e)const
			{
				std::string s = std::to_string(e->start->getIndex()) + ", " + std::to_string(e->end->getIndex());
				std::hash<std::string> h;
				//std::cout << "from : "<< e->start->getIndex() << " to " << e->end->getIndex() << " hash: " << h(s) << '\n';
				return h(s);
			}
			bool operator() (const Edge *e1, const Edge *e2)
			{
				//std::cout << "comparing two nodes returning " << e1->equals(e2) << '\n';
				return e1->equals(e2);
			}
		};

		typedef std::unordered_set<Edge*, hash_Edge, hash_Edge> EdgeSet;
		EdgeSet edges;

		/* Nodes */
		bool addNode(Node*);
		void removeNode(Node*);

		/* Edges */
		bool addEdge(Edge *);
		bool removeEdge(Edge *);

	public:

		Graph(bool directed = true);
		~Graph();

		wchar_t *comment;

		/* saves a graph in CSV format */
		void saveCSV(wchar_t* url);
		void saveGML(wchar_t *);
		void saveDOT(wchar_t *);
		EdgeSet getEdges();

		/* 
		* The parameters of this functions needs to be same of a node.
		* Returns a reference to the index of the created node in the graph.
		* Can be overloaded.
		*/
		virtual std::size_t& createNode();
		void removeNode(std::size_t);
		Node *nodeAt(std::size_t);

		/*
		* Creates an edge between two nodes. 
		* Returns true if the two nodes are successfully linked, false if there
		* is an issue (one of the node not existing). 
		*/
		bool link(std::size_t, std::size_t);

		/* Removed the edge between two nodes */
		bool unlink(std::size_t, std::size_t);

		/* checks if two nodes are linked or not */
		bool areLinked(std::size_t, std::size_t);

		/* Once all nodes added and linked, we calculate the adjMatrix */
		void calculateAjdMatrix();

		/* Prints adjency matrix */
		void printAdjMatrix();

		/* returns an element of the ajdMatrix */
		bool get(std::size_t, std::size_t);

		/* calculates pagerank */
		void calculatePageRank();

		void calculatePageRankMatrix();

		void printPageRank() const;

		/* returns the rank of a node */
		double getRank(std::size_t);

		/* number of nodes */
		std::size_t getNodesCount()const;

		/* number of edges */
		std::size_t getEdgesCount()const;

#ifdef GRAPH_ENABLE_DEBUG
		static size_t instanceCount;
#endif
	};
}
#endif // GRAPH_H
