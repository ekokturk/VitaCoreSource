// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VCSettingsTypes.h"
#include "VCGameInstance.generated.h"


class UVCSaveGame;

UCLASS()
class VITACORE_API UVCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UVCGameInstance();

protected:

	virtual void Init() override;
	virtual void Shutdown() override;
// =============== LOGIN STATE
public:
	UPROPERTY(BlueprintReadWrite)
		FVitaCoreSettings VitaCoreSettings;
	
	UFUNCTION(BlueprintCallable)
		bool IsLoggedIn() const;

	UFUNCTION(BlueprintCallable)
		void SetIsLoggedIn(bool IsLoggedIn);
protected:
	
	UPROPERTY()
		bool bIsLoggedIn;
	


// ============== SETTINGS
public:
	UFUNCTION(BlueprintCallable)
		UVCSaveGame * GetSettingsSave();

	UFUNCTION(BlueprintCallable)
		FString GetSettingsSaveSlotName() const;

	UFUNCTION(BlueprintCallable)
		void SaveCurrentSettings();

	// ---- Video Settings
	UFUNCTION(BlueprintCallable)
		void ApplyVideoSettings(FVideoSettings VideoSettings, bool ShouldOnlyUpdateOnChange = true);

	// ---- Audio Settings
	UFUNCTION(BlueprintCallable)
		void ApplySoundSettings(FSoundSettings SoundSettings, bool ShouldOnlyUpdateOnChange = true);

	// ---- Gameplay Settings
	UFUNCTION(BlueprintCallable)
		void ApplyGameplaySettings(FGameplaySettings GameplaySettings, bool ShouldOnlyUpdateOnChange = true);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FString SettingsSaveSlotName;

	
// =============== PLAYER NAME
public:
	UFUNCTION(BlueprintCallable)
		FString GetPlayerName() const;
	
	UFUNCTION(BlueprintCallable)
		void SetPlayerName(FString NewPlayerName);

	
protected:
	UPROPERTY()
		FString PlayerName;
	

};
