#pragma once


#include "ScriptMacros.h"
#include "UObject/Object.h"
#include "VCSettingsTypes.generated.h"


USTRUCT(BlueprintType)
struct FVideoSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) float Gamma;

	FVideoSettings()
		:Gamma(.75f)
	{}

};

USTRUCT(BlueprintType)
struct FSoundSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) float MasterVolume;
	UPROPERTY(BlueprintReadWrite) float MusicVolume;
	UPROPERTY(BlueprintReadWrite) float SFXVolume;

	FSoundSettings()
		:MasterVolume(1.f),
		MusicVolume(1.f),
		SFXVolume(1.f)
	{}

};

USTRUCT(BlueprintType)
struct FGameplaySettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) float VerticalSensitivity;
	UPROPERTY(BlueprintReadWrite) float HorizontalSensitivity;

	FGameplaySettings()
		:VerticalSensitivity(1.f),
		HorizontalSensitivity(1.f)
	{
		
	}
};

USTRUCT(BlueprintType)
struct FVitaCoreSettings
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)	FString				Username;
	UPROPERTY(BlueprintReadWrite)	FVideoSettings		VideoSettings;
	UPROPERTY(BlueprintReadWrite)	FSoundSettings		SoundSettings;
	UPROPERTY(BlueprintReadWrite)	FGameplaySettings	GameplaySettings;

	FVitaCoreSettings() {}
};
