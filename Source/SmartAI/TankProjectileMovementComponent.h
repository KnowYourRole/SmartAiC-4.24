#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TankProjectileMovementComponent.generated.h"


UCLASS(ClassGroup = (TankParts), meta = (BlueprintSpawnableComponent))
class BATTLETANKS_API UTankProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
};
