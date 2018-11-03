#include <vector>
#include "Chunk.h"
#include <fmt/format.h>
#include <utility>
#include <GLFW/glfw3.h>
#define FMT_HEADER_ONLY

namespace didey {
	namespace chunk {

		class ChunkManager {
		public:
			ChunkManager::ChunkManager(int shaderID);
			ChunkManager::~ChunkManager();

			std::vector<Chunk*> chunks;
			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> positionQueue;
			
			void addChunkFromQueue();
			void queueValidChunkPositions(glm::vec3&, GLFWwindow*);
			void renderActiveChunks(glm::vec3&);

			int getShader() { return m_shaderID; }

		private:
			int m_shaderID;
			
		};

	}
}