#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

// classes 
class UTankBarrel;
class UTankTurret;
class AProjectile;
class UTankTrack;
class UParticleSystemComponent;

// Declaring a delegate function
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANKS_API ATank : public APawn
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaTime) override;
	
	// getting health as a percentage 
	UFUNCTION(BlueprintPure, Category = "Health")	
		float GetHealthPercent();

	UFUNCTION(BlueprintPure, Category = "Health")	
		bool HasDied();

	//Declaring a delegate function
	FTankDelegate OnDeath;

	void CounterSliding();

protected:			
	// called once the project is started 
	virtual void BeginPlay() override;

private:
	ATank();

	bool BeginIsDead = false;

	UPROPERTY(VisibleAnywhere)
		UTankTrack* LeftTankTrack = nullptr;              

	UPROPERTY(VisibleAnywhere)
		UTankTrack* RightTankTrack = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tank Setup")
		int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Tank Setup")
		int32 MyHealth = 0;  // this is set in the begin play 

	// this virtual float is called once the actor is damaged 
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FTimerHandle Timer_RemoveOnDead;
	void RemoveOnDead();
};
