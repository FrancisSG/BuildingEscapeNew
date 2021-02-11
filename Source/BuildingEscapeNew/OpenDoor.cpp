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

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Warning, TEXT("DoorLastOpened: %f DoorCloseDelay: %f"), DoorLastOpened, DoorCloseDelay);
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("DoorLastOpened: %f DoorCloseDelay: %f"), DoorLastOpened, DoorCloseDelay);

		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s's Yaw is : %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);

	DoorRotation.Yaw = FMath::FInterpTo(GetOwner()->GetActorRotation().Yaw, OpenAngle, DeltaTime, DoorOpeningSpeed);
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s's Yaw is : %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);

	DoorRotation.Yaw = FMath::FInterpTo(GetOwner()->GetActorRotation().Yaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	GetOwner()->SetActorRotation(DoorRotation);
}

//------------REFERENCE CODE

/* float MyFloat = 90.f;	

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = MyFloat;
	GetOwner()->SetActorRotation(CurrentRotation); */

/* FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = 90.f;
	GetOwner()->SetActorRotation(CurrentRotation); */

// FRotator OpenDoor(0.f, 0.f, 90.f); //This also works

// GetOwner()->SetActorRotation({0.f, 0.f, 0.f});

/* float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator OpenDoor(0.f, 0.f, 0.f);
	OpenDoor.Yaw = FMath::Lerp(CurrentYaw, OpenAngle, 0.02f);
	// OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, OpenAngle, DeltaTime, 45.f);
	// OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, 2.f);

	GetOwner()->SetActorRotation(OpenDoor);

	UE_LOG(LogTemp, Error, TEXT("TESTLINE")); */

// Set Actor Rotation

/* FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentYaw = CurrentRotation.Yaw;
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, 2.f);
	GetOwner()->SetActorRotation(CurrentRotation); */

// CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, 2.f);
// DoorRotation.Yaw = CurrentYaw;
// GetOwner()->SetActorRotation(DoorRotation);