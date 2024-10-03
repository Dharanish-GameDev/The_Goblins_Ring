// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinsRing/VettiyanAnimInstance.h"
#include "GoblinsRing/VettiyanCharacter.h"

void UVettiyanAnimInstance::NativeInitializeAnimation()
{
	vettiyanCharacter = Cast<AVettiyanCharacter>(TryGetPawnOwner());
}

void UVettiyanAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	if (vettiyanCharacter != nullptr)
	{
		isSprinting = vettiyanCharacter->GetIsSprinting();
	}
}
