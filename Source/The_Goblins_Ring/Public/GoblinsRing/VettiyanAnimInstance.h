// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VettiyanAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THE_GOBLINS_RING_API UVettiyanAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class AVettiyanCharacter* vettiyanCharacter;

	UPROPERTY(BlueprintReadOnly)
	bool isSprinting = false;
	
};
