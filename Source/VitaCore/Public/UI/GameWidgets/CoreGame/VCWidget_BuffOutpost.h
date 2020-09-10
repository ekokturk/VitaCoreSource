// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCUserWidget.h"

#include "VCWidget_BuffOutpost.generated.h"

class AVCBuffOutpost;
class AVCCharacter;
class UProgressBar;

UCLASS()
class VITACORE_API UVCWidget_BuffOutpost : public UVCUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION() void SetOwningOutpost(AVCBuffOutpost * Outpost);
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* CaptureAmountBar;
	
	UPROPERTY() FColor DefaultCaptureAmountColor;
	
	UPROPERTY(Transient) AVCBuffOutpost * MyOutpost;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

