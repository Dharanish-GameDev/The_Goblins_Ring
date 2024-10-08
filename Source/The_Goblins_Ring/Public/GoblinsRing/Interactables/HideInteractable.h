// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoblinsRing/Interactables/Interactable.h"
#include "HideInteractable.generated.h"

class AVettiyanCharacter;
/**
 * 
 */
UCLASS()
class THE_GOBLINS_RING_API AHideInteractable : public AInteractable
{
	GENERATED_BODY()

public:
	AHideInteractable();
	virtual void Interact() override;
	void HideVettiyan(AVettiyanCharacter* vettiyanChar);

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USceneComponent* hidingPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* releasingPoint;

public:
	FORCEINLINE USceneComponent* GetReleasingPoint() const { return releasingPoint;}
};
