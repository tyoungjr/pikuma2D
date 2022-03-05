#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "ECS/ECS.h"
#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"

class MovementSystem: public System {
public:
	MovementSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void Update() {

		for (auto entity : GetSystemEntities()) {
			// Update entity position based on its velocity
			auto& transform = entity.GetComponent<TransformComponent>();
			const auto rigidBody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidBody.velocity.x;
			transform.position.y += rigidBody.velocity.y;

			Logger::Log(
				"Entity id = " + 
				std::to_string(entity.GetId()) + 
				" position is now (" +
				std::to_string(transform.position.x) + 
				", " +
				std::to_string(transform.position.y) + ")"
			);
		}
	}
};

#endif