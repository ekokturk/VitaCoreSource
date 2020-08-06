// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCUserWidget.h"


#include "UMGSequencePlayer.h"
#include "VCGameState.h"
#include "VCPlayerController.h"
#include "VCPlayerState.h"


bool UVCUserWidget::Initialize()
{
	if(Super::Initialize() == false){
		return false;
	}

	bIsWidgetActive = false;
	bShouldActivateWidgetOnConstruct = true;
	bShouldChangeActiveStateWithAnimation = true;
	bShouldRemoveFromViewportOnDeactivation = true;
	
	return true;
}

void UVCUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	VCGameState = GetWorld()->GetGameState<AVCGameState>();
	VCPlayerController = Cast<AVCPlayerController>(GetPlayerContext().GetPlayerController());
	check(VCPlayerController)
	SetWidgetActive(bShouldActivateWidgetOnConstruct);
}

void UVCUserWidget::RemoveFromParent()
{
	OnRemovedFromViewport.Broadcast();
	Super::RemoveFromParent();

}

void UVCUserWidget::SetWidgetActive(bool IsActive)
{
	if(IsActive == bIsWidgetActive){
		return;
	}
	
	if(bShouldChangeActiveStateWithAnimation)
	{
		UWidgetAnimation* Animation = IsActive ? Animation_WidgetActivation : Animation_WidgetDeactivation;
		if(Animation)
		{
			PlayAnimation(Animation)->OnSequenceFinishedPlaying().AddWeakLambda(this, 
		[=](UUMGSequencePlayer& SequencePlayer){
					bIsWidgetActive = IsActive;
					bIsWidgetActive ? HandleWidgetActivated() : HandleWidgetDeactivated();
				});
			return;
		}
	}
	
	bIsWidgetActive = IsActive;
	bIsWidgetActive ? HandleWidgetActivated() : HandleWidgetDeactivated();
}

void UVCUserWidget::HandleWidgetActivated()
{
	BP_OnWidgetActivated();
	OnWidgetActivated.Broadcast(this);
}

void UVCUserWidget::HandleWidgetDeactivated()
{
	BP_OnWidgetDeactivated();
	OnWidgetDeactivated.Broadcast(this);
	if (bShouldRemoveFromViewportOnDeactivation)
	{
		RemoveFromViewport();
	}
}
