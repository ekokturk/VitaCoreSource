// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VCMenuContainer.generated.h"

class AVCMenuHttp;
class UButton;
class UEditableTextBox;

UCLASS()
class VITACORE_API UVCMenuContainer : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP")
		AVCMenuHttp * MenuHttp;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	UEditableTextBox * UsernameTextBox;


	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//	FString PlayerName;

	FTimerHandle TimerHandle_Queue;

	//UFUNCTION(BlueprintCallable)
	//	void EnterQueue();

	//UFUNCTION(BlueprintCallable)
	//	void ExitQueue();

};
