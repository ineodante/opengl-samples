/*
 * ModelLoader.h
 *
 *  Created on: 26-Mar-2018
 *      Author: naveen
 */

#ifndef MODELLOADER_H_
#define MODELLOADER_H_
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

using namespace glm;
namespace naveen{
	struct Model {
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> indices;
	};
}
class ModelLoader {

	private:
		std::vector<std::string> split(std::string input, std::string delimiter);
	public:
		ModelLoader();
		virtual ~ModelLoader();
		bool loadObjFile(const char* filePath,naveen::Model &obj);


};
#endif /* MODELLOADER_H_ */
