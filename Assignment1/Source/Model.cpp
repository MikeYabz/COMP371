//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "stdio.h"
#include "Model.h"
#include "Animation.h"
#include "World.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace std;
using namespace glm;

Model::Model() 
: mName("UNNAMED"), mPosition(0.0f, 0.0f, 0.0f), mScaling(1.0f, 1.0f, 1.0f), mRotationAxis(0.0f, 1.0f, 0.0f),
  mRotationAngleInDegrees(0.0f), mAnimation(nullptr)
{
}

Model::~Model()
{
}

void Model::Update(float dt)
{
}

void Model::Draw()
{
}


void Model::Load(ci_istringstream& iss)
{
	ci_string line;

	// Parse model line by line
	while(std::getline(iss, line))
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

bool Model::ParseLine(const std::vector<ci_string> &token)
{
    if (token.empty() == false)
	{
		if (token[0].empty() == false && token[0][0] == '#')
		{
			return true;
		}
		else if (token[0] == "name")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			mName = token[2];	
		}
		else if (token[0] == "position")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			mPosition.x = static_cast<float>(atof(token[2].c_str()));
			mPosition.y = static_cast<float>(atof(token[3].c_str()));
			mPosition.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "rotation")
		{
			assert(token.size() > 5);
			assert(token[1] == "=");

			mRotationAxis.x = static_cast<float>(atof(token[2].c_str()));
			mRotationAxis.y = static_cast<float>(atof(token[3].c_str()));
			mRotationAxis.z = static_cast<float>(atof(token[4].c_str()));
			mRotationAngleInDegrees = static_cast<float>(atof(token[5].c_str()));

			glm::normalize(mRotationAxis);
		}
		else if (token[0] == "scaling")
		{
			assert(token.size() > 4);
			assert(token[1] == "=");

			mScaling.x = static_cast<float>(atof(token[2].c_str()));
			mScaling.y = static_cast<float>(atof(token[3].c_str()));
			mScaling.z = static_cast<float>(atof(token[4].c_str()));
		}
		else if (token[0] == "animation")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");

			ci_string animName = token[2];
            
            mAnimation = World::GetInstance()->FindAnimation(animName);
		}
		else
		{
			return false;
		}
	}

	return true;
}

glm::mat4 Model::GetWorldMatrix() const
{
	// @TODO 2 - You must build the world matrix from the position, scaling and rotation information
    //           If the model has an animation, get the world transform from the animation.
	mat4 worldMatrix(1.0f);

	glm::mat4 translatedMat = glm::translate(glm::mat4(1.0f), GetPosition()); // creating translation matrix
	glm::mat4 translationMatrix = translatedMat * worldMatrix;	//calculating translation matrix

	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), GetScaling()); // creating scaling matrix

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), GetRotationAngle(), GetRotationAxis()); // creating rotation matrix

	worldMatrix = translationMatrix * rotationMatrix * scalingMatrix * worldMatrix; //calculating world matrix with the translation, rotation and scaling matrices
    
	if (mAnimation != nullptr)
	{
		return mAnimation->GetAnimationWorldMatrix();// &GetAnimationWorldMatrix();
	}

	else return worldMatrix;
}

void Model::SetPosition(glm::vec3 position)
{
	mPosition = position;
}

void Model::SetScaling(glm::vec3 scaling)
{
	mScaling = scaling;
}

void Model::SetRotation(glm::vec3 axis, float angleDegrees)
{
	mRotationAxis = axis;
	mRotationAngleInDegrees = angleDegrees;
}
