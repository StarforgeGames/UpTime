#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/Interactable.h"

#include "Pickup.generated.h"

UCLASS()
class UPTIME_API APickup : public AActor, public IInteractable
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<class UUserWidget> InteractMessageWidgetRef;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="UI")
	class UWidgetComponent* InteractMessageWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess="true"))
	FString PickupDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pickup")
	bool bAutoPickup = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Pickup")
	bool bIsPickupMessageDisplayed = false;

public:
	APickup();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Pickup")
	void EnablePickup();

	UFUNCTION(BlueprintCallable, Category="Pickup")
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category="Pickup")
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category="Pickup")
	void SetInteractMessageVisibility(bool bNewVisibility);

	/**
	 * Handle the interact action by the other actor.
	 * @param OtherActor The actor initiating the interact action.
	 */
	virtual void OnInteract_Implementation(AActor* OtherActor) override;

	/**
	 * Handles what happens on the actual pickup action.
	 * @param PlayerCharacter The character that is picking up the item.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void OnPickedUp(class AUpTimePlayerCharacter* PlayerCharacter);
};
