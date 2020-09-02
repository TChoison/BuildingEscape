// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetPhysicsHandleComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetHitResults();

private:
	// set debug line length
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray cast and grab what is in reach
	void Grab();

	// Called when grab is released
	void Release();

	// set attached physics handle component
	void SetAPhysicsHandleComponent();

	// set attached Input component
	void SetInputComponent();

	// get line trace hit result
	const FHitResult GetHitResult();

	// return start location of trace line
	FVector GetTraceLineStart();

	// return end location of trace line
	FVector GetTraceLineEnd();
};
