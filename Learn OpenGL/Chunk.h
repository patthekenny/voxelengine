#pragma once

#include <vector>
#include "Block.h"
#include <glad/glad.h>
#include <fmt/format.h>
#define FMT_HEADER_ONLY

namespace didey {
	namespace chunk {
		
		static const int CHUNK_SIZE = 16;

		class Chunk {

		public:
			Chunk::Chunk(glm::vec3 position);
			Chunk::~Chunk();

			GLuint VAO = -1, VBO = -1;
			
			void operator () () {
				this->fillVertices();
			}

			void render();
			void update();

			void fillVertices();
			void createMesh();

			std::vector<Vertex> * vertices;

			int vertexCount;
			
			glm::vec3 getCenter() { return glm::vec3(m_position.x + (CHUNK_SIZE / 2), m_position.y + (CHUNK_SIZE / 2), m_position.z + (CHUNK_SIZE / 2)); }

			int getShader() { return m_shaderID; }

			friend bool operator==(Chunk c1, Chunk c2);

		private:
			static int m_shaderID;
			bool m_shouldUpdate = false;
			glm::vec3 m_position;
			Block * m_blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
			void createBlocks();
			void generateVertices(const float x, const float y, const float z, const glm::vec3 &color, std::vector<Vertex> &vertices);
		};

	}
}