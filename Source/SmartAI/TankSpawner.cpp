#include "TankSpawner.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Engine/TargetPoint.h"

ATankSpawner::ATankSpawner()
{
 	// called every frame 
	PrimaryActorTick.bCanEverTick = false;
}

// Called once the project begins 
void ATankSpawner::BeginPlay()
{
	Super::BeginPlay();

	// randomises the seed 
	FMath::GetRandSeed();

	GetWorldTimerManager().SetTimer(BeginTimer,this, &ATankSpawner::SpawnFunction, 3.0f, false, 3.0f);
}

void ATankSpawner::SpawnFunction()
{
	if (!TankSpawned) { return; }

	// get the spawn points 
	TArray<AActor*> SpawnPoint_Array;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), SpawnPoint_Array);
	if (SpawnPoint_Array.Num() < 1) { return; }

	// this enables the random spawn point selection from array 
	int32 SPointsCount = SpawnPoint_Array.Num();
	int32 SelectedPoint = FMath::RandRange(0, SPointsCount - 1);
	auto SpawnLocation = SpawnPoint_Array[SelectedPoint]->GetActorLocation();

	// generate the tanks 
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor<ATank>(TankSpawned, SpawnLocation, SpawnLocation.Rotation(), SpawnParams);

	//the spawn time for the next time is reduced 
	if (SpawnInterval > 3.0f)
	{
		SpawnInterval -= 1.0f;
	}

	// the timer is restarted 
	GetWorldTimerManager().SetTimer(TimeBetweenSpawn, this, &ATankSpawner::SpawnFunction, SpawnInterval, false, SpawnInterval);
}

