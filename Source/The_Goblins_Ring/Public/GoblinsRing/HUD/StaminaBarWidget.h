// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class THE_GOBLINS_RING_API UStaminaBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	void SetStaminaPercent(float percent);
};
