//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//         with help from Jordan Rutty
//
// Copyright (c) 2014-2019 Concordia University. All rights reserved.
//

#include "ParticleSystem.h"
#include "ParticleDescriptor.h"
#include "ParticleEmitter.h"
#include "EventManager.h"
#include "World.h"

#define WIN32_LEAN_AND_MEAN	//for debug
#include <Windows.h>	//for debug
#include "math.h"
#include <string>
#include <string.h>
#include "string"
#include "string.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>

using namespace glm;


ParticleSystem::ParticleSystem(ParticleEmitter* emitter, ParticleDescriptor* descriptor)
: mpDescriptor(descriptor), mpEmitter(emitter), timeSinceLastParticleEmitted(0.0f)
{
    assert(mpDescriptor != nullptr);
    assert(mpEmitter != nullptr);
    
    // Pre-allocate the maximum number of particles at a give time, according to
    // lifetime and emission rate
    int maxParticles = static_cast<int>(mpDescriptor->emissionRate * (mpDescriptor->totalLifetime + mpDescriptor->totalLifetimeRandomness)) + 1;
    
    mInactiveParticles.resize(maxParticles);
    for (std::list<Particle*>::iterator it = mInactiveParticles.begin(); it != mInactiveParticles.end(); ++it)
    {
        *it = new Particle();
    }
}

ParticleSystem::~ParticleSystem()
{
	for (std::list<Particle*>::iterator it = mParticleList.begin(); it != mParticleList.end(); ++it)
	{
        World::GetInstance()->RemoveBillboard(&(*it)->billboard);
		delete *it;
	}

    for (std::list<Particle*>::iterator it = mInactiveParticles.begin(); it != mInactiveParticles.end(); ++it)
    {
        delete *it;
    }
    
    mParticleList.resize(0);
	mInactiveParticles.resize(0);
}

