

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VettiyanAttributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THE_GOBLINS_RING_API UVettiyanAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVettiyanAttributes();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE bool CanSprint() const
	{
		return currentEnergy > 0;
	}

	FORCEINLINE float GetCurrentStaminaPercent() const
	{
		return currentEnergy/maxEnergy;
	}

	void ModifyEnergy(float DeltaTime, bool bIsSprinting);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = Attributes)
	float currentEnergy = 0;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float maxEnergy = 100;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float eneryIncreasingRate = 1;
	
	UPROPERTY(EditAnywhere, Category = Attributes)
	float energyDecresingRate = 2;
};
