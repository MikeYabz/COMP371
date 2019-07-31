//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//         with help from Jordan Rutty
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "ParticleEmitter.h"
#include "Model.h"

#include "Renderer.h"

using namespace glm;

ParticleEmitter::ParticleEmitter(glm::vec3 position, const Model* parent)
: mpParent(parent), mPosition(position)
{
}


// This is a point emitter, nothing is random
// As a small extra task, you could create derived classes
// for more interesting emitters such as line emitters or circular emitters
// In these cases, you would sample a random point on these shapes
glm::vec3 ParticleEmitter::GetPosition()
{
    // @TODO 8 - Position from Parented Emitter
    //
    // Return the position where the particle is emitted.
    // If the emitter is parented, the position is relative to its parent
	glm::vec3 position = vec3(0.0f,0.0f,0.0f);
	//position = mpParent->GetPosition();

	//position = vec3(mpParent->GetWorldMatrix()[3][0], mpParent->GetWorldMatrix()[3][1], mpParent->GetWorldMatrix()[3][2]);

	if (mpParent != nullptr)
	{
		position.x = mpParent->GetWorldMatrix()[3][0];
		position.y = mpParent->GetWorldMatrix()[3][1];
		position.z = mpParent->GetWorldMatrix()[3][2];
	}
	//Renderer::GetCurrentShader().;	
	
	
	//gl_PerVertex.
		//gl_Position;

	/*
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	position.x = getWorldTransform[0][0];
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &mat4(1.0f)[0][0]);
	//position.x = WorldMatrixLocation;
	*/
	//if (mpParent->GetName )

	/*
    glm::vec3 position = mPosition;
	glm::vec4 v(mpParent->GetPosition(), 0);
	glm::mat4 worldMatrix = mpParent->GetWorldMatrix();
	position = v * worldMatrix;
	//position = vec3(1.0f, 1.0f, 1.0f);
	*/
    // ...
    
    return position;
}




