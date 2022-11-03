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
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());
	const auto ArmedActor = Cast<IArmedActor>(EnemyCharacter);

	if (!IArmedActor::Execute_GetWeapon(EnemyCharacter))
	{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan,
				FString::Printf(TEXT("%s has no weapon"), *EnemyCharacter->GetName()));
		return;
	}

	bool bCanAttack = false;
	AUpTimePlayerCharacter* Enemy = GetEnemy();

	if (Enemy)
	{
		if (Enemy->IsAlive())
		{
			if (IArmedActor::Execute_CanFire(EnemyCharacter))
			{
				bCanAttack = LineOfSightTo(Enemy, EnemyCharacter->GetActorLocation());
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

		const float Distance = EnemyCharacter->GetDistanceTo(Enemy);
		if (Distance <= EnemyCharacter->GetAttackRange())
		{
			IArmedActor::Execute_StartFiring(EnemyCharacter);
		}
	}
	else if (ArmedActor->Execute_IsFiring(EnemyCharacter))
	{
		UE_LOG(LogUpTime, Log, TEXT("Cannot attack enemy, clearing focus and stopping fire"));
		ClearFocus(EAIFocusPriority::Gameplay);
		IArmedActor::Execute_StopFiring(EnemyCharacter);
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	UAIPerceptionComponent* MyPerceptionComponent = GetPerceptionComponent();
	if (ensureMsgf(MyPerceptionComponent, TEXT("Perception component not set, cannot intialize target perception updating")))
	{
		MyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		SetEnemy(Cast<AUpTimePlayerCharacter>(Actor));
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
