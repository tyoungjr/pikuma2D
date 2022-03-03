#ifndef ECS_H
#define ECS_H

#include <memory>
#include <set>
#include <bitset>
#include <typeinfo>
#include <utility>
#include <vector>
#include <typeindex>
#include <unordered_map>

const unsigned int MAX_COMPONENTS = 32;

/// <summary>
/// Signature 
/// We use a bitset of 1s and 0s to keep track of which components an entity hs 
/// and also helps keep track of which entities a system is interested in. 
/// </summary>
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
	static int nextId; 
};

// Used to assign unique ids to a component type 
template <typename T>
class Component: public IComponent {
	
	// Returns the unique id of the Component<T> 
  	static int GetId() {
  		static auto id = nextId++; 
  		return id;
  	}
};

class Entity {
private:
	int id;

public:
	Entity(int id) : id(id) {};
	Entity(const Entity& entity) = default; 
	
	int GetId() const;

	Entity& operator =(const Entity& other) = default; 
	bool operator ==(const Entity& other) const { return id == other.id; } 
	bool operator !=(const Entity& other) const { return id != other.id; }
	bool operator >(const Entity& other) const { return id > other.id; }
	bool operator <(const Entity& other) const { return id < other.id; }

};

/// <summary>
/// System 
/// Processes entities that contain a specific signature 
/// </summary>
class System {
private:
	Signature componentSignature; 
	std::vector<Entity> entities; 

public:
	System() = default; 
	virtual ~System() = default; 
	void AddEntityToSystem(Entity entity); 
	void RemoveEntityFromSystem(Entity entity); 
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	// Defines the component type entities must have to be considered by the system 
	template <typename TComponent> void RequireComponent();
};

/**
 * Pool 
 * Just a vector (contigous data) of objects of Type T
 */
class IPool {
public: 
	virtual ~IPool() {}

};

template <typename T>
class Pool: public IPool {

private:
	std::vector<T> data;

public:
	Pool(int size = 100) {
		data.resize(size); 
	}
	virtual ~Pool() = default; 

	bool isEmpty() const  { return data.empty(); }

	int GetSize() const { return data.size(); }

	void Resize(int n) { data.resize(n); }

	void Clear() { data.clear(); }

	void Add(T object) { data.push_back(object); }

	void Set(int index, T object) { data[index] = object; }

	T& Get(int index) { return static_cast<T&>(data[index]); }

	T& operator [](unsigned int index) { return data[index]; }

}; 

/**
 * Registry
 * Manages the creation and destruction of entities, as well as adding systems 
 * and adding components to entities. 
 */
class Registry {

private:
	// Keep track of how many entities were added to the scene
	int numEntities = 0; 

	// Vector of component pools, contains all the data for a certain component type
	// vector index = component type id 
	// Pool index = entity id. 
	std::vector<IPool*> componentPools;

	// set of Entities that are flagged to be added or removed in the next Update
	std::set<Entity> entitiesToBeAdded;  
	std::set<Entity> entitiesToBeKilled;  
	
	// Vector of component signatures, handles which component is turned "on"
	// for [Vector index = entity id] 
	std::vector<Signature> entityComponentSignatures;

	// map of systems from their respective type_index
	std::unordered_map<std::type_index, System*> systems; 

public:
	Registry() = default; 

	void Update();
	
	// Entity management 
	Entity CreateEntity();
	
	// Component management
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);  
	template <typename TComponent> void RemoveComponent(Entity entity); 
	template <typename TComponent> bool HasComponent(Entity entity) const;

	// TODO:
	//void KillEntity(Entity entity);
	
	// System management
	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template<typename TSystem> void RemoveSystem();
	template<typename TSystem> bool HasSystem() const; 
	template<typename TSystem> TSystem& GetSystem() const;

	// Checks the component signature of an entity and add the entity to the systems 
	// that are interested in it 
	void AddEntityToSystems(Entity entity); 

};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId(); 
	componentSignature.set(componentId);
}

template <typename TSystem, typename ...TArgs> 
void Registry::AddSystem(TArgs&& ...args) {
	TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...)); 
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem> 
void Registry::RemoveSystem() {
	auto system = systems.find(std::type_index(typeid(TSystem))); 
	systems.erase(system); 
}	

template<typename TSystem>
bool Registry::HasSystem() const {
	return systems.find(std::type_index(typeid(TSystem))) != systems.end(); 	
}
template<typename TSystem> onst {
	auto system = systems.find(s
TSystem& Registry::GetSystem() ctd::type_index(typeid(TSystem))); 
	return std::static_pointer_cast<TSystem>(system->second);
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs && ...args) {

	const auto componentId = Component<TComponent>::GetId(); 
	const auto entityId = entity.GetId();

	// ids represent the current numOfComponents in the current pool;
	if (componentId >= componentPools.size()){
		componentPools.resize(componentId + 1, nullptr);
	}

	// if we don't have a component pool for this component. make it
	if (!componentPools[componentId]) { 
		Pool<TComponent>* newComponentPool = new Pool<TComponent>(); 
		componentPools[componentId] = newComponentPool;
	}

	// get the pool ofor 
	Pool<TComponent>* componentPool = componentPools[componentId];

	if(entityId >= componentPool->GetSize()) {
		componentPool->Resize(numEntities); 
	} 

	TComponent newComponent(std::forward<TArgs>(args)...);
	componentPool->Set(entityId, newComponent); 

	entityComponentSignatures[entityId].set(componentId); 
}

template <typename TComponent> 
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	entityComponentSignatures[entityId].set(componentId, false);
}


template <typename TComponent> 
bool Registry::HasComponent(Entity entity) const{
	const auto componentId = Component<TComponent>::GetId(); 
	const auto entityId = entity.GetId(); 

	return entityComponentSignatures[entityId].test(componentId); 
}
#endif