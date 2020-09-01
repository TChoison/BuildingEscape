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

	if (!GetWorld()->GetFirstPlayerController())
	{
		UE_LOG(LogTemp, Error, TEXT("cannot get player controller"));
	}
	else if (!GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		UE_LOG(LogTemp, Error, TEXT("cannot get pawn"))
	}
	else
	{
	ActorWhoOpenDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}

void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner();
	// ways to set rotation:
	FRotator OwnerRotator = FRotator(0.f, -80.f, 0.f);
	Owner->SetActorRotation(OwnerRotator);
	//FQuat OwnerQuat = { 30, 30, 30, 30 };
	//Owner->SetActorRotation(OwnerQuat);

	//// ways to get rotation:
	//FString OwnerRotation = "";
	//OwnerRotation = Owner->GetActorRotation().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("rotation is %s"), *OwnerRotation); // same as editor
	//OwnerRotation = Owner->GetTransform().GetRotation().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("rotation2 is %s"), *OwnerRotation); // quite strange
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorWhoOpenDoor))
	{
		OpenDoor();
	}
}

