#include "LightSource.h"

using namespace glm;
using namespace std;

bool LightSource::ParseLine(vector<ci_string> token)
{
	if (token.empty())
	{
		return true;
	}
	if (token[0].empty() == false && token[0][0] == '#')
	{
		return true;
	}
	else if (token[0] == "position")
	{
		assert(token.size() > 4);
		assert(token[1] == "=");

		lightSources[lightSources.size() - 1].position.x = static_cast<float>(atof(token[2].c_str()));
		lightSources[lightSources.size() - 1].position.y = static_cast<float>(atof(token[3].c_str()));
		lightSources[lightSources.size() - 1].position.z = static_cast<float>(atof(token[4].c_str()));
		lightSources[lightSources.size() - 1].position.w = static_cast<float>(atof(token[4].c_str()));
	}
	else if (token[0] == "color")
	{
		assert(token.size() > 4);
		assert(token[1] == "=");

		lightSources[lightSources.size() - 1].color.x = static_cast<float>(atof(token[2].c_str()));
		lightSources[lightSources.size() - 1].color.y = static_cast<float>(atof(token[3].c_str()));
		lightSources[lightSources.size() - 1].color.z = static_cast<float>(atof(token[4].c_str()));
	}
	else if (token[0] == "coefficients")
	{
		assert(token.size() > 4);
		assert(token[1] == "=");

		lightSources[lightSources.size() - 1].coefficients.x = static_cast<float>(atof(token[2].c_str()));
		lightSources[lightSources.size() - 1].coefficients.y = static_cast<float>(atof(token[3].c_str()));
		lightSources[lightSources.size() - 1].coefficients.z = static_cast<float>(atof(token[4].c_str()));
	}
	else
	{
		fprintf(stderr, "Error loading scene file... token:  %s!", token[0].c_str());
		getchar();
		exit(-1);
	}

	return true;
}

void LightSource::Load(ci_istringstream& iss)
{
	ci_string line;

	lightSourceStruct temp;
	lightSources.push_back(temp);

	// Parse model line by line
	while (std::getline(iss, line))
	{
		// Splitting line into tokens
		ci_istringstream strstr(line);
		istream_iterator<ci_string, char, ci_char_traits> it(strstr);
		istream_iterator<ci_string, char, ci_char_traits> end;
		vector<ci_string> token(it, end);

		if (ParseLine(token) == false)
		{
			fprintf(stderr, "Error loading scene file... token:  %s!", token[0].c_str());
			getchar();
			exit(-1);
		}
	}
}

