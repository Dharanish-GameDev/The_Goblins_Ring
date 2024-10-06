// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinsRing/Interactables/DoorInteractable.h"
#include "GoblinsRing/VettiyanCharacter.h"
#include "Components/BoxComponent.h"

ADoorInteractable::ADoorInteractable()
{
	doorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door_Mesh"));
	doorMeshComponent->SetupAttachment(GetRootComponent());

    crossDedectionBoxComponent = CreateDefaultSubobject<UBoxComponent>("FrontCrossDedector");
    crossDedectionBoxComponent->SetupAttachment(GetRootComponent());
}

void ADoorInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsRotating)
    {
        float newYaw = FMath::FInterpTo(currentYaw,tempYaw, DeltaTime, RotationSpeed);
        doorMeshComponent->SetRelativeRotation(FRotator(0, newYaw, 0));
        currentYaw = newYaw;

        if (bIsOpened)
        {
            if (FMath::IsNearlyEqual(currentYaw,initialYaw, 0.5f))
            {
                bIsRotating = false;
                bIsOpened = false;
            }
        }
        else
        {
            if (FMath::IsNearlyEqual(currentYaw, TargetYaw, 0.5f))
            {
                bIsRotating = false;
                bIsOpened = true;
            }
        }
        
    }
}

void ADoorInteractable::BeginPlay()
{
	Super::BeginPlay();
    initialYaw = doorMeshComponent->GetRelativeRotation().Yaw; // Storing the Initial Yaw for Closing Purpose
    crossDedectionBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADoorInteractable::OnCrossDedectorEndOverLap);
}

void ADoorInteractable::StartOpening()
{
    bIsRotating = true;
    tempYaw = TargetYaw;
}

void ADoorInteractable::OnCrossDedectorEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!bIsOpened) return;
    if (AVettiyanCharacter* vettiyanCharacter = Cast<AVettiyanCharacter>(OtherActor))
    {
        bIsRotating = true;
        tempYaw = initialYaw;
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(1, 3, FColor::Blue, FString::Printf(TEXT("The Vettiyan Crossed the Cross Dedector")));
        }
    }
}

void ADoorInteractable::Interact()
{
	Super::Interact();
    currentYaw = doorMeshComponent->GetRelativeRotation().Yaw;
    GetWorld()->GetTimerManager().SetTimer(rotationDelayHandle, this, &ADoorInteractable::StartOpening, DelayBeforeOpening, false);

}
