// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VCSessionWidget.generated.h"

class UButton;
class UVerticalBox;
class UImage;
class UEditableTextBox;
class UCanvasPanel;
class UOverlay;

UCLASS()
class VITACORE_API UVCSessionWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "RefreshSessionList"), BlueprintCallable)
		void BP_RefreshSessionList();
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Config")
		float RefreshListInterval;

	UPROPERTY( meta = (BindWidget))
		UVerticalBox* SessionListBox;

	UPROPERTY()
		FString GameName;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};
