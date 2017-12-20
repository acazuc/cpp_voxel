#include "ParticlesManager.h"
#include "World/World.h"
#include "Main.h"
#include <glm/gtc/matrix_transform.hpp>

namespace voxel
{

	ParticlesManager::ParticlesManager(Chunk &chunk)
	: chunk(chunk)
	, initialized(false)
	{
		//Empty
	}

	ParticlesManager::~ParticlesManager()
	{
		for (uint32_t i = 0; i < this->particles.size(); ++i)
			delete (this->particles[i]);
	}

	void ParticlesManager::tick()
	{
		for (uint32_t i = 0; i < this->particles.size(); ++i)
		{
			this->particles[i]->tick();
			if (this->particles[i]->isDeleted())
			{
				delete (this->particles[i]);
				this->particles.erase(this->particles.begin() + i);
				i--;
			}
		}
	}

	void ParticlesManager::draw()
	{
		if (!this->particles.size())
			return;
		if (!this->initialized)
		{
			this->texCoordsBuffer = new VertexBuffer();
			this->vertexesBuffer = new VertexBuffer();
			this->indicesBuffer = new VertexBuffer();
			this->colorsBuffer = new VertexBuffer();
			this->initialized = true;
		}
		this->texCoords.clear();
		this->vertexes.clear();
		this->indices.clear();
		this->colors.clear();
		glm::mat4 playerMat(1);
		playerMat = glm::rotate(playerMat, glm::vec2(-this->chunk.getWorld().getPlayer().getRot().z / 180. * M_PI, 0).x, glm::vec3(0, 0, 1));
		playerMat = glm::rotate(playerMat, glm::vec2(-this->chunk.getWorld().getPlayer().getRot().y / 180. * M_PI, 0).x, glm::vec3(0, 1, 0));
		playerMat = glm::rotate(playerMat, glm::vec2(-this->chunk.getWorld().getPlayer().getRot().x / 180. * M_PI, 0).x, glm::vec3(1, 0, 0));
		for (uint32_t i = 0; i < this->particles.size(); ++i)
			this->particles[i]->draw(vertexes, colors, texCoords, indices, playerMat);
		this->texCoordsBuffer->setData(GL_ARRAY_BUFFER, texCoords.data(), texCoords.size() * sizeof(glm::vec2), GL_FLOAT, 2, GL_DYNAMIC_DRAW);
		this->vertexesBuffer->setData(GL_ARRAY_BUFFER, vertexes.data(), vertexes.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		this->indicesBuffer->setData(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 1, GL_DYNAMIC_DRAW);
		this->colorsBuffer->setData(GL_ARRAY_BUFFER, colors.data(), colors.size() * sizeof(glm::vec3), GL_FLOAT, 3, GL_DYNAMIC_DRAW);
		Main::getParticlesShader().program->use();
		glm::mat4 mvp = this->chunk.getWorld().getPlayer().getProjMat() * this->chunk.getWorld().getPlayer().getViewMat();
		Main::getParticlesShader().vLocation->setMat4f(this->chunk.getWorld().getPlayer().getViewMat());
		Main::getParticlesShader().mvpLocation->setMat4f(mvp);
		glm::mat4 model(1);
		Main::getParticlesShader().mLocation->setMat4f(model);
		Main::getParticlesShader().texCoordsLocation->setVertexBuffer(*this->texCoordsBuffer);
		Main::getParticlesShader().vertexesLocation->setVertexBuffer(*this->vertexesBuffer);
		Main::getParticlesShader().colorsLocation->setVertexBuffer(*this->colorsBuffer);
		this->indicesBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
		Main::getTerrain()->bind();
		glDisable(GL_CULL_FACE);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		glEnable(GL_CULL_FACE);
	}

	void ParticlesManager::addParticle(Particle *particle)
	{
		this->particles.push_back(particle);
		this->texCoords.reserve(this->particles.size() * 4);
		this->vertexes.reserve(this->particles.size() * 4);
		this->indices.reserve(this->particles.size() * 6);
		this->colors.reserve(this->particles.size() * 4);
	}

	void ParticlesManager::removeParticle(Particle *particle)
	{
		for (std::vector<Particle*>::iterator iter = this->particles.begin(); iter != this->particles.end(); ++iter)
		{
			if (*iter != particle)
				continue;
			this->particles.erase(iter);
			return;
		}
	}

}
