#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#include "Defines.h"
#include "Locator.h"

#include "PlayerController.h"
#include "GameObject.h"
#include "VectorExtension.h"

PlayerController::PlayerController(GameObject* parent) : Controller(parent) {
	velocity = sf::Vector2f(0, 0);
	maxSpeed = 1800.f;
	friction = 0.01;
}

void PlayerController::simulate(sf::Time deltaTime, GameState& state)
{
    sf::Vector2f lastVelocity(velocity);

    velocity += getInputVector() * state.getPlayerAcceleration() * deltaTime.asSeconds();;

    //move
    if (velocity != sf::Vector2f(0, 0)) {
        if (std::sqrt(VectorExtension::vectorSquareLength(velocity)) > maxSpeed)
        {
            float angle = std::atan2(velocity.y, velocity.x);
            velocity.x = maxSpeed * std::cos(angle);
            velocity.y = maxSpeed * std::sin(angle);
        }
        
        velocity *= std::powf(friction, deltaTime.asSeconds());

        
        if (std::abs(velocity.x) < std::abs(lastVelocity.x) && std::abs(velocity.x) < 5.f) velocity.x = 0.f;
        if (std::abs(velocity.y) < std::abs(lastVelocity.y) && std::abs(velocity.y) < 5.f) velocity.y = 0.f;

        clipToBoundingBox();
        parentObject->move(velocity * deltaTime.asSeconds());
    }
       
    rotateToMouse(deltaTime);
}

sf::Vector2f PlayerController::getInputVector()
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        movement.y -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movement.y += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement.x -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement.x += 1;
    }

    if (movement == sf::Vector2f(0, 0)) {
        return sf::Vector2f(0, 0);
    }
    
    return VectorExtension::normalize(movement);
}

void PlayerController::rotateToMouse(sf::Time deltaTime)
{
    sf::RenderWindow& window = Locator::getSceneManager().getGameWindow();
    sf::Vector2i mouseWindowPosition = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosition = window.mapPixelToCoords(mouseWindowPosition);

    float targetAngle = VectorExtension::getAngle(mousePosition - parentObject->getPosition());
    float deltaAngle = targetAngle - parentObject->getRotation();

    if (deltaAngle > 180.f) deltaAngle -= 360.f;
    if (deltaAngle < -180.f) deltaAngle += 360.f;

    float maxRotation = 240 * deltaTime.asSeconds();

    if (std::abs(deltaAngle) < maxRotation) {
        parentObject->setRotation(targetAngle);
    } else {
        if (deltaAngle > 0) {
            parentObject->rotate(maxRotation);
        } else {
            parentObject->rotate(-maxRotation);
        }
    }
}

void PlayerController::clipToBoundingBox()
{
    sf::Vector2f movePosition = parentObject->getPosition() + velocity;

    movePosition.x = std::max(10.f, movePosition.x);
    movePosition.x = std::min(static_cast<float>(WINDOW_SIZE) - 10, movePosition.x);
    movePosition.y = std::max(10.f, movePosition.y);
    movePosition.y = std::min(static_cast<float>(WINDOW_SIZE) - 10, movePosition.y);

    velocity = movePosition - parentObject->getPosition();
}
