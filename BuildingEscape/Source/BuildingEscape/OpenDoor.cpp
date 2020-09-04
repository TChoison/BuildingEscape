// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s miss the component PressurePlate"), *Owner->GetName())
	}
}

void UOpenDoor::OpenDoor()
{
	OnOpenRequest.Broadcast();
	//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	OnDoorOpen.Broadcast();
	OnCloseRequest.Broadcast();
	//Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOverlapMass() > 25.f)
	{
		OpenDoor();
		LastOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastOpenTime > DelayTime)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetOverlapMass()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlapActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OverlapActors);
	for (auto* actor : OverlapActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("name of actor is: %s"), *actor->GetName())
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

