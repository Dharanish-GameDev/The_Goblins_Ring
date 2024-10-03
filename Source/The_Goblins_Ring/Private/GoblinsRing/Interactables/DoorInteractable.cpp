// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinsRing/Interactables/DoorInteractable.h"

ADoorInteractable::ADoorInteractable()
{
	doorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door_Mesh"));
	doorMeshComponent->SetupAttachment(GetRootComponent());
}

void ADoorInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsRotating)
    {
        // Interpolate towards the target yaw smoothly
        float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, RotationSpeed);

        // Set the new yaw to the door mesh
        doorMeshComponent->SetRelativeRotation(FRotator(0, NewYaw, 0));

        // Update CurrentYaw to track the door's rotation progress
        CurrentYaw = NewYaw;

        // Check if we've reached the target yaw
        if (FMath::IsNearlyEqual(CurrentYaw, TargetYaw, 1.f))
        {
            // Once reached, stop the rotation
            bIsRotating = false;
        }
    }
}

void ADoorInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorInteractable::StartOpening()
{
    bIsRotating = true;
}

void ADoorInteractable::Interact()
{
	Super::Interact();
    // Set target yaw to rotate towards
    TargetYaw = 100.f;  // You can change this value based on door state (open/close)

    // Get current yaw of the door mesh
    CurrentYaw = doorMeshComponent->GetRelativeRotation().Yaw;

    // Set the flag to true so that Tick can handle the smooth rotation
    // Set a timer to delay the rotation by a certain amount of time
    GetWorld()->GetTimerManager().SetTimer(RotationDelayHandle, this, &ADoorInteractable::StartOpening, DelayBeforeOpening, false);

}
