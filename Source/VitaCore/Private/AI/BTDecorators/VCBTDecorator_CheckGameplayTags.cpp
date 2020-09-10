// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCBTDecorator_CheckGameplayTags.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameplayTagAssetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "VCCharacter.h"

UVCBTDecorator_CheckGameplayTags::UVCBTDecorator_CheckGameplayTags()
{
	NodeName = "Check Gameplay Tags";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
	
	// Accept only actors
	ActorToCheck.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UVCBTDecorator_CheckGameplayTags, ActorToCheck), AActor::StaticClass());

	// Default to using Self Actor
	ActorToCheck.SelectedKeyName = FBlackboard::KeySelf;
	
}

bool UVCBTDecorator_CheckGameplayTags::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return false;
	}

	IAbilitySystemInterface* ActorAbilitySystem = Cast<IAbilitySystemInterface>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(ActorToCheck.GetSelectedKeyID()));
	if(ActorAbilitySystem == nullptr || ActorAbilitySystem->GetAbilitySystemComponent() == nullptr)
	{
		return false;
	}

	switch (TagsToMatch)
	{
	case EGameplayContainerMatchType::All:
		return ActorAbilitySystem->GetAbilitySystemComponent()->HasAllMatchingGameplayTags(GameplayTags);

	case EGameplayContainerMatchType::Any:
		return ActorAbilitySystem->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(GameplayTags);

	default:
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Invalid value for TagsToMatch (EGameplayContainerMatchType) %d.  Should only be Any or All."), static_cast<int32>(TagsToMatch));
		return false;
	}
	}
}

void UVCBTDecorator_CheckGameplayTags::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
}

#if WITH_EDITOR

void UVCBTDecorator_CheckGameplayTags::BuildDescription()
{
	CachedDescription = GameplayTags.ToMatchingText(TagsToMatch, IsInversed()).ToString();
}
#endif



void UVCBTDecorator_CheckGameplayTags::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		ActorToCheck.ResolveSelectedKey(*BBAsset);
	}
}

