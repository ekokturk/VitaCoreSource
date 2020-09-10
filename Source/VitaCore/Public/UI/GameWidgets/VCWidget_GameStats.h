// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VCWidget_GameStats.generated.h"

class AVCCoreGameState;

UCLASS()
class VITACORE_API UVCWidget_GameStats : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient) AVCCoreGameState* MyGameState;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


};
