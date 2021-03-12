#include "Mesh.h"

#include <gl_core_4_4.h>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::InitialiseQuad()
{
	// Check that mesh has not been initialised already
	assert(vao == 0);

	// Generate the buffers 
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Binds the vertex array; a mesh wrapper
	glBindVertexArray(vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Define the 6 vertices for the 2 triangles that make the quad
	Vertex vertices[6];

	vertices[0].position = { -0.5f, 0.0f,  0.5f, 1.0f };
	vertices[1].position = {  0.5f, 0.0f,  0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.0f, -0.5f, 1.0f };
	vertices[3].position = { -0.5f, 0.0f, -0.5f, 1.0f };
	vertices[4].position = {  0.5f, 0.0f,  0.5f, 1.0f };
	vertices[5].position = {  0.5f, 0.0f, -0.5f, 1.0f };

	vertices[0].normal = { 0, 1, 0, 0 }; 
	vertices[1].normal = { 0, 1, 0, 0 }; 
	vertices[2].normal = { 0, 1, 0, 0 }; 
	vertices[3].normal = { 0, 1, 0, 0 }; 
	vertices[4].normal = { 0, 1, 0, 0 }; 
	vertices[5].normal = { 0, 1, 0, 0 };

	vertices[0].texCoord = { 0, 1 }; // Bottom left
	vertices[1].texCoord = { 1, 1 }; // Bottom right
	vertices[2].texCoord = { 0, 0 }; // Top left
	vertices[3].texCoord = { 0, 0 }; // Top left
	vertices[4].texCoord = { 1, 1 }; // Bottom right
	vertices[5].texCoord = { 1, 0 }; // Top right

	// Fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Enable the first element as a position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Enable the second element as a normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// Enable the third element as a TexCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// Unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Quad has two triangles
	triCount = 2;
}

void Mesh::Initialise(unsigned int a_vertexCount, const Vertex* a_vertices, unsigned int a_indexCount, unsigned int* a_indices)
{
	// Check that mesh has not been initialised already
	assert(vao == 0);

	// Generate the buffers 
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Binds the vertex array; a mesh wrapper
	glBindVertexArray(vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, a_vertexCount * sizeof(Vertex), a_vertices, GL_STATIC_DRAW);
	
	// Enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Bind the indices if there are any
	if (a_indexCount != 0)
	{
		glGenBuffers(1, &ibo);

		// Bind the Vertex Buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// Fill the Vertex Buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_indexCount * sizeof(unsigned int), a_indices, GL_STATIC_DRAW);

		triCount = a_indexCount / 3;
	}
	else
	{
		triCount = a_vertexCount / 3;
	}

	// Unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Draw()
{
	glBindVertexArray(vao);

	// Check if we are using indices or just vertices
	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}
}
