#include "ChunkManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <future>

float renderDistance = 128;

didey::chunk::ChunkManager::ChunkManager(int shaderID) {
	m_shaderID = shaderID;
	positionQueue.reserve(64);
}

didey::chunk::ChunkManager::~ChunkManager() {

}

void didey::chunk::ChunkManager::queueValidChunkPositions(glm::vec3& position, GLFWwindow * window) {
	for (int x = position.x - renderDistance; x < position.x + renderDistance; x++) {
		for (int z = position.z - renderDistance; z < position.z + renderDistance; z++) {
			if (x % CHUNK_SIZE == 0 && z % CHUNK_SIZE == 0) {
				glm::vec3 pos = glm::vec3(x, 0, z);
				if (std::find(positions.begin(), positions.end(), pos) == positions.end()) {
					positions.push_back(pos);
					positionQueue.push_back(pos);
				}
			}
		}
	}
}

void didey::chunk::ChunkManager::addChunkFromQueue() {
	if (positionQueue.size() > 0) {
		Chunk * chunk = new Chunk(positionQueue.at(0));
		std::async(std::launch::async, std::ref(*chunk));
		chunks.push_back(chunk);
		positionQueue.erase(positionQueue.begin());
	}
}

void didey::chunk::ChunkManager::renderActiveChunks(glm::vec3& cameraPosition) {
	for (Chunk * chunk : chunks) {
		if (glm::distance(chunk->getCenter(), cameraPosition) > 100.0f) continue;
		glm::mat4 model;
		model = glm::translate(model, chunk->getPosition());
		glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		chunk->render();
	}

}