// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCUserWidget.h"
#include "VCWidget_StatusBar.generated.h"

class UProgressBar;
class UTextBlock;
class AVCCharacter;

UCLASS()
class VITACORE_API UVCWidget_StatusBar : public UVCUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetOwningCharacter(AVCCharacter* OwnerCharacter);

	UFUNCTION(BlueprintCallable)
		void SetHealthBarColor();

private:
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
		UTextBlock* CharacterNameText;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
		UTextBlock* CharacterLevelText;
	
	UPROPERTY(Transient) AVCCharacter * MyCharacter;

private:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
