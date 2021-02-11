// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPENEW_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;

private:

FRotator DoorRotation;

float InitialYaw;
float CurrentYaw;

UPROPERTY(EditAnywhere)
float OpenAngle = 90.f; //TargetYaw

UPROPERTY(EditAnywhere)
AActor* ActorThatOpens = nullptr;

float DoorLastOpened = 0.f;

UPROPERTY(EditAnywhere)
float DoorCloseDelay = 2.f;
		
UPROPERTY(EditAnywhere)
float DoorOpeningSpeed = 2.f;		

UPROPERTY(EditAnywhere)
float DoorCloseSpeed = 2.f;

UPROPERTY(EditAnywhere)
ATriggerVolume* PressurePlate = nullptr;

UPROPERTY(EditAnywhere)
float MassRequired = 50.f;
};
