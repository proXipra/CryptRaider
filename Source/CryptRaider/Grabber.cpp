// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Math/Vector.h"


#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandler = GetPhysicsHandle();
	
	if (PhysicsHandler && PhysicsHandler->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandler->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandler = GetPhysicsHandle();

	if (PhysicsHandler && PhysicsHandler->GetGrabbedComponent())
	{
		PhysicsHandler->GetGrabbedComponent()->WakeAllRigidBodies();

		AActor* GrabbedActor = PhysicsHandler->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");

		PhysicsHandler->ReleaseComponent();

		//UE_LOG(LogTemp, Display, TEXT("Component Succesfully Released!"));
	}
	
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandler = GetPhysicsHandle();
	if (PhysicsHandler == nullptr)
	{
		return;
	}
	
	bool HasHit;
	FHitResult HitResult;
	GetGrabbableInReach(&HitResult, &HasHit);
	
	if(HasHit)
	{
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, true, 3);
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		HitResult.GetActor()->Tags.Add("Grabbed");
		HitComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		//The start point of grabbing
		PhysicsHandler->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
	}
}


UPhysicsHandleComponent* UGrabber::GetPhysicsHandle()
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandler is Null!"));
	}
	return Result;
}

void UGrabber::GetGrabbableInReach(FHitResult* HitResult, bool* HasHit)
{
	FVector StartVector = GetComponentLocation();
	FVector EndVector = StartVector + GetForwardVector() * MaxGrabDistance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	//DrawDebugLine(GetWorld(), StartVector, EndVector, FColor::Red);
	*HasHit = GetWorld()->SweepSingleByChannel(*HitResult,
		StartVector, 
		EndVector, 
		FQuat::Identity, 
		ECC_GameTraceChannel2,
	 	Sphere);
}


