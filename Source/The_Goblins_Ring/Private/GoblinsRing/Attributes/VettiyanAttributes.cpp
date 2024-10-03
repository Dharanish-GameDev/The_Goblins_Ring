#include "GoblinsRing/Attributes/VettiyanAttributes.h"

UVettiyanAttributes::UVettiyanAttributes()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVettiyanAttributes::ModifyEnergy(float DeltaTime, bool bIsSprinting)
{
    if (bIsSprinting && CanSprint())
    {
        // Decrease energy while sprinting
        currentEnergy = FMath::Max(0.f, currentEnergy - energyDecresingRate * DeltaTime);
    }
    else if (!bIsSprinting)
    {
        // Increase energy when not sprinting
        currentEnergy = FMath::Min(maxEnergy, currentEnergy + eneryIncreasingRate * DeltaTime);
    }
}

void UVettiyanAttributes::BeginPlay()
{
	Super::BeginPlay();
}

void UVettiyanAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

