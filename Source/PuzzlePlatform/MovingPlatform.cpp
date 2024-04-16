// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::IncrementTriggersCount()
{
	++TriggersCount;
}

void AMovingPlatform::DecrementTriggersCount()
{
	if(TriggersCount > 0) --TriggersCount;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	// set start location
	StartLocation = GetActorLocation();
	// set global target location
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
	if(HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);	
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(TriggersCount < TriggersNeeded) return;
	if(HasAuthority())
	{
		const FVector CurrentLocation = GetActorLocation();
		const FVector Direction = (GlobalTargetLocation - StartLocation).GetSafeNormal();
		const FVector Target = GlobalTargetLocation + Direction;
		const float JourneyLength = (GlobalTargetLocation - StartLocation).Size();
		const FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, Target, DeltaTime, JourneyLength / Time);
		SetActorLocation(NewLocation);
		const float JourneyTravelled = (CurrentLocation - StartLocation).Size();
		if(JourneyTravelled > JourneyLength)
		{
			const FVector Swap = StartLocation;
			StartLocation = GlobalTargetLocation;
			GlobalTargetLocation = Swap;
		}
	}
}
