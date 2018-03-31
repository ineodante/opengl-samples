/*
 * ModelLoader.cpp
 *
 *  Created on: 26-Mar-2018
 *      Author: naveen
 */

#include "ModelLoader.h"

ModelLoader::ModelLoader() {
	// TODO Auto-generated constructor stub

}

ModelLoader::~ModelLoader() {
	// TODO Auto-generated destructor stub
}

std::vector<std::string> ModelLoader::split(std::string input, std::string delimiter){
	std::vector<std::string> output;
	int index = 0;
	int inputLength = input.length();
	int foundAt = -1;
	while((foundAt = input.find(delimiter,index)) != -1){
		output.push_back(input.substr(index, foundAt-index));
		index = foundAt + 1;
		if(index > inputLength)
			break;
	}
	if(index < inputLength)
		output.push_back(input.substr(index,inputLength-index));
	return output;
}

bool ModelLoader::loadObjFile(const char* filePath,naveen::Model &obj){
	std::ifstream objFile(filePath,std::ios::in);
	if(!objFile.is_open()){
		return false;
	}
	std::string line;
	while(std::getline(objFile,line)){
		if(line.at(0) != '#' && line.at(0) != 'o'){
			std::vector<std::string> temp = split(line," ");
			if(temp.at(0) == "v"){
				glm::vec3 vertex;
				vertex.x = std::stof(temp.at(1).c_str(),0);
				vertex.y = std::stof(temp.at(2).c_str(),0);
				vertex.z = std::stof(temp.at(3).c_str(),0);
				obj.vertices.push_back(vertex);
			}
			if(temp.at(0) == "f"){
				for(int fIndex = 1 ; fIndex < 4;fIndex++){
					std::vector<std::string> faceIndices = split(temp.at(fIndex),"/");
					obj.indices.push_back(std::stoul(faceIndices.at(0).c_str(),nullptr,0)-1);
				}
			}
		}
	}
	objFile.close();
	return true;
}
