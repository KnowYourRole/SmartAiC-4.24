#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpawnPoint.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANKS_API USpawnPoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Default values for the component's properies are set
	USpawnPoint();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* GetGeneratedActor() const;

protected:
	// Called when the project is started 
	virtual void BeginPlay() override;

private: 

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		TSubclassOf<AActor> SpawnClass;				// This is for the way that the var for SpawnClass is initialised 

	AActor* SpawnedActor;
};
