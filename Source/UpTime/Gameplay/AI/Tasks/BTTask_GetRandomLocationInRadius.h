#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_GetRandomLocationInRadius.generated.h"

/**
 * 
 */
UCLASS()
class UPTIME_API UBTTask_GetRandomLocationInRadius : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_GetRandomLocationInRadius();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	/** The radius in which the random location should lie (Origin: Blackboard Key). */
	UPROPERTY(EditAnywhere, Category = "Node", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Radius;

	/** The blackboard value to set with the resulting random location when task has been executed successfully. */
	UPROPERTY(EditAnywhere, Category = "Node")
	FBlackboardKeySelector RandomLocationKey;
};
