#include "EnemyAIController.h"

#include "UpTime.h"
#include "Actors/EnemyCharacter.h"
#include "Actors/Common/ArmedActor.h"
#include "Actors/UpTimePlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Perception/AIPerceptionComponent.h"


AEnemyAIController::AEnemyAIController()
	: BehaviorTree(nullptr),
	EnemyTargetKey("EnemyTarget"),
	GuardLocationKey("GuardLocation")
{}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	UAIPerceptionComponent* MyPerceptionComponent = GetPerceptionComponent();
	if (ensureMsgf(MyPerceptionComponent,
		TEXT("Perception component not set, cannot intialize target perception updating")))
	{
		MyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,
			&AEnemyAIController::OnTargetPerceptionUpdated);
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunBehaviorTree(BehaviorTree);

	UBlackboardComponent* AiBlackboard = GetBlackboardComponent();
	if (!AiBlackboard)
	{
		UE_LOG(LogUpTime, Error, TEXT("Blackboard is NULL in AI Controller OnPossess"));
		return;
	}

	const FVector CurrentLocation = InPawn->GetActorLocation();
	AiBlackboard->SetValueAsVector(GuardLocationKey, CurrentLocation);
}

void AEnemyAIController::AttackEnemy()
{
	AEnemyCharacter* MyPawn = Cast<AEnemyCharacter>(GetPawn());

	if (!IArmedActor::Execute_GetWeapon(MyPawn))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan,
			FString::Printf(TEXT("%s has no weapon"), *MyPawn->GetName()));
		return;
	}

	bool bCanAttack = false;
	AUpTimePlayerCharacter* Enemy = GetEnemy();

	if (Enemy)
	{
		if (Enemy->IsAlive())
		{
			if (IArmedActor::Execute_CanFire(MyPawn))
			{
				bCanAttack = LineOfSightTo(Enemy, MyPawn->GetActorLocation());
			}
		}
		else
		{
			UE_LOG(LogUpTime, Log, TEXT("Enemy not alive anymore, clearing target"));
			SetEnemy(nullptr);
		}
	}

	if (bCanAttack)
	{
		if (GetFocusActor() != Enemy)
		{
			SetFocus(Enemy);
		}

		const float Distance = MyPawn->GetDistanceTo(Enemy);
		if (Distance <= MyPawn->GetAttackRange())
		{
			IArmedActor::Execute_StartFiring(MyPawn);
		}
	}
	else if (IArmedActor::Execute_IsFiring(MyPawn))
	{
		UE_LOG(LogUpTime, Log, TEXT("Cannot attack enemy, clearing focus and stopping fire"));
		ClearFocus(EAIFocusPriority::Gameplay);
		IArmedActor::Execute_StopFiring(MyPawn);
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		AUpTimePlayerCharacter* Enemy = Cast<AUpTimePlayerCharacter>(Actor);
		SetEnemy(Enemy);
	}
	else
	{
		SetEnemy(nullptr);
	}
}

AUpTimePlayerCharacter* AEnemyAIController::GetEnemy() const
{
	if (!Blackboard)
	{
		return nullptr;
	}

	return Cast<AUpTimePlayerCharacter>(Blackboard->GetValue<UBlackboardKeyType_Object>(EnemyTargetKey));
}

void AEnemyAIController::SetEnemy(AUpTimePlayerCharacter* Enemy) const
{
	if (!Blackboard || Enemy && !Enemy->IsPlayerControlled())
	{
		return;
	}

	Blackboard->SetValueAsObject(EnemyTargetKey, Enemy);
}
