#include "TankPlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TankAimingComponent.h"
#include "Tank.h"

#define OUT

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!GetPawn()) { return; }															
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();	
	if (!ensure(AimingComponent != nullptr)) { return; }
	
	FoundAimingComponent(AimingComponent);
}

// called every frame 
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CorrectAimDirection();
}

void ATankPlayerController::CorrectAimDirection()
{
	if (GetPawn() == nullptr) { return; }

	FVector HitLocation; 
	bool bGotHitLocation = GetSightRayHitLocation(OUT HitLocation);
	if (bGotHitLocation)
	{
		// tank is aimed at the pointed position 
		auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();  
		if (AimingComponent == nullptr) { return; }  

		AimingComponent->TurnAndAimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{

	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(OUT ViewportSizeX, OUT ViewportSizeY); 
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairLocationX, ViewportSizeY * CrossHairLocationY); 

	FVector CamLookDirection;
	if (GetLookDirection(ScreenLocation, OUT CamLookDirection))
	{
		// this checks if we can hit 
		return GetLookVectorHitLocation(CamLookDirection, OutHitLocation);
	}

	return false; 
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation;

	return DeprojectScreenPositionToWorld(	
		ScreenLocation.X,
		ScreenLocation.Y,
		OUT CameraWorldLocation,
		OUT LookDirection
	);

}

bool ATankPlayerController::GetLookVectorHitLocation(FVector CamLookDirection, FVector& HitLocationPoint) const
{
	FVector LineTraceStart = PlayerCameraManager->GetCameraLocation();
	FVector LineTraceEnd = LineTraceStart + (CamLookDirection * LineTraceRange);

	FCollisionQueryParams QueryParams;
	if (GetPawn())
	{
		QueryParams.AddIgnoredActor(GetPawn());
	}

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(
		OUT HitResult, 
		LineTraceStart,
		LineTraceEnd,
		ECollisionChannel::ECC_Camera,				// the collision is set to camera in order to prevent clashes with UI objects 
		QueryParams
	))
	{
		HitLocationPoint = HitResult.Location;
		
		return true;
	}

	return false;
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn); 
	
	if (InPawn) 
	{
		ATank* PossessedTank = Cast<ATank>(InPawn);
		if (!PossessedTank) { return; }

		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossesedTankDeath); 
	}
}

void ATankPlayerController::OnPossesedTankDeath()
{
	StartSpectatingOnly();			//Turns the player to a spectator on death.
}
