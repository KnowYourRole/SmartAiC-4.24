#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringStatus : uint8
{
	Locked,
	Aiming,
	Reloading,
	OutOfAmmo
};

//classes
class UWeaponActor;
class UTankHead;
class AProjectile;

UCLASS( ClassGroup = (TankParts), meta=(BlueprintSpawnableComponent) )
class BATTLETANKS_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTankAimingComponent(); 

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void TurnAndAimAt(FVector TargetLocation);
	
	EFiringStatus GetFiringStatus() const;

	UFUNCTION(BlueprintCallable, Category="Setup")
		void InitialiseAimComponent(UWeaponActor* WeaponActor, UTankHead* TankHead);

	UFUNCTION(BlueprintCallable, Category = "TankSetup") 
		void Fire();

protected:
	// Called once the project begins 
	virtual void BeginPlay() override;

	// setting the ammo count 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		int32 MyAmmoCount = 20;

	//setting the firing status var to be called by the UI  
	UPROPERTY(BlueprintReadOnly, Category = "Setup")  
		EFiringStatus MyFiringState = EFiringStatus::Reloading;

private:
	UWeaponActor* MyWeaponActor = nullptr;
	UTankHead* MyTankHead = nullptr;
	FVector CurrentAimDirection = FVector(0,0,0);
	float ShotDuration = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float LaunchSpeed = 8000; // launch speed of the projectile 
	
	UPROPERTY(EditAnywhere, Category = "Firing") 
		float ReloadTimeInSeconds = 3; // reload time is set to 3 seconds 
	
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		TSubclassOf<AProjectile> TankProjectileType;	

	void WeaponControl(FVector AimDirection);			//Improved allows weapon control
	void RotateTankHead(FVector AimDirection);		//Improved allows player to retate tank head
	bool IsHeadRotating();
};
