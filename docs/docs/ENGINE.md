# Engine

## 1. Using the ECS
Now that your project compiles, it is time to add some entities to it. This quick start tutorial assumes that you are already familiar with what an [ECS](https://en.wikipedia.org/wiki/Entity_component_system) is.
- Create/destroy an Entity using ``Registry::CreateEntity()``, ``Registry::DestroyEntity()``.
- Add/remove components to it using ``Registry::EmplaceComponent()`` ``Registry::RemoveComponent()``.
- Add a system using ``Registry::AddSystem()``. Once registered you cannot remove a system, but you can enable or disable it at anytime using ``System::enable``.
- You will find all the basic components and systems in the ``MGE::Commons`` namespace, but you can also create your own!

As an exemple, let's create a simplistic ``Rocket`` system.

### 2. Create your own component
For the sake of the tutorial, let's make a rocket component.
In real life, every rocket share a common component: an __engine__. Let's start from there.
Creating a component in MGE require a little more than just having a struct like in most ECS, because MGE's replication system relies on __reflection__.

```
// RocketComponent.hpp

struct RocketComponent {
    REFLECT_STRUCT(VehicleComponent) // REFLECT_STRUCT is a macro that contains boilerplate code for the MGE's reflection system. 

    const float maxSpeed = 100; // The max speed of an engine doesn't change once it is set so it can be const.
    float throttle = 0;         // The throttle of the engine, from 0 to 1. 0 meaning the engine is off, 1 meaning the engine is at max speed.
} 
```
```
// RocketComponent.cpp

void RocketComponent::SetupReflection() { // This static method is defined by the REFLECT_STRUCT macro, and allows us to customize how our component is seen by the reflection system.
    Class.RegisterProperty(maxSpeed);             // Here we can register any property that we want the reflection system to be aware of.
    Class.RegisterProperty(throttle, REPLICATED); // We want all clients to be able to know the throttle of the rocket to show appropriate animations, so we tell the reflection system that this property is to be replicated.
} 
```
We also need a way to know the position in the world of our rocket, but the ``TransformComponent`` is here for that purpose, so let's use this.

### 3. Creating a system
Now that we have a rocket component, we need a way to acctually update it's position in the world. So we need a way to compute the speed of the rocket.
Thankfully, MGE comes with a ``VelocitySystem`` that takes a ``TransformComponent`` and a ``VelocityComponent``, and changes the position of the entity accordingly.
MGE also comes with a ``RigidBodySystem`` that takes a ``RigidBodyComponent`` and a ``VelocityComponent``, and applies physics to the entity, like gravity and modifies the velocity accordingly.
Now that we have a component for the position of the rocket, a component for the velocity of the rocket, a component for its speed data, and a component for the physics, we can create our system.
```
void RocketSystemFunction(MGE::World& world, MGE::Commmon &velocity, RocketComponent& rocket) // First we define our function;
{
    velocity.velocity = Directions3f::Up * rocket.maxSpeed * rocket.throttle;
}

std::shared_ptr<System> RocketSystem = std::make_shared<System>("Rocket System", RocketSystemFunction, System::ClientAndServer);
```

### 4. Creating our entity
```
std::shared_ptr<Entity> rocketEntity = registery.CreateEntity(true); // true because we want our entity to be replicated to the clients.
registry.EmplaceComponent<TransformComponent>(rocketEntity);
registry.EmplaceComponent<VelocityComponent>(rocketEntity);
registry.EmplaceComponent<RocketComponent>(rocketEntity, 100); // We pass in a max speed of 100.
registry.EmplaceComponent<RigidBody>(rocketEntity);
registery.RegisterSystem(RocketSystem);
```
And with that:
- The ``RocketSystem`` will update the speed of the rocket.
- The ``RigidBodySystem`` will update the ``VelocityComponent`` to account for gravity.
- The ``VelocitySystem`` will update the position of the rocket according to the velocity stored in the ``VelocitySystem``.
- By setting ``rocketEntity`` to be replicated, and by setting up the ``RocketComponent`` to replicate its ``throttle`` property, the replication system will automatically update this property on all clients when it is changed on the server. Note that the replication of a property is trigger only once when the property is changed. It is not guaranteed to arrive to all clients. Use it only for properties that changes regularly. Use an RPC for more important and less frequent updates.
