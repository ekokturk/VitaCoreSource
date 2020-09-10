// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCWidget_CoreTitan.h"
#include "ProgressBar.h"
#include "TextBlock.h"
#include "VCTitanCharacter.h"
#include "VCHealthAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "VCPlayerController.h"

bool UVCWidget_CoreTitan::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	TitanDistanceMultiplierText = Cast<UTextBlock>(GetWidgetFromName("TitanDistanceMultiplierText"));
	TitanHealthBar = Cast<UProgressBar>(GetWidgetFromName("TitanHealthBar"));

	return true;
}


void UVCWidget_CoreTitan::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AVCCharacter>(VCPlayerController->GetPawn());
	// TODO : Mayble listen to pawn changes?
	if(PlayerCharacter)
	{
		PlayerCharacter->OnCharacterMovementUpdated.AddDynamic(this, &UVCWidget_CoreTitan::HandlePlayerCharacterMoved);
	}
}

void UVCWidget_CoreTitan::NativeDestruct()
{
	Super::NativeDestruct();
	if (PlayerCharacter)
	{
		PlayerCharacter->OnCharacterMovementUpdated.RemoveAll(this);
	}
}


void UVCWidget_CoreTitan::InitTitan(AVCTitanCharacter* TitanCharacter)
{
	MyTitan = TitanCharacter;

	UpdateHealth();
	
	// Notify Max Health ChangeVCHealthAttributeSet
	MyTitan->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UVCHealthAttributeSet::GetMaxHealthAttribute())
		.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData) { UpdateHealth(); });

	// Notify Health ChangeVCHealthAttributeSet
	MyTitan->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(UVCHealthAttributeSet::GetHealthAttribute())
		.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData) { UpdateHealth(); });

}

void UVCWidget_CoreTitan::UpdateHealth() const
{
	TitanHealthBar->SetPercent(MyTitan->GetHealthAttributeSet()->GetHealth() / MyTitan->GetHealthAttributeSet()->GetMaxHealth());
}

void UVCWidget_CoreTitan::HandlePlayerCharacterMoved(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	if (PlayerCharacter)
	{
		//TitanDistanceMultiplierText->SetText(FText::FromString(FString::FromInt(MyTitan->GetDistanceMultiplier(PlayerPawn) * 100)));
	}
}

