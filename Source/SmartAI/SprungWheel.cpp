#include "SprungWheel.h"
#include "Components/StaticMeshComponent.h" 
#include "PhysicsEngine/PhysicsConstraintComponent.h" 
#include "Components/SphereComponent.h" 

ASprungWheel::ASprungWheel()
{
 	// Called every frame 
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.TickGroup = TG_PostPhysics;

	
	// Adding the components to the spring 
	SpringImprovement = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("PhysicsConstraint(Spring)"));
	SetRootComponent(SpringImprovement);													// set the root component  
	SpringImprovement->SetAngularSwing1Limit(ACM_Locked, 0);								// set the angular constraint
	SpringImprovement->SetAngularSwing2Limit(ACM_Locked, 0);
	SpringImprovement->SetAngularTwistLimit(ACM_Locked, 0);
	SpringImprovement->SetLinearZLimit(LCM_Free, 300);									// set the linear restrictions (Z)
	SpringImprovement->SetLinearPositionDrive(false, false, true);						// Turning the position Z on
	SpringImprovement->SetLinearVelocityDrive(false, false, true);						// Turning the velocity Z on 
	SpringImprovement->SetLinearDriveParams(120.0f, 60.0f, 0);							// Position's and Velocity's Strengths
	
	// Adding the rod mesh component
	AxleMesh = CreateDefaultSubobject<USphereComponent>(FName("AxleMesh"));					// the mesh can be invisible if it's set to 
	AxleMesh->SetupAttachment(SpringImprovement);											// component attached

	// Adding constraints to the axle physics 
	AxlePhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("PhysicsConstraint(Axle)"));
	AxlePhysicsConstraint->SetupAttachment(AxleMesh);
	AxlePhysicsConstraint->SetAngularSwing1Limit(ACM_Locked, 0);								// set the angular constraint
	AxlePhysicsConstraint->SetAngularSwing2Limit(ACM_Free, 0);									// allows correct rotation of the vehicle forward
	AxlePhysicsConstraint->SetAngularTwistLimit(ACM_Locked, 0);									// set the twist motion
	
	// Adding the component for the spring 
	WheelMesh = CreateDefaultSubobject<USphereComponent>(FName("WheelMesh"));					// the mesh can be set to invisible 
	WheelMesh->SetupAttachment(AxleMesh);														// component attached 
}

void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	// setting the delegate function by calling a Unreal Engine 4 function to the mesh 
	WheelMesh->SetNotifyRigidBodyCollision(true);												// 'Stimulation generate hit events' are always turned on   			
	WheelMesh->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);							// called when the on hit component function is activated

	RestictSpeed();
	
}

void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetWorld()->TickGroup == TG_PostPhysics)			// this ensures that the tick group is correct 
	{
		CurrentSpeed = 0.0f;
	}
	else
	{
		PrimaryActorTick.TickGroup = TG_PostPhysics;
	}
}

void ASprungWheel::RestictSpeed()
{
	auto MyParentActor = GetAttachParentActor();		// this gets the root component, the component its attached to and the tank component
	if (MyParentActor)	// Protection of the pointer 
	{
		//Setting the restriction settings to: 
		//Component1 being the Tank Root and Component2 being the Axle
		auto BodyRoot_Converted = Cast<UPrimitiveComponent>(MyParentActor->GetRootComponent());		//Converting the root to UPrimitive component
		auto AxleMesh_Converted = Cast<UPrimitiveComponent>(AxleMesh);								//Converting the mesh to UPrimitive component
		SpringImprovement->SetConstrainedComponents(
			BodyRoot_Converted,
			NAME_None,
			AxleMesh_Converted,
			NAME_None
		);

		// setting the component1 to be axle and component2 to be the wheel 
		auto WheelMesh_Converted = Cast<UPrimitiveComponent>(WheelMesh);							//Converting the mesh to UPrimitive component
		AxlePhysicsConstraint->SetConstrainedComponents(											//Seting the component1 to be the tank root and component2 to be the axle mesh 
			AxleMesh_Converted,
			NAME_None,
			WheelMesh_Converted,
			NAME_None
		);
	}
}

void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Force to the wheel is applied
	WheelMesh->AddForce((AxleMesh->GetForwardVector() * CurrentSpeed));  
	
}

void ASprungWheel::AddingSpeed(float ForceMagnitude)
{
	CurrentSpeed += ForceMagnitude;
	
}