// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCBTTask_TryActivateAbility.h"


#include "VCAbilitySystemComponent.h"
#include "VCAbilitySystemInterface.h"
#include "VCAIController.h"

UVCBTTask_TryActivateAbility::UVCBTTask_TryActivateAbility()
	:AbilityID(0)
{
}

EBTNodeResult::Type UVCBTTask_TryActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(VCAIController)
	{
		IVCAbilitySystemInterface* OwnerAbilitySystem = Cast<IVCAbilitySystemInterface>(VCAIController->GetPawn());
		if(OwnerAbilitySystem)
		{
			if(UAbilitySystemComponent * AbilitySystemComponent = OwnerAbilitySystem->GetAbilitySystemComponent())
			{
				FGameplayAbilitySpec* Ability = AbilitySystemComponent->FindAbilitySpecFromInputID(AbilityID);
				if(Ability)
				{
					if(AbilitySystemComponent->TryActivateAbility(Ability->Handle))
					{
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
		
	}
	
	return EBTNodeResult::Failed;

}
