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
	if (PhysicsHandle)
	{
		// TODO
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("cannot find physics handler in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("find input component"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("cannot find Input component in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))
	auto HitResult = GetHitResult();
	auto ComponentToGrab = HitResult.GetComponent();
	// grab object if we hit something
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))

	// release the object we are grabbing
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation
	);
	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;


	// move the component when grabbing it
	PhysicsHandle->SetTargetLocation(LineTraceEnd);
}

FHitResult UGrabber::GetHitResult() const
{
	// get player view point
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation
	);
	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	FHitResult Hit;
	FCollisionQueryParams QueryParams(FName(TEXT("")), false, GetOwner());

	// line tracing (AKA ray-casting)
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParams
	);
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("hit result is %s"), *(HitActor->GetName()));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s hit nothing"), *(GetOwner()->GetName()));
	}

	return Hit;
}
