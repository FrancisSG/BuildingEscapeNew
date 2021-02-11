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

FVector UGrabber::GetPlayerReach() const
{
	// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerLocation() const
{
	// Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	// Ray-cast out to a certain distance (Reach)

	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	AActor *ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Looking at %s"), *Hit.GetActor()->GetName());
	}

	return Hit;
}

void UGrabber::Grab()
{
	// Only raycast when the key is pressed and see if we reach any actors with physics body collision channel set.
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();

	// If we hit something then attach the physics handle.
	if (HitResult.GetActor()) //pointer
	{
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayerReach());
	}
}

void UGrabber::Release()
{
	// Release physics handle
	PhysicsHandle->ReleaseComponent();
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
	// Keybinds setup
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the physics handle is attached.
	if(PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are moving.
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

