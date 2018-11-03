#include "Utils.h"

glm::vec3 didey::draw::hexToRGB(int hex) {
	return glm::vec3(((hex >> 16) & 0xFF) / 255.0f, ((hex >> 8) & 0xFF) / 255.0f, ((hex) & 0xFF) / 255.0f);
}