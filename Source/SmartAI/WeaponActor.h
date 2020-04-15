#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponActor.generated.h"

// this holds the properties for the weapon and elevate method      
UCLASS( ClassGroup = (TankParts), meta = (BlueprintSpawnableComponent) )
class BATTLETANKS_API UWeaponActor : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	void Elevate(float ElevatePosition);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MaxDegreesPerSecond = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MaxDegreesElevation = 40.0f; 

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MinDegreesElevation = -20.0f; 
};
