#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponActor.h" 
#include "TankHead.h" 
#include "Projectile.h" 
#define OUT

// setting the values for the properties of this component
UTankAimingComponent::UTankAimingComponent()
{
	// setting the component to initialise and tick every frame 
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the project begins
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);  

	//this allows the computer to check whether the player is able to fire, depending on the seconds passed since the last shot
	if (MyAmmoCount <= 0)
	{
		MyFiringState = EFiringStatus::OutOfAmmo;
	}
	else if ((GetWorld()->GetTimeSeconds() - ShotDuration) < ReloadTimeInSeconds)
	{
		MyFiringState = EFiringStatus::Reloading;
	}
	else if (IsHeadRotating())
	{
		MyFiringState = EFiringStatus::Aiming;
	}
	else
	{
		MyFiringState = EFiringStatus::Locked;
	}
}

void UTankAimingComponent::TurnAndAimAt(FVector TargetLocation)
{
	if (!ensure(MyWeaponActor != nullptr)) { return; }   
	auto StartLocation = MyTankHead->GetComponentLocation();

	FVector TossVelocity(0);

	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,			// 'this' is the TankAimingComponent.cpp class 
		OUT TossVelocity,
		StartLocation,
		TargetLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	
	if (bHaveAimSolution)
	{
		//by using get safe normal function, the toss velocity's value is normalised 
		FVector AimDirection = TossVelocity.GetSafeNormal();

		// the barrel is moved towards the aiming direction  
		WeaponControl(AimDirection);
		RotateTankHead(AimDirection);

		CurrentAimDirection = AimDirection;
	}
}

void UTankAimingComponent::InitialiseAimComponent(UWeaponActor* WeaponActor, UTankHead* TankHead)
{
	MyWeaponActor = WeaponActor;
	MyTankHead = TankHead;
}

void UTankAimingComponent::Fire()
{
	if (ensure(MyWeaponActor != nullptr))
	{
		//if the "tank" had reloaded and it's ready to fire
		if (MyFiringState == EFiringStatus::Aiming || MyFiringState == EFiringStatus::Locked)
		{
			//this generates the projectile       
			auto* MyProjectile = GetWorld()->SpawnActor<AProjectile>(
				TankProjectileType,
				MyWeaponActor->GetSocketLocation(FName("ProjectileSpawn")),
				MyWeaponActor->GetSocketRotation(FName("ProjectileSpawn"))
				);

			// moging the projectile 
			if (ensure(MyProjectile))
			{
				MyProjectile->CastDirection(LaunchSpeed);
			}
			ShotDuration = GetWorld()->GetTimeSeconds();
			MyAmmoCount -= 1;
		}
	}
}

void UTankAimingComponent::WeaponControl(FVector AimDirection)
{
	if (!ensure(MyWeaponActor != nullptr)) { return; }

	auto CurrentBarrelRotation = MyWeaponActor->GetForwardVector().Rotation();
	auto TargetRotation = AimDirection.Rotation();
	auto RoationDifference = TargetRotation - CurrentBarrelRotation;
	
	MyWeaponActor->Elevate(RoationDifference.Pitch);
}

void UTankAimingComponent::RotateTankHead(FVector AimDirection)
{
	if (!ensure(MyTankHead != nullptr)) { return; } 

	auto CurrentTurretRotation = MyTankHead->GetForwardVector().Rotation();
	auto TargetRotation = AimDirection.Rotation();
	auto RoationDifference = TargetRotation - CurrentTurretRotation;

	float FixedRotationDifference = 0; 
	
	if (FMath::Abs(RoationDifference.Yaw) < 180)
	{
		MyTankHead->WeaponHeadRotation(RoationDifference.Yaw);
	}
	else
	{
		MyTankHead->WeaponHeadRotation(-RoationDifference.Yaw);
	}
}

bool UTankAimingComponent::IsHeadRotating()
{
	if (!ensure(MyWeaponActor != nullptr)) { return false; } 

	auto BarrelForward = MyWeaponActor->GetForwardVector();

	return (!BarrelForward.Equals(CurrentAimDirection, 0.1f));
}

EFiringStatus UTankAimingComponent::GetFiringStatus() const
{
	return MyFiringState;
}
