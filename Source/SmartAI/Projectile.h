
#pragma once

class UTankProjectileMovementComponent;
class UParticleSystemComponent;
class UStaticMeshComponent;
class URadialForceComponent;

//Includes only the header file
#include "Projectile.generated.h"

UCLASS()
class BATTLETANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	//Setting actor's properties
	AProjectile();

protected:
	//when start the game
	virtual void BeginPlay() override;

public:	
	//Calls every frame
	virtual void Tick	//setting this tick as dissabled, so that it can be called only when needed (Primary actor tick must be true to enable it)
	(float DeltaTime) 
	override;

	//This public void selects bullet direction
	void CastDirection(float Speed);

private:
	// Variables store projectile data including mesh, particle objects and floats
	UTankProjectileMovementComponent* ProjMoveComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BulletMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		UParticleSystemComponent* TriggerShooting = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		UParticleSystemComponent* ShotEffect = nullptr;
	
	UPROPERTY(VisibleAnywhere)
		URadialForceComponent* PullBackStrength = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
		float OnDeadDelay = 5.0f;
	
	UPROPERTY(EditDefaultsOnly)
		float TotalDmg = 50.0f;
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);  // Used for the delegate function "OnHit()"  

	//create a function to destroy the projectile
	void OnTimer01Expire();
};
