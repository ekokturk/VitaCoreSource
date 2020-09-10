// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCGameInstance.h"

#include "AkGameplayStatics.h"
#include "VCSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"


UVCGameInstance::UVCGameInstance()
	:SettingsSaveSlotName(TEXT("GameSettingsSave"))
{
}

void UVCGameInstance::Init()
{
	Super::Init();
	VitaCoreSettings = GetSettingsSave()->VitaCoreSettings;
	ApplyVideoSettings(VitaCoreSettings.VideoSettings, false);
	ApplySoundSettings(VitaCoreSettings.SoundSettings, false);
	ApplyGameplaySettings(VitaCoreSettings.GameplaySettings, false);
}

void UVCGameInstance::Shutdown()
{
	Super::Shutdown();

}

bool UVCGameInstance::IsLoggedIn() const
{
	return bIsLoggedIn;
}

void UVCGameInstance::SetIsLoggedIn(bool IsLoggedIn)
{
	bIsLoggedIn = IsLoggedIn;
}

FString UVCGameInstance::GetPlayerName() const
{
	return PlayerName;
}


UVCSaveGame* UVCGameInstance::GetSettingsSave()
{
	UVCSaveGame * SettingsSave = Cast<UVCSaveGame>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlotName, 0));
	if (SettingsSave == nullptr)
	{
		SettingsSave = Cast<UVCSaveGame>(UGameplayStatics::CreateSaveGameObject(UVCSaveGame::StaticClass()));
	}
	return SettingsSave;
}

FString UVCGameInstance::GetSettingsSaveSlotName() const
{
	return SettingsSaveSlotName;
}

void UVCGameInstance::ApplyVideoSettings(FVideoSettings VideoSettings, bool ShouldOnlyUpdateOnChange)
{
	if(((VideoSettings.Gamma == VitaCoreSettings.VideoSettings.Gamma) && ShouldOnlyUpdateOnChange == true ) == false )
	{
		GEngine->DisplayGamma = VideoSettings.Gamma * 3.0f + 0.01;
	}
}

void UVCGameInstance::ApplySoundSettings(FSoundSettings SoundSettings, bool ShouldOnlyUpdateOnChange)
{
	APlayerController * LocalPlayerController = GetPrimaryPlayerController();
	if(LocalPlayerController && LocalPlayerController->PlayerCameraManager)
	{
		UAkGameplayStatics::SetRTPCValue(nullptr, SoundSettings.MasterVolume, 0, LocalPlayerController->PlayerCameraManager, "MasterVolume");
		UAkGameplayStatics::SetRTPCValue(nullptr, SoundSettings.MusicVolume, 0, LocalPlayerController->PlayerCameraManager, "MusicVolume");
		UAkGameplayStatics::SetRTPCValue(nullptr, SoundSettings.SFXVolume, 0, LocalPlayerController->PlayerCameraManager, "SFXVolume");
	}
}

void UVCGameInstance::ApplyGameplaySettings(FGameplaySettings GameplaySettings, bool ShouldOnlyUpdateOnChange)
{
	
}

void UVCGameInstance::SaveCurrentSettings()
{
	UVCSaveGame * SettingsSave = GetSettingsSave();
	SettingsSave->VitaCoreSettings = VitaCoreSettings;
	UGameplayStatics::SaveGameToSlot(SettingsSave, SettingsSaveSlotName, 0);
}


void UVCGameInstance::SetPlayerName(FString NewPlayerName)
{
	VitaCoreSettings.Username = NewPlayerName;
	PlayerName = NewPlayerName;
	SaveCurrentSettings();
	
}
