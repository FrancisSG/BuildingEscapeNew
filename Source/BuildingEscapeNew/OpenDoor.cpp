// Copyright Sigma

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
// #include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	DoorRotation = GetOwner()->GetActorRotation();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;

	OpenAngle += InitialYaw; //OpenAngle = OpenAngle + InitialYaw

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component on it, but no PressurePlate set!"), *GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > 50.f)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{

		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	DoorRotation.Yaw = FMath::FInterpTo(GetOwner()->GetActorRotation().Yaw, OpenAngle, DeltaTime, DoorOpeningSpeed);
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	DoorRotation.Yaw = FMath::FInterpTo(GetOwner()->GetActorRotation().Yaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find All Overlapping Actors

	TArray<AActor*> OverlappingActors; 
	PressurePlate->GetOverlappingActors(OverlappingActors);

	// Add up their Masses

	return TotalMass;
}
