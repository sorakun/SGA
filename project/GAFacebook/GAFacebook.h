
#ifndef GAFACEBOOK_H
#define GAFACEBOOK_H

#include <fstream>
#include <iostream>
#include "Graph.h"
#include "User.h"

namespace GAFacebook{
	
	typedef struct Feature{
		wchar_t *name;
		std::size_t value;
	};

	class FacebookGraph: public GraphLib::Graph
	{
	private:
		wchar_t *combinedfile, *featuresfile;

	public:
		FacebookGraph(wchar_t *combinedfile, wchar_t *featuresfile)
		{
			this->combinedfile = combinedfile;
			this->featuresfile = featuresfile;
		}

		~FacebookGraph()
		{
			delete combinedfile;
			delete featuresfile;
		}

		std::size_t& createNode()
		{
			User* u = new User();
			addNode(u);
			return u->getIndex();
		}
		
		void run()
		{
			std::ifstream input(combinedfile);
			std::size_t n1, n2;

			std::size_t count = 0;

			std::string line;

			std::size_t nodesCount = 4039;

			std::cout << "Parsing combined file\n";

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
			std::cout << "Done parsing combined files\n\n";
			std::cout << "\n\n Number of Nodes: " << this->getNodesCount()<< "\n";
			std::cout << "\n\n Number of Edges: " << this->getEdgesCount()<< "\n";

			std::cout << "Parsing features file\n";
		}
	};
}

#endif