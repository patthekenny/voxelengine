#include "Chunk.h"
#include "Utils.h"

#include <chrono>

#include <thread>
#include <iostream>

didey::chunk::Chunk::Chunk(glm::vec3 position) {
	m_position = position;
	vertices = new std::vector<Vertex>;
	this->createBlocks();
}

didey::chunk::Chunk::~Chunk() {
	
}

void didey::chunk::Chunk::createBlocks() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				m_blocks[x][y][z] = new Block(glm::vec3(x, y, z), BLOCK_DIRT);
			}
		}
	}
}

void didey::chunk::Chunk::fillVertices() {
	if (vertices == nullptr) {
		vertices = new std::vector<Vertex>;
	}

	vertices->reserve(147456);

	int firstTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				Block * block = m_blocks[x][y][z];
				if (block != nullptr) {
					if (x == 15 || x == 0 || y == 15 || y == 0 || z == 15 || z == 0) {
							generateVertices(block->m_position.x, block->m_position.y, block->m_position.z, glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX), *vertices);
					} else if (m_blocks[x - 1][y][z] == nullptr || m_blocks[x + 1][y][z] == nullptr || m_blocks[x][y + 1][z] == nullptr || m_blocks[x][y - 1][z] == nullptr || m_blocks[x][y][z - 1] == nullptr || m_blocks[x][y][z + 1] == nullptr) {
							generateVertices(block->m_position.x, block->m_position.y, block->m_position.z, glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX), *vertices);
							//generateVertices(block->m_position.x, block->m_position.y, block->m_position.z, didey::draw::hexToRGB(block->m_blockType), *vertices);
					}
				}
			}
		}
	}

	int lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//fmt::print("time to gen vertices(ms): {}\n", (lastTime - firstTime));
}

void didey::chunk::Chunk::createMesh() {

	vertexCount = this->vertices->size();

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices->size(), this->vertices->data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, vertex)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
	glEnableVertexAttribArray(1);
	
	delete this->vertices;
}


void didey::chunk::Chunk::update() {
	
	this->fillVertices();
	this->createMesh();

	delete this->vertices;

	this->m_shouldUpdate = false;
}

void didey::chunk::Chunk::render() {
	if (this->m_shouldUpdate) {
		this->update();
	}
	if (this->VAO == -1) {
		this->createMesh();
	}

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
}

void didey::chunk::Chunk::generateVertices(const float x, const float y, const float z, const glm::vec3 &color, std::vector<Vertex> &vertices) {
	const float x0 = x, x1 = x + 1.0f;
	const float y0 = -z, y1 = -z + 1.0f;
	const float z0 = y, z1 = y + 1.0f;
	vertices.insert(vertices.end(), {
		{ { x0, y0, z0 }, color },
		{ { x0, y1, z0 }, color },
		{ { x0, y0, z1 }, color },
		{ { x0, y0, z1 }, color },
		{ { x0, y1, z0 }, color },
		{ { x0, y1, z1 }, color },
		{ { x1, y0, z0 }, color },
		{ { x1, y0, z1 }, color },
		{ { x1, y1, z0 }, color },
		{ { x1, y1, z0 }, color },
		{ { x1, y0, z1 }, color },
		{ { x1, y1, z1 }, color },
		{ { x0, y0, z0 }, color },
		{ { x1, y0, z0 }, color },
		{ { x0, y1, z0 }, color },
		{ { x0, y1, z0 }, color },
		{ { x1, y0, z0 }, color },
		{ { x1, y1, z0 }, color },
		{ { x0, y0, z1 }, color },
		{ { x0, y1, z1 }, color },
		{ { x1, y0, z1 }, color },
		{ { x1, y0, z1 }, color },
		{ { x0, y1, z1 }, color },
		{ { x1, y1, z1 }, color },
		{ { x0, y1, z0 }, color },
		{ { x1, y1, z0 }, color },
		{ { x0, y1, z1 }, color },
		{ { x0, y1, z1 }, color },
		{ { x1, y1, z0 }, color },
		{ { x1, y1, z1 }, color },
		{ { x0, y0, z0 }, color },
		{ { x0, y0, z1 }, color },
		{ { x1, y0, z0 }, color },
		{ { x1, y0, z0 }, color },
		{ { x0, y0, z1 }, color },
		{ { x1, y0, z1 }, color } 
	});
}

bool didey::chunk::operator==(Chunk c1, Chunk c2)
{
	return c1.getCenter() == c2.getCenter();
}
