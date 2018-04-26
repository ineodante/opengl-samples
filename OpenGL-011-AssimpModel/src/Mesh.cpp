/*
 * Mesh.cpp
 *
 *  Created on: 26-Apr-2018
 *      Author: naveen
 */

#include "Mesh.h"

namespace naveen {

Mesh::Mesh() {
	// TODO Auto-generated constructor stub

}

Mesh::~Mesh() {
	// TODO Auto-generated destructor stub
}

bool Mesh::loadModel(std::string path){
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(),aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes);
	if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode){
		std::cout <<"Failed to open the model file" << std::endl;
		return false;
	}
	directory = path.substr(0, path.find_last_of("/")).append("/");
	processNode(scene->mRootNode,scene);
	setupMesh();
	return true;
}

void Mesh::processNode(aiNode* node,const aiScene* scene){
	for(unsigned int i=0;i<node->mNumMeshes;i++){
		this->models.push_back(this->processMesh(scene->mMeshes[node->mMeshes[i]], scene));
	}
	for(unsigned int i=0;i<node->mNumChildren;i++){
		this->processNode(node->mChildren[i], scene);
	}
}

naveen::Model Mesh::processMesh(aiMesh* mesh,const aiScene* scene){
	naveen::Model model;
	for(unsigned int i=0;i<mesh->mNumVertices;i++){
		glm::vec3 vertex(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
		glm::vec3 normal(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
		model.vertices.push_back(vertex);
		model.normals.push_back(normal);
		if(mesh->mTextureCoords[0]){
			glm::vec2 texCoord;
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			model.texCoords.push_back(texCoord);
		}

	}
	if(mesh->mMaterialIndex >= 0){
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		processMaterial(model,material,aiTextureType_DIFFUSE,"diffuse_texture");
		processMaterial(model,material,aiTextureType_SPECULAR,"specular_texture");
		processMaterial(model,material,aiTextureType_HEIGHT,"normal_texture");
	}
	for(unsigned int i=0;i<mesh->mNumFaces;i++){
		aiFace face = mesh->mFaces[i];
		for(unsigned int j=0;j<face.mNumIndices;j++){
			model.indices.push_back(face.mIndices[j]);
		}
	}
	return model;
}

void Mesh::processMaterial(Model &model,aiMaterial* material,aiTextureType type,std::string typeName){
	for(unsigned int i=0;i<material->GetTextureCount(type);i++){
		aiString str;
		material->GetTexture(type, i, &str);
		std::string path = directory + str.C_Str();
		bool textureFound = false;
		for(unsigned int j=0;j<model.textures.size();j++){
			if(std::strcmp(model.textures[j].path.data(), path.c_str()) == 0){
				textureFound = true;
				std::cout <<"Texture found" << std::endl;
				break;
			}
		}
		if(!textureFound){
			naveen::Texture texture;
			loadTextureFromFile(texture,path);
			texture.typeName = typeName;
			model.textures.push_back(texture);
		}
	}
}

void Mesh::loadTextureFromFile(Texture& texture,std::string path){
	texture.path = path;

	int width,height,nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width,&height,&nrChannels, 0);

	if(data){
		GLint format;
		if(nrChannels == 1){
			format = GL_RED;
		}else if(nrChannels == 3){
			format = GL_RGB;
		}else if(nrChannels == 4){
			format = GL_RGBA;
		}
		glGenTextures(1, &texture.textureId);
		glBindTexture(GL_TEXTURE_2D, texture.textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	stbi_image_free(data);
}

void Mesh::setupMesh(){
	for(unsigned int i=0;i<this->models.size();i++){
		glGenVertexArrays(1,&models[i].vao);
		glBindVertexArray(models[i].vao);
		GLuint vbo;
		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*models[i].vertices.size(),&models[i].vertices[0],GL_STATIC_DRAW);
		GLuint ebo;
		glGenBuffers(1,&ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int) * models[i].indices.size(),&models[i].indices[0],GL_STATIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
		glEnableVertexAttribArray(0);

		GLuint normalBuffer;
		glGenBuffers(1,&normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
		glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)* models[i].normals.size(),&models[i].normals[0],GL_STATIC_DRAW);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
		glEnableVertexAttribArray(1);

		GLuint textureBuffer;
		glGenBuffers(1,&textureBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,textureBuffer);
		glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec2)* models[i].texCoords.size(), &models[i].texCoords[0],GL_STATIC_DRAW);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(GL_FLOAT),(void*)0);
		glEnableVertexAttribArray(2);

	}
}

void Mesh::drawMesh(Shader* shader){
	shader->useShader();
	for(unsigned int i=0;i<this->models.size();i++){
		for(unsigned int j=0;j <this->models[i].textures.size();j++){
			if(this->models[i].textures[j].typeName == "diffuse_texture"){
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, this->models[i].textures[j].textureId);
			}

			if(this->models[i].textures[j].typeName == "specular_texture"){
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, this->models[i].textures[j].textureId);
			}


		}

		glBindVertexArray(models[i].vao);
		glDrawElements(GL_TRIANGLES,models[i].indices.size(),GL_UNSIGNED_INT,0);
		glBindVertexArray(0);

	}
}
} /* namespace naveen */
