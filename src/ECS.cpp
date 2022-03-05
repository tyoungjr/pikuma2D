#include "ECS/ECS.h"
#include "Logger/Logger.h"

int IComponent::nextId = 0; 

int Entity::GetId() const {
	return id; 
}

void System::AddEntityToSystem(Entity entity) {
	entities.push_back(entity);
} 

void System::RemoveEntityFromSystem(Entity entity) {

	// fun way to erase an element from a vector based on equality overload
	// the comparison operator checks if the ids are equal 
	entities.erase(std::remove_if(entities.begin(), entities.end(),
    					[&entity](Entity other){ 
    						return entity == other; 
    					}), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const {
	return entities; 
}

const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Entity Registry::CreateEntity() {

	int entityId;
	entityId = numEntities++;
	Entity entity(entityId); 
	entity.registry = this; 
	entitiesToBeAdded.insert(entity);

	if(entityId >= static_cast<int>(entityComponentSignatures.size())) {
		entityComponentSignatures.resize(entityId + 1);
	}
	
	Logger::Log("Entity created with id = " + std::to_string(entityId));
	
	return entity; 
}

// Adds an entity that has the required components to the system 
void Registry::AddEntityToSystems(Entity entity) {
	const auto entityId = entity.GetId(); 

	const auto entityComponentSignature = entityComponentSignatures[entityId];
	
	for(auto& system: systems) {
		const auto& systemComponentSignature = system.second->GetComponentSignature();
		
		// bitwise and on both bitsets to test if the bits of one is in the other 
		bool isInterested = (entityComponentSignature & 
							 systemComponentSignature) == systemComponentSignature; 
		
		if (isInterested) {
			// Add entity to the system 
			system.second->AddEntityToSystem(entity);
		} 
	}

}

// Add the entities that are waiting to be create to the active Systems
// Remove the entities that are waiting to be killed from the active systems 
void Registry::Update() {
	for (auto entity: entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}

	entitiesToBeAdded.clear();


}