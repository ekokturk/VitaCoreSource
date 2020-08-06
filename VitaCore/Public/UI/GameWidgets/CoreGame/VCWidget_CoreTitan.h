// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCTitanCharacter.h"
#include "VCUserWidget.h"

#include "VCWidget_CoreTitan.generated.h"

class AVCTitanCharacter;
class UTextBlock;
class UProgressBar;

UCLASS()
class VITACORE_API UVCWidget_CoreTitan : public UVCUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION() void InitTitan(AVCTitanCharacter* TitanCharacter);

private:

	UFUNCTION() void UpdateHealth() const;

	UFUNCTION() void HandlePlayerCharacterMoved(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);
	
private:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* TitanDistanceMultiplierText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* TitanHealthBar;

	UPROPERTY(EditAnywhere, Category = "Config")
		ETitanTypes TitanType;
	
	UPROPERTY(Transient) AVCTitanCharacter * MyTitan;

	UPROPERTY(Transient) AVCCharacter * PlayerCharacter;

protected:
	
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};
