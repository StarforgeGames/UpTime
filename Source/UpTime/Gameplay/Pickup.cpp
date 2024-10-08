#include "Pickup.h"

#include "Actors/UpTimePlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
APickup::APickup()
	: PickupDisplayName(TEXT(""))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("Pickup Collision");
	CollisionSphere->InitSphereRadius(200.f);
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetRootComponent(CollisionSphere);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	PickupMesh->SetupAttachment(RootComponent);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractMessageWidget = CreateDefaultSubobject<UWidgetComponent>("Interact Message Widget");
	InteractMessageWidget->SetupAttachment(RootComponent);
	InteractMessageWidget->SetRelativeLocation(FVector(0, 0, 200));
	InteractMessageWidget->SetRelativeRotation(FRotator(60, 180, 0));
	InteractMessageWidget->SetVisibility(false);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	if (InteractMessageWidgetRef)
	{
		InteractMessageWidget->SetWidgetClass(InteractMessageWidgetRef);
	}

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &APickup::OnEndOverlap);
}

void APickup::EnablePickup()
{
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUpTimePlayerCharacter* PlayerCharacter = Cast<AUpTimePlayerCharacter>(OtherActor);
	if (!PlayerCharacter)
	{
		return;
	}

	if (bAutoPickup)
	{
		OnPickedUp(PlayerCharacter);
	}
	else
	{
		SetInteractMessageVisibility(true);
		PlayerCharacter->FocusInteractable(this);
	}
}

void APickup::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AUpTimePlayerCharacter* PlayerCharacter = Cast<AUpTimePlayerCharacter>(OtherActor);
	if (!bIsPickupMessageDisplayed || !PlayerCharacter)
	{
		return;
	}

	SetInteractMessageVisibility(false);
	PlayerCharacter->UnfocusInteractable();
}

void APickup::SetInteractMessageVisibility(bool bNewVisibility)
{
	InteractMessageWidget->SetVisibility(bNewVisibility);
	bIsPickupMessageDisplayed = bNewVisibility;
}

void APickup::OnInteract_Implementation(AActor* OtherActor)
{
	AUpTimePlayerCharacter* PlayerCharacter = Cast<AUpTimePlayerCharacter>(OtherActor);
	if (!bIsPickupMessageDisplayed || !PlayerCharacter)
	{
		return;
	}

	OnPickedUp(PlayerCharacter);
}
