// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCUserWidget.h"
#include "VCWidget_Indicator.generated.h"

class UImage;
class UCanvasPanelSlot;
class UOverlay;

UCLASS()
class VITACORE_API UVCWidget_Indicator : public UVCUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void SetTargetActor(AActor* Target);

private:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UOverlay* TargetOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UImage* TargetDirectionImage;

	UPROPERTY(EditAnywhere, Category = "Config")
		FVector2D HorizontalLimit;

	UPROPERTY(EditAnywhere, Category = "Config")
		FVector2D VerticalLimit;

	UPROPERTY(EditAnywhere, Category = "Config")
		FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = "Config")
		float TargetHideDistance;
	
	UPROPERTY() UCanvasPanelSlot* TargetOverlaySlot;
	
	UPROPERTY() float InitialTargetDirectionAngle;
	
	UPROPERTY(Transient) AActor* TargetActor;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
