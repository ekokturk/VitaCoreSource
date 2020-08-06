// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCBTTask_AddGameplayTagToPawn.h"


#include "AbilitySystemComponent.h"
#include "VCAbilitySystemInterface.h"
#include "VCAIController.h"
#include "TimerManager.h"

UVCBTTask_AddGameplayTagToPawn::UVCBTTask_AddGameplayTagToPawn()
	:bShouldRemoveTagAfterAdded(true)
{
}

EBTNodeResult::Type UVCBTTask_AddGameplayTagToPawn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (VCAIController && TimerHandle_Delay.IsValid() == false && Tag_ToBeAdded.IsValid() == true)
	{
		IVCAbilitySystemInterface* OwnerAbilitySystem = Cast<IVCAbilitySystemInterface>(VCAIController->GetPawn());
		if (OwnerAbilitySystem)
		{
			if (UAbilitySystemComponent* AbilitySystemComponent = OwnerAbilitySystem->GetAbilitySystemComponent())
			{
				// Get Random Delay
				const float Delay = FMath::Clamp(FMath::RandRange(RandomDelayInterval.X, RandomDelayInterval.Y),
					0.f,RandomDelayInterval.Y);
				
				if(Delay == 0.f)
				{
					AbilitySystemComponent->AddLooseGameplayTag(Tag_ToBeAdded);
					if(bShouldRemoveTagAfterAdded)
					{
						AbilitySystemComponent->RemoveLooseGameplayTag(Tag_ToBeAdded);
					}
				}
				else if(Delay > 0.f)
				{
					if(UWorld * World = GetWorld())
					{
						FTimerDelegate TimerCallback;
						TimerCallback.BindLambda([=]{
							if(AbilitySystemComponent)
							{
								AbilitySystemComponent->AddLooseGameplayTag(Tag_ToBeAdded);
								if (bShouldRemoveTagAfterAdded)
								{
									AbilitySystemComponent->RemoveLooseGameplayTag(Tag_ToBeAdded);
								}
								TimerHandle_Delay.Invalidate();
							}
						});
						
						World->GetTimerManager().SetTimer(TimerHandle_Delay, TimerCallback, Delay, false);
					}
				}
				return EBTNodeResult::Succeeded;

			}
		}
	}

	return EBTNodeResult::Failed;
}
