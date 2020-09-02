// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	SetAPhysicsHandleComponent();
	SetInputComponent();
}

void UGrabber::SetAPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("cannot find physics handler in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("cannot find Input component in %s"), *GetOwner()->GetName());
	}
}

// grab object if we hit something
void UGrabber::Grab()
{
	auto HitResult = GetHitResult();
	auto ComponentToGrab = HitResult.GetComponent();
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, // no bone to set
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

// release the object we are grabbing
void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// move the component when grabbing it
	PhysicsHandle->SetTargetLocation(GetTraceLineEnd());
}

const FHitResult UGrabber::GetHitResult()
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams(FName(TEXT("")), false, GetOwner());
	// line tracing (AKA ray-casting)
	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		GetTraceLineStart(),
		GetTraceLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParams
	);
	AActor* HitActor = HitResult.GetActor();
	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("hit result is %s"), *(HitActor->GetName()));
	}
	return HitResult;
}

FVector UGrabber::GetTraceLineStart()
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation
	);
	return PlayerLocation;
}

FVector UGrabber::GetTraceLineEnd()
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation
	);
	return PlayerLocation + PlayerRotation.Vector() * Reach;
}