void ParticleSystem::Update(float dt)
{
    // Emit particle according to the emission rate
    float averageTimeBetweenEmission = 1.0f / mpDescriptor->emissionRate;
//    float randomValue = EventManager::GetRandomFloat(0.0f, averageTimeBetweenEmission);
    timeSinceLastParticleEmitted += dt;
    
    if (timeSinceLastParticleEmitted > averageTimeBetweenEmission && mInactiveParticles.size() > 0)
    {
        timeSinceLastParticleEmitted = 0.0f;
        
        
        // emit particle
        // transfer a particle from the inactive pool to the active pool
        Particle* newParticle = mInactiveParticles.back();
        mParticleList.push_back(newParticle);
        mInactiveParticles.pop_back();
        World::GetInstance()->AddBillboard(&newParticle->billboard);
        
        // Set particle initial parameters
        newParticle->billboard.position = mpEmitter->GetPosition();
        newParticle->billboard.size = mpDescriptor->initialSize + EventManager::GetRandomFloat(-1.0f, 1.0f) * mpDescriptor->initialSizeRandomness;
        newParticle->billboard.color = mpDescriptor->initialColor;
        newParticle->currentTime = 0.0f;
        newParticle->lifeTime = mpDescriptor->totalLifetime + mpDescriptor->totalLifetimeRandomness * EventManager::GetRandomFloat(-1.0f, 1.0f);
        newParticle->velocity = mpDescriptor->velocity;

        newParticle->billboard.angle = mpDescriptor->initialRotationAngle + EventManager::GetRandomFloat(0.0f, mpDescriptor->initialRotationAngleRandomness);
        
        // @TODO 7 - Initial Random Particle Velocity vector
        //
        // Adjust the random velocity according to a random vertical angle variation on a cone
        //
        // Step 1 : You can rotate the velocity vector by a random number between 0 and
        //          mpDescriptor->velocityAngleRandomness.
        // Step 2 : You can rotate the result in step 1 by an random angle from 0 to
        //          360 degrees about the original velocity vector

        // ...

		
		/*
		float angle = EventManager::GetRandomFloat(0, mpDescriptor->velocityAngleRandomness);
		double perpendicularVariation = (tan(angle) * newParticle->velocity.y/14);

		double coneRotationAngle = EventManager::GetRandomFloat(0, 360);
		double newZ = sin(coneRotationAngle) * perpendicularVariation;
		double newX = cos(coneRotationAngle) * perpendicularVariation;
		
		
		newParticle->velocity.z = newZ;
		newParticle->velocity.x = newX;
		*/




		//************ need to do All rotations around unit vectors or else x and z could get massive velocities that acceleration cant counteract

		//mpDescriptor->velocity.x = 10.0f;  //test case
		
		//char buffer[100];
		float magnitude = sqrt(pow(mpDescriptor->velocity.x,2)+ pow(mpDescriptor->velocity.y, 2)+ pow(mpDescriptor->velocity.z, 2));	//find magnitude of velocity
		vec3 velocityVector = mpDescriptor->velocity / magnitude;	//make unit vector from velocity, need to do this or everythign breaks if using regular velocity vector
		vec3 normalLine = cross(velocityVector, vec3(EventManager::GetRandomFloat(0,1.0), EventManager::GetRandomFloat(0, 1.0), EventManager::GetRandomFloat(0, 1.0)));	//find axis perpendicular to velocity by crossing with random vector
		float normalMagnitude = sqrt(pow(normalLine.x, 2) + pow(normalLine.y, 2) + pow(normalLine.z, 2));	//find magnitude of velocity
		//sprintf_s(buffer, "before x: %f y: %f z: %f mag:%f \n", normalLine.x, normalLine.y, normalLine.z, normalMagnitude);
		//OutputDebugStringA(buffer);		
		normalLine /= normalMagnitude;
		//normalMagnitude = sqrt(pow(normalLine.x, 2) + pow(normalLine.y, 2) + pow(normalLine.z, 2));	//find magnitude of velocity
		//sprintf_s(buffer, "after x: %f y: %f z: %f mag:%f \n\n",normalLine.x,normalLine.y,normalLine.z,normalMagnitude);
		//OutputDebugStringA(buffer);

		//Part 1
		float perpendicularAngle = EventManager::GetRandomFloat(0, mpDescriptor->velocityAngleRandomness);	//find random angle 
		glm::mat4 rotationPerpendicular = glm::mat4_cast(angleAxis(radians(perpendicularAngle), normalLine));	//make rotation matrix to rotate around perpendicular axis
		vec3 offsetVector = vec4(newParticle->velocity, 0.0f) * rotationPerpendicular;	//calculate new velocity vector rotated around perpendicular axis

		//Part 2
		float conicalAngle = EventManager::GetRandomFloat(0, 360);	//get random angle
		glm::mat4 rotationConical = glm::mat4_cast(angleAxis(radians(conicalAngle), velocityVector));	//make rotation matrix to rotate around velocity vector in cone shape
		newParticle->velocity = vec4(offsetVector, 0.0f) * rotationConical; //rotate in cone shape
		


		/*
		//Part 1
		float perpendicularAngle = EventManager::GetRandomFloat(0, mpDescriptor->velocityAngleRandomness);
		glm::mat4 rotationPerpendicular = glm::mat4_cast(angleAxis(radians(perpendicularAngle), vec3(0.0f,0.0f,1.0f)));
		vec3 offsetVector = vec4(newParticle->velocity, 0.0f)* rotationPerpendicular;

		//Part 2
		float conicalAngle = EventManager::GetRandomFloat(0, 360);
		glm::mat4 rotationConical = glm::mat4_cast(angleAxis(radians(conicalAngle), vec3(0.0f,1.0f,0.0f)));
		newParticle->velocity = vec4(offsetVector,0.0f) * rotationConical;
		*/

    }
    
    
    for (std::list<Particle*>::iterator it = mParticleList.begin(); it != mParticleList.end(); it++)
    {
        Particle* p = *it;
		p->currentTime += dt;
        p->billboard.position += p->velocity * dt;
        
        // @TODO 6 - Update each particle parameters
        //
        // Update the velocity of the particle from the acceleration in the descriptor
        // Update the size of the particle according to its growth
        // Update The color is also updated in 3 phases
        //
        //
        // Phase 1 - Initial: from t = [0, fadeInTime] - Linearly interpolate between initial color and mid color
        // Phase 2 - Mid:     from t = [fadeInTime, lifeTime - fadeOutTime] - color is mid color
        // Phase 3 - End:     from t = [lifeTime - fadeOutTime, lifeTime]
                
        
        // ...

		p->velocity += dt * mpDescriptor->acceleration;
		p->billboard.size += dt * mpDescriptor->sizeGrowthVelocity;


		if (p->currentTime < mpDescriptor->fadeInTime)
		{
			p->billboard.color = mix(mpDescriptor->initialColor, mpDescriptor->midColor, p->currentTime);
		}
		
		else if (p->currentTime < mpDescriptor->totalLifetime - mpDescriptor->fadeOutTime)
		{
			p->billboard.color = mpDescriptor->midColor;
		}
		else
		{
			p->billboard.color = mix(mpDescriptor->midColor, mpDescriptor->endColor, p->currentTime - mpDescriptor->fadeOutTime);
		}
		
        //p->billboard.color = vec4(1.0f, 1.0f, 1.0f, 1.0f); // wrong... check required implementation above
        
														   
														   
														   // ...
        
        // Do not touch code below...
        
        // Particles are destroyed if expired
        // Move from the particle to inactive list
        // Remove the billboard from the world
        if (p->currentTime > p->lifeTime)
        {
            mInactiveParticles.push_back(*it);
            
            World::GetInstance()->RemoveBillboard(&(p->billboard));
            mParticleList.remove(*it++);
        }
    }
}
