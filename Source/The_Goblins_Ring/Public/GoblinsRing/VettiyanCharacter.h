#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GoblinsRingStates.h"
#include "VettiyanCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class UVettiyanAttributes;
class UStaminaBarWidget;
class AInteractable;
class AHideInteractable;

UCLASS()
class THE_GOBLINS_RING_API AVettiyanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AVettiyanCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SeizeCharacter();
	void UnSeizeCharacter();
	void SetVettiyanPosAndRot(USceneComponent* sceneComponent);

protected:
	virtual void BeginPlay() override;

	//Inputs

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterInputs)
	UInputMappingContext* vettiyanCharInputContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterInputs)
	UInputAction* lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterInputs)
	UInputAction* sprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterInputs)
	UInputAction* moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterInputs)
	UInputAction* interactAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UStaminaBarWidget> staminaBarWidgetClass;

	// Input Callback Methods
	void Look(const FInputActionValue& value);
	void MoveCharacter(const FInputActionValue& value);
	void StartSprinting();
	void StopSprinting();
	void Interact();

	UFUNCTION(BlueprintCallable)
	void MakeUnOccupied();

private:

	EVettiyanStates vettiyanState = EVettiyanStates::EVS_UnOccupied;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* viewCamera;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* handsMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UVettiyanAttributes* vettiyanAttribute;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* doorOpenMontage;


	//Sprint
	bool bIsSprinting = false;
	bool bIstryingToSprint = false;
	float walkSpeed;
	float sprintSpeed;

	// Camera shake settings
	UPROPERTY(EditAnywhere, Category = "Camera | Head Bob")
	float bobFrequency = 10.0f; // Frequency of the head bobbing (adjustable in the editor)

	UPROPERTY(EditAnywhere, Category = "Camera | Head Bob")
	float bobAmount = 2.0f; // Amount of camera bobbing (adjustable in the editor)

	UPROPERTY(EditAnywhere, Category = "Camera | Camera Sway")
	float swayAmount = 1.5f; // Amount of camera sway (adjustable in the editor)

	UPROPERTY(EditAnywhere, Category = "Camera | Camera Sway")
	float swaySpeed = 5.0f; // Speed of camera sway (adjustable in the editor)

	FVector originalCameraPosition; // Store original camera position for head bobbing effect
	FRotator originalCameraRotation; // Store original camera rotation for camera sway effect

	void ApplyHeadBobbing(float DeltaTime); // Function to apply head bobbing effect
	void ApplyCameraSway(float DeltaTime); // Function to apply camera sway effect
	void DisableSprinting();

	class UCharacterMovementComponent* characterMovementComponent;

	UPROPERTY(VisibleInstanceOnly)
	AInteractable* overlappingInteractable;

	UPROPERTY()
	UStaminaBarWidget* staminaBarWidgetInstance;

	UPROPERTY()
	AHideInteractable* hideInteractableObj;


public:

	FORCEINLINE void SetOverlappingInteractable(AInteractable* interactable) {overlappingInteractable = interactable;}
	FORCEINLINE void SetHideInteractable(AHideInteractable* hideInterac) { hideInteractableObj = hideInterac;}
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }
};
