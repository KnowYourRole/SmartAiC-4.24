#include "TankHead.h"

void UTankHead::RotateTurret(float RotationPosition)
{
	auto RotationReq = FMath::Abs(RotationPosition);
	if (RotationReq < 1.0f) { return; }
	
	//normalises the position value to be used with the speed value
	RotationPosition = FMath::Clamp<float>(RotationPosition, -1, 1);
	
	// the final rotation amount is calculated 
	auto RotationChangeSpeed = (RotationPosition * MaxRotationPerSecond) * GetWorld()->GetDeltaSeconds();

	auto FinalRotation = RelativeRotation.Yaw + RotationChangeSpeed;
	
	//Tank Head is rotated 
	SetRelativeRotation(FRotator(0, FinalRotation, 0));
	
}