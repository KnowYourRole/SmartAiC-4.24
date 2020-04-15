#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;

UCLASS()
class BATTLETANKS_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// this limits the radius of the AI tank to its target (how close the ai can be to the player)
	UPROPERTY(EditDefaultsOnly, Category = "Tank Setup") 
		float AcceptanceRadius = 2500;
	 
	virtual void SetPawn(APawn* InPawn) override;

	// delegate listener method, used to execute when its called 
	UFUNCTION()
		void OnPossessedTankDeath(); // delegate
};
