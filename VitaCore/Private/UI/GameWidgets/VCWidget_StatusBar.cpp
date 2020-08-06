// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCWidget_StatusBar.h"

#include "ProgressBar.h"
#include "VCEnergyAttributeSet.h"
#include "VCHealthAttributeSet.h"
#include "VCCharacter.h"
#include "VCHeroCharacter.h"
#include "UMG/Public/Components/TextBlock.h"
#include "VCCorePlayerState.h"
#include "VCTeam.h"


bool UVCWidget_StatusBar::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	HealthBar = Cast<UProgressBar>(GetWidgetFromName("HealthBar"));
	CharacterNameText = Cast<UTextBlock>(GetWidgetFromName("CharacterNameText"));
	CharacterLevelText = Cast<UTextBlock>(GetWidgetFromName("CharacterLevelText"));
	
	return true;
}


void UVCWidget_StatusBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVCWidget_StatusBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (MyCharacter == nullptr)
	{
		return;
	}
	
	UVCHealthAttributeSet * Attributes = MyCharacter->GetHealthAttributeSet();

	if (Attributes != nullptr)
	{
		if (HealthBar != nullptr)
		{
			HealthBar->SetPercent(Attributes->GetHealth() / Attributes->GetMaxHealth());
		}
	}

	if(CharacterLevelText)
	{
		CharacterLevelText->SetText(FText::FromString(FString::FromInt(MyCharacter->GetCharacterLevel()+1)));
	}
}

void UVCWidget_StatusBar::SetOwningCharacter(AVCCharacter * OwnerCharacter)
{
	MyCharacter = OwnerCharacter;
	SetHealthBarColor();

	AVCCharacter * OwnerPlayerCharacter = Cast<AVCCharacter>(OwnerCharacter);
	if (OwnerPlayerCharacter)
	{
		AVCPlayerState * MyPlayerState = OwnerPlayerCharacter->GetPlayerState<AVCPlayerState>();
		if (MyPlayerState )
		{
			if (CharacterNameText)
			{
				CharacterNameText->SetText(FText::FromString(MyPlayerState->GetPlayerName()));
			}
		}
	}
}

void UVCWidget_StatusBar::SetHealthBarColor()
{
	AVCCharacter * MyPlayerCharacter = Cast<AVCCharacter>(MyCharacter);
	if(MyPlayerCharacter)
	{
		AVCTeam * MyTeam = MyPlayerCharacter->GetTeam();
		if(MyTeam)
		{
			HealthBar->SetFillColorAndOpacity(MyTeam->GetTeamColor());
		}
	}
}
