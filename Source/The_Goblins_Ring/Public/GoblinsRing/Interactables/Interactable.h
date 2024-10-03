// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class THE_GOBLINS_RING_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractable();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact();

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBoxBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* meshComponent;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* boxComponent;

private:

};
