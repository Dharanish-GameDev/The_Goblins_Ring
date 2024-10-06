// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoblinsRing/Interactables/Interactable.h"
#include "GameFramework/Actor.h"
#include "DoorInteractable.generated.h"

/**
 * 
 */
UCLASS()
class THE_GOBLINS_RING_API ADoorInteractable : public AInteractable
{
	GENERATED_BODY()

public:
	ADoorInteractable();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;

protected:

	virtual void BeginPlay() override;
	void StartOpening();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* doorMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Door_Properties")
	float TargetYaw = 100.f;  // Set this to the target yaw

	UPROPERTY(EditAnywhere, Category = "Door_Properties")
	float RotationSpeed = 45.f;  // Rotation speed in degrees per second

	UPROPERTY(EditAnywhere, Category = "Door_Properties")
	float DelayBeforeOpening = 2.f;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* crossDedectionBoxComponent;

	UFUNCTION()
	void OnCrossDedectorEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
private:
	bool bIsRotating = false;  // Flag to control whether the door is currently rotating
	bool bIsOpened = false;
	float currentYaw = 0.f;
	float initialYaw = 0.f;
	float tempYaw = 0.f;

	FTimerHandle rotationDelayHandle;  // Timer handle for delay
};
