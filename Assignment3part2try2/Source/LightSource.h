#pragma once

#include <glm/glm.hpp>
#include "ParticleSystem.h"
#include "ParsingHelper.h"
#include <vector>

struct lightSourceStruct
{
	glm::vec4 position;
	glm::vec3 color;
	glm::vec3 coefficients;
};

class LightSource
{
public:

	void Load(ci_istringstream& iss);
	bool ParseLine(std::vector<ci_string> token);
	std::vector<lightSourceStruct> lightSources;
};
