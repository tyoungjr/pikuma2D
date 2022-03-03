#include <ECS\ECS.h>
#include <Logger\Logger.h>

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
	entitiesToBeAdded.insert(entity);

	Logger::Log("Entity created with id = " + std::to_string(entityId));
	
	return entity; 
}

// TODO
// Add the entities that are waiting to be create to the active Systems
// Remove the entities that are waiting to be killed from the active systems 
void Registry::Update() {
	
}