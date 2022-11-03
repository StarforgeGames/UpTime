#include "BTTask_GetRandomLocationInRadius.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "VisualLogger/VisualLogger.h"


UBTTask_GetRandomLocationInRadius::UBTTask_GetRandomLocationInRadius()
	: Radius(1000.f)
{
	NodeName = "Get Random Location In Radius";
}

EBTNodeResult::Type UBTTask_GetRandomLocationInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (BlackboardKey.SelectedKeyType != UBlackboardKeyType_Vector::StaticClass())
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		UE_VLOG(AIController, 
			LogBehaviorTree, 
			Warning,
			TEXT("UBTTask_GetRandomLocationInRadius::ExecuteTask tried to retrieve Guard Location BB %s entry was empty"), 
			*BlackboardKey.SelectedKeyName.ToString());
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!ensureMsgf(NavigationSystem, TEXT("Navigation System could not be retrieved to get random location in radius")))
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	const FVector GuardLocation = Blackboard->GetValueAsVector(BlackboardKey.SelectedKeyName);
	FNavLocation ResultLocation;
	
	if(!NavigationSystem->GetRandomReachablePointInRadius(GuardLocation, Radius, ResultLocation))
	{
		return EBTNodeResult::Failed;
	}

	Blackboard->SetValueAsVector(RandomLocationKey.SelectedKeyName, ResultLocation.Location);

	return EBTNodeResult::Succeeded;
}
