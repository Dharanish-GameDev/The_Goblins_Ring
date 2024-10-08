#include "GoblinsRing/Interactables/HideInteractable.h"
#include "Components/CapsuleComponent.h"
#include "GoblinsRing/VettiyanCharacter.h"

AHideInteractable::AHideInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	hidingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Hidding Point"));
	hidingPoint->SetupAttachment(GetRootComponent());

	releasingPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Releasing Point"));
	releasingPoint->SetupAttachment(GetRootComponent());
}

void AHideInteractable::Interact()
{
	Super::Interact();
	
}

void AHideInteractable::HideVettiyan(AVettiyanCharacter* vettiyanChar)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3, 2, FColor::Black, FString::Printf(TEXT("Its Hide Interactable!")));
	}

	if (hidingPoint)
	{
		vettiyanChar->SetVettiyanPosAndRot(hidingPoint);
		vettiyanChar->SetHideInteractable(this);
	}
	vettiyanChar->SeizeCharacter();
}
