#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprungWheel.generated.h"

class UPhysicsConstraintComponent;
class USphereComponent;

UCLASS()
class BATTLETANKS_API ASprungWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// the default values for the actor's properties is set 
	ASprungWheel();

	virtual void Tick(float DeltaTime) override;

	void AddingSpeed(float ForceMagnitude);

protected:
	// Called when the project begins 
	virtual void BeginPlay() override;

	

private:
	void RestictSpeed();

	float CurrentSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPhysicsConstraintComponent* SpringImprovement = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPhysicsConstraintComponent* AxlePhysicsConstraint = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")	
		USphereComponent* WheelMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* AxleMesh = nullptr;		

	// delegate function is created in order to ensure that the indication is the same as the OnHit component as they will include the same arguments       
	UFUNCTION() 																							
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit); 
};
