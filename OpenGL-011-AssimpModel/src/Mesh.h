/*
 * Mesh.h
 *
 *  Created on: 26-Apr-2018
 *      Author: naveen
 */

#ifndef MESH_H_
#define MESH_H_
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <imageloader/stb_image.h>
#include "Shader.h"
namespace naveen {
struct Texture{
	GLuint textureId;
	std::string path;
	std::string typeName;
};
struct Model{
	GLuint vao;
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<naveen::Texture> textures;
};
class Mesh {
private:
	std::vector<Model> models;
	std::string directory;
	void processNode(aiNode* node,const aiScene* scene);
	Model processMesh(aiMesh* mesh,const aiScene* scene);
	void processMaterial(Model &model,aiMaterial* material,aiTextureType type,std::string typeName);
	void loadTextureFromFile(Texture& texture,std::string path);
	void setupMesh();
public:
	Mesh();
	virtual ~Mesh();
	bool loadModel(std::string path);
	void drawMesh(naveen::Shader* shader);
};

} /* namespace naveen */

#endif /* MESH_H_ */
