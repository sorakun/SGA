#ifndef GRAPH_TESTSAMPLE
#define GRAPH_TESTSAMPLE

#include <fstream>
#include <iostream>
#include "Graph.h"

namespace GraphLib
{

	class GraphTestSample: public Graph{
		const wchar_t* filePath;

	public:
		GraphTestSample(wchar_t *filePath): Graph()
		{
			this->filePath = filePath;
		}

		void run()
		{
			std::ifstream input(filePath);
			std::size_t n1, n2;

			std::size_t count = 0;

			std::string line;

			std::size_t nodesCount = 4039;

			for(std::size_t i = 0; i < nodesCount; i++)
				createNode();

			while(std::getline(input, line))
			{
				std::istringstream istream(line);
				istream >> n1 >> n2;

				//std::cout << "linking " << n1 << ", " << n2 << '\n';
				link(n1, n2);
			}
			
			input.close();

			std::cout << "\n\n Number of Nodes: " << this->getNodesCount()<< "\n";
			std::cout << "\n\n Number of Edges: " << this->getEdgesCount()<< "\n";
			std::cout << "\n\tCalculating AdjMatrix...\n";
			calculateAjdMatrix();
			//calculatePageRankMatrix();
			//printPageRank();
		}
	};
}

#endif