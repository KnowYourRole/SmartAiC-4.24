#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

UCLASS( ClassGroup = (TankParts), meta = (BlueprintSpawnableComponent) )
class BATTLETANKS_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	///Fly-by-wire Controls
	UFUNCTION(BlueprintCallable, Category = "TankSetup") // tank can move back and forward 
		void IntendMoveForward(float Amount);

	UFUNCTION(BlueprintCallable, Category = "TankSetup") // tank can move left and right 
		void IntendTurnRight(float Amount);

	///Setup
	UFUNCTION(BlueprintCallable, Category = "TankSetup")
		void InitialiseMoveComponent(UTankTrack* LeftTrack, UTankTrack* RightTrack);


private:
	UTankTrack* RightSideWheels = nullptr;
	UTankTrack* LeftSideWheels = nullptr;

	// this function is called from the move to actor function and this is a Polymorphism 
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override; 
};
