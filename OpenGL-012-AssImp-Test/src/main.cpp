/*
 * main.cpp
 *
 *  Created on: 16-Apr-2018
 *      Author: naveen
 */

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;

void processNodes(int level,aiNode* node,const aiScene* scene);
int main(int argc, char** argv){
	string path = "src/models/cat.obj";
	Assimp::Importer importer;
	const aiScene* scene= importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode){
		cout << "Failed to load the file" << endl;
		return EXIT_FAILURE;
	}

	aiNode* rootNode = scene->mRootNode;
	processNodes(0,rootNode,scene);

	return EXIT_SUCCESS;
}

void processNodes(int level,aiNode* node,const aiScene* scene){
	int noOfMeshes = node->mNumMeshes;
	int noOfChildren = node->mNumChildren;
	cout <<"Number of Meshes at level " << level << " are " << noOfMeshes << endl;
	for(int i=0;i<noOfMeshes;i++){
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		cout <<"Mesh vertices count: "<< mesh->mNumVertices << endl;
		cout <<"Indices Count: " << mesh->mNumFaces << endl;
		cout <<"Material Count: " << mesh->mMaterialIndex << endl;
		cout <<"Texture count: " << mesh->mTextureCoords << endl;
	}
	cout <<"Number of children at level " << level  << " are " << noOfChildren << endl;
	for(int i=0;i<noOfChildren;i++){
		processNodes(level + 1, node->mChildren[i],scene);
	}
}

