//
// Created by kyllian on 1/18/25.
//

#include "ComponentTekken.hpp"

void ComponentRType::AttackSystem(MGE::World& world, MGE::EntityRef& entity, AttackComponent& attackComponent, MGE::Common::TransformComponent& transformComponent)
{
    if (attackComponent.isAttacking) {
        attackComponent.attackProgress += world.GetFDeltaSeconds();
        
        if (attackComponent.attackProgress >= 0.5f) {
            attackComponent.isAttacking = false;
            attackComponent.attackProgress = 0.0f;
        }

    }

    if (attackComponent.attackCooldown > 0.0f) {
        attackComponent.attackCooldown -= world.GetFDeltaSeconds();
        if (attackComponent.attackCooldown < 0.0f)
            attackComponent.attackCooldown = 0.0f;
    }
}

AttackComponent::AttackComponent(bool isAttacking, float attackProgress, float attackCooldown, int id)
    : isAttacking(isAttacking), attackProgress(attackProgress), attackCooldown(attackCooldown), id(id) {}

void AttackComponent::SetupReflection()
{
    Class.RegisterProperty(id);
}
