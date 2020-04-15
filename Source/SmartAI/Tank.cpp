#include "Tank.h"
#include "TankTrack.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SprungWheel.h"
#include "TimerManager.h"

ATank::ATank()
{
 	// called every frame 
	PrimaryActorTick.bCanEverTick = false; 
}

// called once the project begins 
void ATank::BeginPlay()
{
	Super::BeginPlay();

	// setting the health of the player  
	MyHealth = StartingHealth;
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CounterSliding();
}

// this function is called once the actor receives damage 
float ATank::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 ClampedDamage = FMath::Clamp<int32>(DamageAmount, 0, MyHealth);
	MyHealth -= ClampedDamage;

	if (MyHealth <= 0 && BeginIsDead == false)
	{
		BeginIsDead = true;

		OnDeath.Broadcast();

		if (!IsPlayerControlled())
		{
			auto MyTempController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (MyTempController) 
			{ 
				auto MyTempPawn = MyTempController->GetPawn();
			}

			// remove the actors once they are dead
			GetWorldTimerManager().SetTimer(Timer_RemoveOnDead, this, &ATank::RemoveOnDead, 10.0f, false, 10.0f);
		}
	}
	return ClampedDamage;
}

float ATank::GetHealthPercent()
{
	return (float)MyHealth / (float)StartingHealth;	 
}


bool ATank::HasDied()
{
	return BeginIsDead;
}

void ATank::CounterSliding()
{
	//this ensures that if the tank is AI controlled the function is skipped 
	if (!IsPlayerControlled()) { return; }

	auto MyMeshComp = Cast<UStaticMeshComponent>(GetRootComponent());
	if (!MyMeshComp) { return; }
	
	auto TankRightVector = MyMeshComp->GetRightVector().GetSafeNormal();
	auto MyVelocity = GetVelocity().GetSafeNormal();

	auto DotProductResult = FVector::DotProduct(TankRightVector, MyVelocity);

	// sets the counter sliding at specific amount
	if (GetVelocity().Size() < 1050) { return; }

	float CounterForce = -DotProductResult * (MyMeshComp->GetMass() * 900000);
	
	auto CounterForceApplied = (MyMeshComp->GetRightVector() * CounterForce) * GetWorld()->GetDeltaSeconds();

	MyMeshComp->AddForce(CounterForceApplied, NAME_None, false);

}

void ATank::RemoveOnDead()
{
	TArray<USceneComponent*> ChildrenArray;
	GetRootComponent()->GetChildrenComponents(true, ChildrenArray);

	for (auto i : ChildrenArray)
	{
		auto TempSprungWheel = Cast<ASprungWheel>(i->GetOwner());

		if (TempSprungWheel)
		{
			TempSprungWheel->Destroy();
		}
	}
	Destroy();
}