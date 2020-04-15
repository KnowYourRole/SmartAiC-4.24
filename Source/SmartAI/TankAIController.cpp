#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"					
#include "GameFramework/Pawn.h"	

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);  // this informs the complier to do what its super class does (tick)
	
	// this is used to change the values of GetPawn and set the pointers
	auto MyTank = GetPawn();     
	auto EnemyTankController = GetWorld()->GetFirstPlayerController();
	if (!EnemyTankController) { return; }
	auto EnemyTank = EnemyTankController->GetPawn();

	if (MyTank != nullptr)  //protection of the null pointer
	{
		if (EnemyTank != nullptr) //protection of the null pointer
		{
			
			// this enables the AI to move towards the player tank by adhering to the radius allowed 
			MoveToActor(EnemyTank, AcceptanceRadius); 
						
			auto MyAimingComponent = MyTank->FindComponentByClass<UTankAimingComponent>();
			MyAimingComponent->TurnAndAimAt(EnemyTank->GetActorLocation()); 

			// enables the AI to aim at the player if it is in firing status 
			if (MyAimingComponent->GetFiringStatus() == EFiringStatus::Locked)
			{
				//when ready, fire
				MyAimingComponent->Fire();
			}
		}
	}	
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	// setting the possessed tank to a variable that is usable 
	auto PossessedTank = Cast<ATank>(InPawn);
	if (PossessedTank == nullptr) { return; }

	// pass the function created to take action 
	PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
}

void ATankAIController::OnPossessedTankDeath()
{
	if (GetPawn() == nullptr) { return; } 
	GetPawn()->DetachFromControllerPendingDestroy(); 
}