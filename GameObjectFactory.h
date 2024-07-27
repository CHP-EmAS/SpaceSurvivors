#pragma once

#include <memory>

#include "ObjectPool.h"
#include "Observer.h"

#include "Bullet.h";
#include "Asteroid.h";
#include "Explosion.h";
#include "Experience.h";

class GameObjectFactory : public Observer
{
public:
	void prepareObjects();
	void clear();

	std::shared_ptr<Bullet> acquireBullet(sf::Vector2f direction, int damage);
	std::shared_ptr<Asteroid> acquireAsteroid(sf::Vector2f direction, float scale, float speed, float rotationSpeed);
	
	std::shared_ptr<Explosion> acquireExplosion(sf::Time duration, float scale, sf::Color tint = sf::Color::White);
	std::shared_ptr<Experience> acquireExperience(int amount);

private:
	std::shared_ptr<GameObject> getPooledObject(ObjectType type);
	void fillPool(ObjectType type, int initialAmount);

	ObjectPool<Bullet, 150> bulletPool;
	ObjectPool<Asteroid, 100> asteroidPool;

	ObjectPool<Explosion, 10> explosionPool;
	ObjectPool<Experience, 100> experiencePool;

	void onEvent(const Event event) override;
};
