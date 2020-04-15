#include "WeaponActor.h"

void UWeaponActor::Elevate(float ElevatePosition)
{	
	// this script helps the weapon to be moved the right amound based on the frame 
	// the weapon movement is given a max elevation speed and frame time 
	
	// this calculates the speed of the marrel based on the frame 
	ElevatePosition = FMath::Clamp<float>(ElevatePosition, -1, 1);
	auto ElevationChangeSpeed = (ElevatePosition * MaxDegreesPerSecond) * GetWorld()->GetDeltaSeconds();
	
	// this calculates the rotation value based on the frame and its current rotation
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChangeSpeed;
	auto ClampedNewElevation = FMath::Clamp<float>(RawNewElevation, MinDegreesElevation, MaxDegreesElevation);

	// updates the rotation with the new values  
	SetRelativeRotation((FRotator(ClampedNewElevation, 0, 0)));
}
