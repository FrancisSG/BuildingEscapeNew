// Copyright Sigma

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#define OUT

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

	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component on it, but no PressurePlate set!"), *GetOwner()->GetName());
	}
}

// Find audio component
void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing an audio component!"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassRequired)
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

	CloseDoorSound = false;

	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	DoorRotation.Yaw = FMath::FInterpTo(GetOwner()->GetActorRotation().Yaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	GetOwner()->SetActorRotation(DoorRotation);
	
	OpenDoorSound = false;
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	// Find All Overlapping Actors

	TArray<AActor *> OverlappingActors;
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate is note set! Null Pointer detected."))
		return TotalMass; //returns 0.f if PressurePlate is a nullPointer
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Add up their Masses
	for (AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate!"), *Actor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%f is the current mass"), TotalMass);
	}

	return TotalMass;
}
