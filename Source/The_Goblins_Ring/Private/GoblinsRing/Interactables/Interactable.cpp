
#include "GoblinsRing/Interactables/Interactable.h"
#include "Components/BoxComponent.h"
#include "GoblinsRing/VettiyanCharacter.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = meshComponent;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	boxComponent->SetupAttachment(GetRootComponent());

}
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnBoxBeginOverLap);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnBoxEndOverLap);
}
void AInteractable::Interact()
{

}
void AInteractable::OnBoxBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVettiyanCharacter* vettiyan = Cast<AVettiyanCharacter>(OtherActor);

	if (vettiyan != nullptr)
	{
		vettiyan->SetOverlappingInteractable(this);
	}
}
void AInteractable::OnBoxEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVettiyanCharacter* vettiyan = Cast<AVettiyanCharacter>(OtherActor);

	if (vettiyan != nullptr)
	{
		vettiyan->SetOverlappingInteractable(nullptr);
	}
}
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

