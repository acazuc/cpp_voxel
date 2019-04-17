#include "ParticlesManager.h"
#include "World/World.h"
#include "Main.h"

namespace voxel
{

	ParticlesManager::ParticlesManager(Chunk &chunk)
	: chunk(chunk)
	, initialized(false)
	{
	}

	ParticlesManager::~ParticlesManager()
	{
		for (size_t i = 0; i < this->particles.size(); ++i)
			delete (this->particles[i]);
	}

	void ParticlesManager::tick()
	{
		for (size_t i = 0; i < this->particles.size(); ++i)
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
			this->positionBuffer = new VertexBuffer();
			this->indiceBuffer = new VertexBuffer();
			this->colorBuffer = new VertexBuffer();
			this->uvBuffer = new VertexBuffer();
			this->initialized = true;
		}
		this->positions.clear();
		this->indices.clear();
		this->colors.clear();
		this->uvs.clear();
		Mat4 playerMat = Mat4::rotateZ(Mat4(1), -this->chunk.getWorld().getPlayer().getRot().z / 180. * M_PI);
		playerMat = Mat4::rotateY(playerMat, -this->chunk.getWorld().getPlayer().getRot().y / 180. * M_PI);
		playerMat = Mat4::rotateX(playerMat, -this->chunk.getWorld().getPlayer().getRot().x / 180. * M_PI);
		for (size_t i = 0; i < this->particles.size(); ++i)
			this->particles[i]->draw(positions, colors, uvs, indices, playerMat);
		this->positionBuffer->setData(GL_ARRAY_BUFFER, positions.data(), positions.size() * sizeof(*positions.data()), GL_DYNAMIC_DRAW);
		this->indiceBuffer->setData(GL_ELEMENT_ARRAY_BUFFER, indices.data(), indices.size() * sizeof(*indices.data()), GL_DYNAMIC_DRAW);
		this->colorBuffer->setData(GL_ARRAY_BUFFER, colors.data(), colors.size() * sizeof(*colors.data()), GL_DYNAMIC_DRAW);
		this->uvBuffer->setData(GL_ARRAY_BUFFER, uvs.data(), uvs.size() * sizeof(*uvs.data()), GL_DYNAMIC_DRAW);
		Main::getParticlesShader().program->use();
		Mat4 model(1);
		Mat4 mvp = this->chunk.getWorld().getPlayer().getProjMat() * this->chunk.getWorld().getPlayer().getViewMat() * model;
		Main::getParticlesShader().vLocation.setMat4f(this->chunk.getWorld().getPlayer().getViewMat());
		Main::getParticlesShader().mvpLocation.setMat4f(mvp);
		Main::getParticlesShader().mLocation.setMat4f(model);
		Main::getParticlesShader().vertexPositionLocation.setVertexBuffer(*this->positionBuffer, 3, GL_FLOAT);
		Main::getParticlesShader().vertexColorLocation.setVertexBuffer(*this->colorBuffer, 3, GL_FLOAT);
		Main::getParticlesShader().vertexUVLocation.setVertexBuffer(*this->uvBuffer, 2, GL_FLOAT);
		this->indiceBuffer->bind(GL_ELEMENT_ARRAY_BUFFER);
		Main::getTerrain()->bind();
		glDisable(GL_CULL_FACE);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glEnable(GL_CULL_FACE);
	}

	void ParticlesManager::addParticle(Particle *particle)
	{
		this->particles.push_back(particle);
		this->positions.reserve(this->particles.size() * 4);
		this->indices.reserve(this->particles.size() * 6);
		this->colors.reserve(this->particles.size() * 4);
		this->uvs.reserve(this->particles.size() * 4);
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
