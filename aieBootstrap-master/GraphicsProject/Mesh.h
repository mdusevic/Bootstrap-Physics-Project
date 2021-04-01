/*
 * File:	Mesh.h
 *
 * Author: Mara Dusevic (s200494@students.aie.edu.au)
 * Date Created: Wednesday 10 March 2021
 * Date Last Modified Thursday 1 April 2021
 *
 * This is file creates the variables used in the corresponding
 * .cpp file that creates meshes for quads.
 *
 */

#pragma once

#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {};
	virtual ~Mesh();

	// Vertex struct
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	// Initialisation functions for different quads
	void InitialiseQuad();
	void InitialiseFullscreenQuad();
	void Initialise(unsigned int a_vertexCount, const Vertex* a_vertices, unsigned int a_indexCount = 0, unsigned int* a_indices = nullptr);

	// Draw function
	virtual void Draw();

protected:
	unsigned int triCount;

	// Vertex Array Object
	unsigned int vao;
	// Vertex Buffer Object
	unsigned int vbo; 
	// Index Buffer Object
	unsigned int ibo;
};