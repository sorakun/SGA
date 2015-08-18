
#ifndef GAFACEBOOK_USER_H
#define GAFACEBOOK_USER_H

#include <vector>
#include "Node.h"

namespace GAFacebook
{
	class User: public GraphLib::Node
	{
	public:
		User()
		{
			fVect = new std::vector<std::size_t*>();
			
			for(std::size_t i = 0; i < featuresCount; i++)
			{
				fVect->push_back(nullptr);
			}
		}

		/* feature vector 
		 * A null value in the feature vecture means that feature is absent 
		 */

		std::vector<std::size_t*> *fVect;

		/* number of features */
		static std::size_t featuresCount;
	};
}

#endif