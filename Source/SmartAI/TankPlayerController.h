#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TankPlayerController.generated.h"

class UTankAimingComponent;

UCLASS()
class BATTLETANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;		
	virtual void Tick(float DeltaTime) override;

	
protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
		void FoundAimingComponent(UTankAimingComponent* AimingComponent); 

private:
	UPROPERTY(EditDefaultsOnly)
		float CrossHairLocationX = 0.5;

	UPROPERTY(EditDefaultsOnly)
		float CrossHairLocationY = 0.33333;

	UPROPERTY(EditDefaultsOnly)
		float LineTraceRange = 1000000.0;

	void CorrectAimDirection();

	bool CollectSightRayHitLocation(FVector& OutHitLocation) const; 
	bool CollectTargetingInf(FVector2D ScreenLocation, FVector& LookDirection) const;				//Understanding the Look direction of the player 
	bool ObserveCollisionLocation(FVector CamLookDirection, FVector& HitLocationPoint) const;		//Understand the look direction to start line tracing 
	
	virtual void SetPawn(APawn* InPawn) override;

	// delegate listener 
	UFUNCTION()
		void OnPossesedTankDeath();	
};
