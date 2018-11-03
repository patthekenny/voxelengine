#pragma once
#include <glm/glm.hpp>

static const int BLOCK_SIZE = 32;

#pragma pack(push, 1)
struct Vertex {
	glm::vec3 vertex;
	glm::vec3 color;
};
#pragma pack(pop)

enum BlockType {
	BLOCK_DIRT = 0x1FC45E
};

struct Block {
	Block(glm::vec3 position, BlockType blockType) {
		m_position = position;
		m_blockType = blockType;
	}
	glm::vec3 m_position;
	BlockType m_blockType;
};