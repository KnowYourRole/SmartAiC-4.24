#include "SpawnPoint.h"
#include "Engine/World.h"	
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Default values for the component properties are set
USpawnPoint::USpawnPoint()
{
	// This is the way to set the component to initialise once the project begins and be able to be ticked every frame
	PrimaryComponentTick.bCanEverTick = false;
}

// Called once the project begins
void USpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	//Attach root component to the actor, this way a new actor is created
	SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, GetComponentTransform(),GetOwner());

	if (SpawnedActor)																						//Protection of the pointer  
	{
		SpawnedActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);				//Attaching the root component of the spawned actor 
		UGameplayStatics::FinishSpawningActor(SpawnedActor, GetComponentTransform());
	}
}


// Called every frame
void USpawnPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* USpawnPoint::GetGeneratedActor() const
{
	if (SpawnedActor) // protection of the pointer 
	{
		return SpawnedActor;
	}

	return nullptr;
}