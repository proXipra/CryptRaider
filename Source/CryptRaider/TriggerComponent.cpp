// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"




UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();


}


void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = GetAcceptableActor();

	if (Actor != nullptr)
	{
		UPrimitiveComponent* component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (component != nullptr)
		{
			component->SetSimulatePhysics(false);
			component->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		
		Mover->SetShouldMove(true);
		UE_LOG(LogTemp, Display, TEXT("Unlocking.."));
	}
	else
	{
		Mover->SetShouldMove(false);
		UE_LOG(LogTemp, Display, TEXT("Relocking.."));
	}
	
}


void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}


AActor* UTriggerComponent::GetAcceptableActor()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		bool HasAcceptableTag = Actor->ActorHasTag(KeyTag);
		bool IsGrabbed = Actor->ActorHasTag("Grabbed");
		if (HasAcceptableTag && !IsGrabbed)
		{
			return Actor;
		}
	}
	return nullptr;
}

