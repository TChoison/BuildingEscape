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

	UE_LOG(LogTemp, Warning, TEXT("Delay time is set to %f"), DelayTime);
}

void UOpenDoor::OpenDoor()
{
	// ways to set rotation:
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	//FQuat OwnerQuat = { 30, 30, 30, 30 };
	//Owner->SetActorRotation(OwnerQuat);

	//// ways to get rotation:
	//FString OwnerRotation = "";
	//OwnerRotation = Owner->GetActorRotation().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("rotation is %s"), *OwnerRotation); // same as editor
	//OwnerRotation = Owner->GetTransform().GetRotation().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("rotation2 is %s"), *OwnerRotation); // quite strange
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorWhoOpenDoor))
	{
		OpenDoor();
		LastOpenTime = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("Open Door at time: %f"), LastOpenTime);
	}

	UE_LOG(LogTemp, Warning, TEXT("what is the value of LastOpenTime %f"), LastOpenTime);
	if (GetWorld()->GetTimeSeconds() - LastOpenTime > DelayTime)
	{
		CloseDoor();
	}
}

