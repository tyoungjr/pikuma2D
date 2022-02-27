#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>

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
	int GetId() const;

};

/// <summary>
/// System 
/// The system processes entities that contain a specific signature 
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
	Signature& GetComponentSignature() const;

	// Defines the component type entities must have to be considered by the system 
	template <typename T> void RequireComponent();

};

class Registry {
	// TODO:... 
};

template <typename T>
void System::RequireComponent() {
	const auto componentId = Component<T>::GetId(); 

	componentSignature.set(componentId);
}
#endif