// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCBTDecorator_InRange.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"

UVCBTDecorator_InRange::UVCBTDecorator_InRange()
	:RangeToCheck(300.f)
{
	NodeName = "In Range";

	TargetKey.AddObjectFilter(this, FName(TEXT("Target")), AActor::StaticClass());
	TargetKey.AddVectorFilter(this, FName(TEXT("Target")));
}

bool UVCBTDecorator_InRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController * AIController = OwnerComp.GetAIOwner();
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(BlackboardComp && AIController && AIController->GetPawn())
	{
		FVector TargetLocation;
		bool IsTargetValid = false;
		const int TargetKeyID = BlackboardComp->GetKeyID(TargetKey.SelectedKeyName);
		
		if(UBlackboardKeyType_Object::StaticClass() == BlackboardComp->GetKeyType(TargetKeyID))
		{
			AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(TargetKeyID));
			if(TargetActor)
			{
				TargetLocation = TargetActor->GetActorLocation();
				IsTargetValid = true;
			}
			
		}
		else if(UBlackboardKeyType_Vector::StaticClass() == BlackboardComp->GetKeyType(TargetKeyID))
		{
			TargetLocation = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(TargetKeyID);
			IsTargetValid = true;
		}
		
		if(IsTargetValid)
		{
			const float Distance = FVector::Distance(AIController->GetPawn()->GetActorLocation(), TargetLocation);
			if(Distance <= RangeToCheck)
			{
				return true;
			}
		}
	}

	return false;
}

#if WITH_EDITOR
void UVCBTDecorator_InRange::BuildDescription()
{
	const FString InverseCondition = IsInversed() ? TEXT("NOT In") : TEXT("In");
	CachedDescription = FString::Printf(TEXT("%s %s Range: %i"), 
		*TargetKey.SelectedKeyName.ToString(), *InverseCondition, FMath::FloorToInt(RangeToCheck));

}
#endif

