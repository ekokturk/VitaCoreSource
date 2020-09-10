// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "VCWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = "UserInterface", editinlinenew, meta = (BlueprintSpawnableComponent))
class VITACORE_API UVCWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool bShouldFaceCamera;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitWidget() override;
	UFUNCTION()
		void LookAtLocalCamera();

	UFUNCTION()
		void SetLocalCameramanager();

	UPROPERTY()
		APlayerCameraManager * LocalCameraManager;
	
};
