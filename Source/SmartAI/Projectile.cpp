//All of the listed includes are neccessary to cast the projectiles
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"		
#include "Components/StaticMeshComponent.h"			
#include "TankProjectileMovementComponent.h"		
#include "PhysicsEngine/RadialForceComponent.h"		
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"


#define OUT

AProjectile::AProjectile()
{
	//Called every frame
	PrimaryActorTick.bCanEverTick = false;

	//Create a subobject and instantiate it in the editor
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("CollisionMesh"));
	SetRootComponent(BulletMesh);														//BulletMesh is instantiated in SetComponent to avoid errors
	BulletMesh->SetNotifyRigidBodyCollision(true);										//Keep collision active during casting

	///Use UE4 particle system library 
	TriggerShooting = CreateDefaultSubobject<UParticleSystemComponent>(FName("TriggerShooting"));			
	TriggerShooting->AttachTo(RootComponent);																	//Attaches this to the root component.

	/// Constructs the shoot effect
	ShotEffect = CreateDefaultSubobject<UParticleSystemComponent>(FName("ShotEffect"));
	ShotEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);			//attach components in the constructor
	ShotEffect->bAutoActivate = false;																		//instantiates as false when play the game


	
	ProjMoveComponent = CreateDefaultSubobject<UTankProjectileMovementComponent>(FName("TankProjectileMovement"));	//constructs projectile direction
	ProjMoveComponent->bAutoActivate = false;

	/// Generate a pullback radius
	PullBackStrength = CreateDefaultSubobject<URadialForceComponent>(FName("PullBackStrength"));
	PullBackStrength->AttachTo(RootComponent);
}

//Bigin with templated parameter
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	///delegate collision/evaluate collision based on projectile type mesh
	BulletMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);    
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//Use forward vector and speed  to cast Projectile
void AProjectile::CastDirection(float Speed)
{
	ProjMoveComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjMoveComponent->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Controll shoot bullets events
	TriggerShooting->Deactivate();
	ShotEffect->Activate();
	
	
	///  Use physics component radial force.
	PullBackStrength->FireImpulse();

	///Destroy bullet mesh immediately after the collision
	SetRootComponent(ShotEffect);
	BulletMesh->DestroyComponent();

	//This method drives from gameplay statistics and applies damage to the enemy/ It's currently called from the player script					
	UGameplayStatics::ApplyRadialDamage(	
		this,									//Allows simple modification over the damaging of the AI
		TotalDmg,
		GetActorLocation(),
		PullBackStrength->Radius,			
		UDamageType::StaticClass(),
		TArray<AActor*>()						//Points to the affected actor
	);

	///Count time before Destroy actor
	FTimerHandle Timer01;
	GetWorld()->GetTimerManager().SetTimer(
		OUT Timer01,						// define output parameter
		this,								
		&AProjectile::OnTimer01Expire,		//Use address of  function  so that it can be called when neccessary
		OnDeadDelay,						//Calls the pre-set time in the .h
		false								//Currently the code runs only once for the timer.
	);
}

void AProjectile::OnTimer01Expire()
{
	Destroy();								//Destroy projectile when timer expires
}

