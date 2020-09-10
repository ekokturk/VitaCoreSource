// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCBTDecorator.h"
#include "VCBTDecorator_CheckGameplayTags.generated.h"


class UBehaviorTree;


UCLASS(meta = (DisplayName = "VC Check Gameplay Tags"))
class VITACORE_API UVCBTDecorator_CheckGameplayTags : public UVCBTDecorator
{
	GENERATED_BODY()

public:
	UVCBTDecorator_CheckGameplayTags();

protected:
	UPROPERTY(EditAnywhere, Category = GameplayTagCheck)
		struct FBlackboardKeySelector ActorToCheck;

	UPROPERTY(EditAnywhere, Category = GameplayTagCheck)
		EGameplayContainerMatchType TagsToMatch;

	UPROPERTY(EditAnywhere, Category = GameplayTagCheck)
		FGameplayTagContainer GameplayTags;
	
private:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	
#if WITH_EDITOR
		virtual void BuildDescription() override;
#endif
	
	
};
