// Copyright Sigma
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
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
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	// Checking for physics Handle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::Grab()
{
	// UE_LOG(LogTemp, Warning, TEXT("Grabber pressed!"));

	// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// Only raycast when the key is pressed and see if we reach any actors with physics body collision channel set.
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();

	// If we hit something then attach the physics handle.
	if (HitResult.GetActor()) //pointer
	{
		// TODO attach a physics handle

		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd);
	}
}

void UGrabber::Release()
{
	// UE_LOG(LogTemp, Warning, TEXT("Grabber released!"));
	PhysicsHandle->ReleaseComponent();

	// TODO remove/release the physics handle.
}

void UGrabber::FindPhysicsHandle()
{
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle component found on %s."), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle component found on %s. Please add a PhysicsHandle component."), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	if (InputComponent)
	{
		// UE_LOG(LogTemp, Error, TEXT("Input component found on %s."), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// Ray-cast out to a certain distance (Reach)

	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	AActor *ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Looking at %s"), *Hit.GetActor()->GetName());
	}

	return Hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// If the physics handle is attached.
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	// Move the object we are moving.
	}
}

// REFERENCE CODE
// if (
// 	(GetWorld()->LineTraceSingleByObjectType(
// 		OUT Hit,
// 		PlayerViewPointLocation,
// 		LineTraceEnd,
// 		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
// 		TraceParams)))
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Looking at %s"), *Hit.GetActor()->GetName());
// 	// UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty! at %f"), GetWorld()->GetTimeSeconds());
// };

// DrawDebugLine(
// 	GetWorld(),
// 	PlayerViewPointLocation,
// 	LineTraceEnd,
// 	FColor(0, 255, 0),
// 	false,
// 	0.f,
// 	0,
// 	5.f);

// UE_LOG(LogTemp, Warning, TEXT("Viewpoint: %s"), *GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint());

// Ray-cast out to a certain distance so our reach (basically like a tractor beam)

// See what it hits
