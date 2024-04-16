// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	if(!ensure(TriggerVolume != nullptr)) return;
	RootComponent = TriggerVolume;
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	Cast<UBoxComponent>(RootComponent)->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlatformTrigger::OverlapBegins);
	Cast<UBoxComponent>(RootComponent)->OnComponentEndOverlap.AddUniqueDynamic(this, &APlatformTrigger::OverlapEnds);
}

void APlatformTrigger::OverlapBegins(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	for (AMovingPlatform* MovingPlatform : PlatformsToTrigger)
	{
		if(MovingPlatform)
		{
			MovingPlatform->IncrementTriggersCount();
		}
	}
}

void APlatformTrigger::OverlapEnds(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{

	for (AMovingPlatform* MovingPlatform : PlatformsToTrigger)
	{
		if(MovingPlatform)
		{
			MovingPlatform->DecrementTriggersCount();
		}
	}
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

