#include "TankMovementComponent.h"
#include "TankTrack.h"
#include "Tank.h"

void UTankMovementComponent::InitialiseMoveComponent(UTankTrack* LeftTrack, UTankTrack* RightTrack)
{
	LeftSideWheels = LeftTrack;
	RightSideWheels = RightTrack;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// AI tank movement 	
	auto TankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIMoveDirection = MoveVelocity.GetSafeNormal();
	
	// Moving the AI tank 
	// getting the speed by the angle difference.    
	auto DotProductResult = FVector::DotProduct(TankForwardDirection, AIMoveDirection); 
	IntendMoveForward(DotProductResult);

	// Turning the AI Tank
	// get the speed based on the Z axis 
	auto CrossProductResult = FVector::CrossProduct(TankForwardDirection, AIMoveDirection).Z; 
	IntendTurnRight(CrossProductResult);
}

void UTankMovementComponent::IntendMoveForward(float Amount)
{
	if (!ensure(LeftSideWheels && RightSideWheels)) { return; }

	LeftSideWheels->SetThrottle(Amount);
	RightSideWheels->SetThrottle(Amount);

}

void UTankMovementComponent::IntendTurnRight(float Amount)
{
	if (!ensure(LeftSideWheels && RightSideWheels)) { return; }
	LeftSideWheels->SetThrottle(Amount);
	RightSideWheels->SetThrottle(-Amount);
}
