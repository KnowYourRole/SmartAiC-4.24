#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

//Must be last Include
#include "TankTrack.generated.h"

class USphereComponent;

UCLASS(ClassGroup = (TankParts), meta = (BlueprintSpawnableComponent)) 
class BATTLETANKS_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable)
		void SetThrottle(float Amount);


	UPROPERTY(EditDefaultsOnly, Category = "TrackSetup")
	float TrackMaxDrivingForce = 200000.0f;	

private: 
	UTankTrack();
	virtual void BeginPlay() override;

	TArray<class ASprungWheel*> GetWheels() const;		

	void DriveTrack(float Throttle);

	void CombineWheels();
	void RotateWheels();

	USphereComponent* MyMainWheel = nullptr;
	TArray<UStaticMeshComponent*> MeshWheels;
};
