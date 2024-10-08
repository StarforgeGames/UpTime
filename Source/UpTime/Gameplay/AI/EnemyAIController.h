#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"


#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UPTIME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category="AI|Combat")
	FName EnemyTargetKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category="AI|Combat")
	FName GuardLocationKey;

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable, Category="AI|Combat")
	void AttackEnemy();

	UFUNCTION(BlueprintCallable, Category="AI|Combat")
	class AUpTimePlayerCharacter* GetEnemy() const;

	UFUNCTION(BlueprintCallable, Category="AI|Combat")
	void SetEnemy(class AUpTimePlayerCharacter* Enemy) const;

protected:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
