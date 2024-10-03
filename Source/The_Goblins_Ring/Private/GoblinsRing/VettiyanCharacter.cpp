#include "GoblinsRing/VettiyanCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GoblinsRing/Attributes/VettiyanAttributes.h"
#include "Animation/AnimMontage.h"
#include "GoblinsRing/Interactables/Interactable.h"
#include"GoblinsRing/Interactables/DoorInteractable.h"
#include "GoblinsRing/HUD/StaminaBarWidget.h"


AVettiyanCharacter::AVettiyanCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 380.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// View Camera
	viewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	viewCamera->SetupAttachment(GetRootComponent());

	//Skeletal Mesh
	handsMeshComponent = GetMesh();
	handsMeshComponent->SetupAttachment(viewCamera);

	//Attribute that holds Vettiyan's Energy or some other Values 
	vettiyanAttribute = CreateDefaultSubobject<UVettiyanAttributes>(TEXT("VettiyanAttribute"));

	characterMovementComponent = GetCharacterMovement();
}

void AVettiyanCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Setting Initial Camera Pos And Rotation For Headbobling and Camera Sway
	originalCameraPosition = viewCamera->GetRelativeLocation();
	originalCameraRotation = viewCamera->GetRelativeRotation();


	// Adding Input Mapping Context to the Player Controller
	if (APlayerController* playerController = CastChecked<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* localPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			if (vettiyanCharInputContext != nullptr)
			{
				localPlayerSubsystem->AddMappingContext(vettiyanCharInputContext,0);
			}
		}
	}

	walkSpeed = GetCharacterMovement()->MaxWalkSpeed / 2;
	sprintSpeed = walkSpeed * 2;

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	// Creating Stamina Widget
	if (staminaBarWidgetClass)
	{
		UStaminaBarWidget* StaminaBarWidget = CreateWidget<UStaminaBarWidget>(GetWorld(), staminaBarWidgetClass);

		if (StaminaBarWidget)
		{
			StaminaBarWidget->AddToViewport();

			// Store a reference to interact with the widget later
			staminaBarWidgetInstance = StaminaBarWidget;

			staminaBarWidgetInstance->SetStaminaPercent(vettiyanAttribute->GetCurrentStaminaPercent());
		}
	}
}

void AVettiyanCharacter::Look(const FInputActionValue& value)
{
	const FVector2D lookInputValue = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(lookInputValue.X);
		AddControllerPitchInput(lookInputValue.Y);
	}
}

void AVettiyanCharacter::MoveCharacter(const FInputActionValue& value)
{
	if (vettiyanState == EVettiyanStates::EVS_Occupied) return;

	const FVector2D moveInputValue = value.Get<FVector2D>();
    
	// Get the player's control rotation (yaw only)
	const FRotator controlRotation = GetControlRotation();
	const FRotator yawRotation(0, controlRotation.Yaw, 0);

	// Calculate the forward (X-axis) and right (Y-axis) movement directions based on yaw
	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);  // Forward/Backward
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);    // Left/Right

	// Apply movement input in both forward and right directions based on the input values
	AddMovementInput(forwardDirection, moveInputValue.X); // Apply forward/backward movement
	AddMovementInput(rightDirection, moveInputValue.Y);   // Apply right/left movement

	if (bIstryingToSprint)
	{
		if (vettiyanAttribute->CanSprint())
		{
			if (moveInputValue.X > 0 && moveInputValue.Y == 0)
			{
				bIsSprinting = true;
				characterMovementComponent->MaxWalkSpeed = sprintSpeed;
			}
			else
			{
				StopSprinting();
			}
		}
		else
		{
			StopSprinting();
		}
	}
	else
	{
		StopSprinting();
	}
}

void AVettiyanCharacter::StartSprinting()
{
	bIstryingToSprint = true;
}

void AVettiyanCharacter::StopSprinting()
{
	bIstryingToSprint = false;
	DisableSprinting();
}

void AVettiyanCharacter::Interact()
{
	if (overlappingInteractable == nullptr) return;
	if (ADoorInteractable* doorInteractable = Cast<ADoorInteractable>(overlappingInteractable))
	{
		doorInteractable->Interact();

		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && doorOpenMontage)
		{
			animInstance->Montage_Play(doorOpenMontage);
			animInstance->Montage_JumpToSection(FName("OpenDoor"), doorOpenMontage);
			vettiyanState = EVettiyanStates::EVS_Occupied;
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(2, 2, FColor::Red, FString::Printf(TEXT("Door")));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(2, 2, FColor::Red, FString::Printf(TEXT("Its Not Door")));
		}
	}
}

void AVettiyanCharacter::MakeUnOccupied()
{
	vettiyanState = EVettiyanStates::EVS_UnOccupied;
}

void AVettiyanCharacter::DisableSprinting()
{
	bIsSprinting = false;
	characterMovementComponent->MaxWalkSpeed = walkSpeed;
}

void AVettiyanCharacter::ApplyHeadBobbing(float DeltaTime)
{
	if (!GetCharacterMovement()->IsMovingOnGround()) return; // Only apply when on the ground

	float speed = GetVelocity().Size();
	float bobOffset = FMath::Sin(GetWorld()->TimeSeconds * bobFrequency) * bobAmount * (speed / GetCharacterMovement()->MaxWalkSpeed);

	// Update camera position for head bobbing
	FVector newCameraPosition = originalCameraPosition;
	newCameraPosition.Z += bobOffset;
	viewCamera->SetRelativeLocation(newCameraPosition);
}

void AVettiyanCharacter::ApplyCameraSway(float DeltaTime)
{
	if (!Controller) return;

	// Get mouse input for sway effect
	FVector2D mouseInput;
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		playerController->GetInputMouseDelta(mouseInput.X, mouseInput.Y);
	}

	// Calculate camera sway based on mouse movement
	FRotator swayRotation = originalCameraRotation;
	swayRotation.Yaw += mouseInput.X * swayAmount;
	swayRotation.Pitch -= mouseInput.Y * swayAmount;

	// Apply interpolation for smoother movement
	FRotator currentRotation = viewCamera->GetRelativeRotation();
	FRotator targetRotation = FMath::RInterpTo(currentRotation, swayRotation, DeltaTime, swaySpeed);

	// Set the camera rotation
	viewCamera->SetRelativeRotation(targetRotation);
}

void AVettiyanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (vettiyanAttribute && staminaBarWidgetInstance)
	{
		if (vettiyanState == EVettiyanStates::EVS_Occupied) return;
		vettiyanAttribute->ModifyEnergy(DeltaTime, bIsSprinting);
		staminaBarWidgetInstance->SetStaminaPercent(vettiyanAttribute->GetCurrentStaminaPercent());
	}
}

void AVettiyanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// Binding Callback Methods to the InputActions(IA)
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &AVettiyanCharacter::MoveCharacter);
		EnhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &AVettiyanCharacter::Look);
		EnhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Started, this, &AVettiyanCharacter::StartSprinting);
		EnhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Completed, this, &AVettiyanCharacter::StopSprinting);
		EnhancedInputComponent->BindAction(interactAction, ETriggerEvent::Started, this, &AVettiyanCharacter::Interact);
	}
}

