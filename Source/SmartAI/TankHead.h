#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankHead.generated.h"

UCLASS(ClassGroup = (TankParts), meta = (BlueprintSpawnableComponent))
class BATTLETANKS_API UTankHead : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:

	void RotateTurret(float RotationPosition);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MaxRotationPerSecond = 25.0; 

};
