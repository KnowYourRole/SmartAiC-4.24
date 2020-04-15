#include "TankTrack.h"
#include "Components/SceneComponent.h"
#include "SprungWheel.h"				
#include "SpawnPoint.h"	
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Tank.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called once the project begins 
void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

}

TArray<class ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> SprungWheelsArray;					

	TArray<USceneComponent*> ChildrenArray;
	GetChildrenComponents(true, ChildrenArray);							
	
	// loop by getting the children array 
	for (auto i : ChildrenArray)
	{
		// cast to correct points 
		auto ChildSpawnPoint = Cast<USpawnPoint>(i);
		if (!ChildSpawnPoint) continue;										
		
		// get spawn actor method is called 
		auto ChildSprungWheel = ChildSpawnPoint->GetGeneratedActor();
		if (!ChildSprungWheel) continue;			
		
		// the results are casted to the correct type 
		auto SprungWheel = Cast<ASprungWheel>(ChildSprungWheel);

		// the sprung wheel is added to the final array 
		SprungWheelsArray.Add(SprungWheel);
	}
	return SprungWheelsArray;
}

void UTankTrack::SetThrottle(float Amount)
{
	auto Throttle = FMath::Clamp<float>(Amount, -1,1);
	DriveTrack(Throttle);
}

void UTankTrack::DriveTrack(float Throttle)
{
	auto ForceApplied = (Throttle * TrackMaxDrivingForce);
	auto ForceDeltaTime = ForceApplied / (GetWorld()->GetDeltaSeconds());
	auto FinalForceApplied = ForceDeltaTime;

	if (GetWorld()->GetDeltaSeconds() > 0.0332f)					//30fps
	{
		FinalForceApplied = ForceDeltaTime * 1.5;
	}
	else if (GetWorld()->GetDeltaSeconds() > 0.0165f)				//60fps
	{
		FinalForceApplied = ForceDeltaTime * 1.25;
	}

	auto AllWheels = GetWheels();	
	
	if (AllWheels.Num() != 0)													
	{
		auto ForcePerWheel = FinalForceApplied / AllWheels.Num();					// drives the tank by the force of the wheels 
		
		for (ASprungWheel* Wheel : AllWheels)								
		{
			Wheel->AddingSpeed(ForcePerWheel);							
		}
		
		RotateWheels();
		Cast<ATank>(GetOwner())->CounterSliding();
	}
}

void UTankTrack::CombineWheels()
{
	auto MySpawnSpring = GetChildComponent(1);
	if (!MySpawnSpring) { return; }

	TArray<USceneComponent*> ChildrenComps;
	MySpawnSpring->GetChildrenComponents(true, ChildrenComps);
	if (ChildrenComps.Num() <= 0) { return; }

	AActor* MyOwner = ChildrenComps[0]->GetOwner();
	TArray<USphereComponent*> SpringChildrenComps;
	MyOwner->GetComponents(SpringChildrenComps);
	if (SpringChildrenComps.Num() <= 0) { return; }

	MyMainWheel = Cast<USphereComponent>(SpringChildrenComps[1]);

	TArray<USceneComponent*> TrackChildren;
	GetChildrenComponents(false, TrackChildren);
	if (TrackChildren.Num() <= 0) { return; }

	for (auto i : TrackChildren)
	{
		auto Item = Cast<UStaticMeshComponent>(i);
		if (Item)
		{
			MeshWheels.Add(Item);
		}
	}
}

void UTankTrack::RotateWheels()
{
	if (!MyMainWheel) 
	{ 
		CombineWheels();
	}

	if (!MyMainWheel) { return; }
	
	auto NewRotation = MyMainWheel->RelativeRotation;

	for (auto i : MeshWheels)
	{
		i->RelativeRotation.Pitch = NewRotation.Roll;
	}
}


