#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PowerSystemComponent.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE(FPowerDrained);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPTIME_API UPowerSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPowerSystemComponent();

protected:
	void BeginPlay() override;

public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * \brief Increases the current power by the given amount.
	 * \param ChargeValue The amount to add.
	 */
	UFUNCTION(BlueprintCallable, Category="Power System")
	void ChargePower(int ChargeValue);
	
	/**
	 * \brief Decreases the current power by the given amount.
	 * \param DrainValue The amount to subtract.
	 */
	UFUNCTION(BlueprintCallable, Category="Power System")
	void DrainPower(float DrainValue);

	UFUNCTION(BlueprintGetter, Category="Power System")
	FORCEINLINE float GetCurrentPower() const { return CurrentPower; }

	UFUNCTION(BlueprintSetter, Category="Power System")
	void SetCurrentPower(float NewPower);

	/**
	 * \brief The power value converted to the number of power cells based on the conversion factor.
	 * \return The number of power cells remaining.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Power System")
	FORCEINLINE int GetCurrentPowerCells() const { return FMath::FloorToInt(CurrentPower / ConversionFactor); }

	UPROPERTY()
	FPowerDrained OnPowerDrained;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetCurrentPower, BlueprintSetter=SetCurrentPower, Category="Power System")
	float CurrentPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Power System")
	int ConversionFactor;
	
	/** Sound when the enemy is killed. */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class USoundCue* PowercellDrainedSound;
	
	UPROPERTY()
	UAudioComponent* PowercelLDrainedAudio;
};
