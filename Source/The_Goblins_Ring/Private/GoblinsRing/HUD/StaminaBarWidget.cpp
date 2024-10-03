// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinsRing/HUD/StaminaBarWidget.h"
#include "Components/ProgressBar.h"


void UStaminaBarWidget::SetStaminaPercent(float percent)
{
	if (StaminaBar != nullptr)
	{
		StaminaBar->SetPercent(percent);
	}
}
