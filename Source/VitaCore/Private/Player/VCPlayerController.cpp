// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCPlayerController.h"

#include "AbilitySystemComponent.h"
#include "AkGameplayStatics.h"
#include "VCHUD_Player.h"
#include "VCHeroCharacter.h"
#include "VCCoreGameMode.h"
#include "VCHUD_CharacterSelection.h"
#include "VCHUD_Game.h"
#include "VCHUD_PauseGame.h"
#include "VCCorePlayerState.h"
#include "VCCheatManager.h"
#include "VCGameInstance.h"
#include "VCCoreGameState.h"
#include "VCGameState.h"

// =================== BASE start ============================

AVCPlayerController::AVCPlayerController()
{
	CheatClass = UVCCheatManager::StaticClass();
	// ----- Audio
	Tag_MuteAbilityFailAkEvent = FGameplayTag::RequestGameplayTag("Audio.UI.MuteAbilityFailed");
}


void AVCPlayerController::PostActorCreated()
{
	Super::PostActorCreated();
}

void AVCPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Cache VC References

	
	EnableCheats();
}

void AVCPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	VCGameState = World->GetGameState<AVCGameState>();
	VCPlayerState = Cast<AVCPlayerState>(PlayerState);

	HandlePlayerLogin();

	UVCGameInstance* MyGameInstance = GetGameInstance<UVCGameInstance>();
	if (MyGameInstance != nullptr) {
		ServerSetPlayerName(MyGameInstance->GetPlayerName());
	}
}

void AVCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Escape Menu
	InputComponent->BindAction("Start", IE_Pressed, this, &AVCPlayerController::ToggleEscapeMenu);
	InputComponent->BindAction<FInputToggleDelegate>("ShowStats", IE_Pressed, this, &AVCPlayerController::ToggleGameStats, true);
	InputComponent->BindAction<FInputToggleDelegate>("ShowStats", IE_Released, this, &AVCPlayerController::ToggleGameStats, false);
	InputComponent->BindAction<FInputToggleDelegate>("ShowCharacterInfo", IE_Pressed, this, &AVCPlayerController::ToggleCharacterInfo, true);
	InputComponent->BindAction<FInputToggleDelegate>("ShowCharacterInfo", IE_Released, this, &AVCPlayerController::ToggleCharacterInfo, false);
}


void AVCPlayerController::HandlePlayerLogin()
{
	if (IsLocalPlayerController()) {
		
		VCGameState->HandlePlayerLogin(this);
	}
}

void AVCPlayerController::HandlePlayerLogout()
{
	
}

void AVCPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	MyCharacter = Cast<AVCCharacter>(InPawn);
	if (MyCharacter)
	{
		HandlePossess(MyCharacter);
	}
}

void AVCPlayerController::OnUnPossess()
{
	if (MyCharacter)
	{
		HandleUnPossess(MyCharacter);
		MyCharacter = nullptr;
	}
	Super::OnUnPossess();

}


void AVCPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	VCPlayerState = Cast<AVCPlayerState>(PlayerState);
}


void AVCPlayerController::OnRep_Pawn()
{
	if(GetPawn() != MyCharacter && MyCharacter)
	{
		HandleUnPossess(MyCharacter);
	}
		
	Super::OnRep_Pawn();
	AVCCharacter* NewCharacter = Cast<AVCCharacter>(GetPawn());
	if(NewCharacter && MyCharacter != NewCharacter)
	{
		MyCharacter = NewCharacter;
		HandlePossess(MyCharacter);
	}
}

void AVCPlayerController::HandlePossess(AVCCharacter* InCharacter)
{

	InCharacter->SetupStatusBar();
	// Register callbacks for failed abilities
	if (IsLocalPlayerController())
	{
		if (InCharacter->GetAbilitySystemComponent())
		{

			InCharacter->GetAbilitySystemComponent()->AbilityFailedCallbacks.AddUObject(this,
				&AVCPlayerController::HandlePawnAbilityFailed);
		}
		AttachHUD<UVCHUD_Player>(PlayerHUDClass, PlayerHUD, false);
		AttachHUD<UVCHUD_PauseGame>(InGameMenuClass, InGameMenu, false, 20);
	}
	OnControlledCharacterChanged.Broadcast(InCharacter);


}

void AVCPlayerController::HandleUnPossess(AVCCharacter* OutCharacter)
{
	if(PlayerHUD)
	{
		PlayerHUD->RemoveFromViewport();
	}
	
	if (MyCharacter->GetAbilitySystemComponent())
	{
		MyCharacter->GetAbilitySystemComponent()->AbilityFailedCallbacks.RemoveAll(this);
	}
	MyCharacter->GetOnLevelChangedDelegate().RemoveAll(this);
	OnControlledCharacterChanged.Broadcast(nullptr);

}


void AVCPlayerController::HandlePawnAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& TagContainer)
{
	// Check if ability contains tag for stopping UI failure audio event 
	if (Ability && Ability->AbilityTags.HasTag(Tag_MuteAbilityFailAkEvent) == false)
	{
		PostLocalAkEvent(AkEvent_AbilityFail);
	}
}


void AVCPlayerController::PostLocalAkEvent(UAkAudioEvent* AudioEvent) const
{
	if (AudioEvent && PlayerCameraManager)
	{
		UAkGameplayStatics::PostEvent(AudioEvent, PlayerCameraManager, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>());
	}
}


void AVCPlayerController::PawnLeavingGame()
{
	// Originally it destroyed the pawn on unpossession
}


void AVCPlayerController::ServerCreateCharacter_Implementation(int CharacterIndex, ETeamType PlayerTeam)
{
	UWorld * World = GetWorld();
	if(World)
	{
		AVCGameMode * MyGameMode = World->GetAuthGameMode<AVCGameMode>();
		if(MyGameMode)
		{
			AVCCharacter* CreatedCharacter = MyGameMode->CreatePlayerCharacter(this, CharacterIndex, PlayerTeam);
			if(CreatedCharacter)
			{
				OnPossess(CreatedCharacter);
			}
		}
	}
}

bool AVCPlayerController::ServerCreateCharacter_Validate(int CharacterIndex, ETeamType PlayerTeam)
{
	return true;
}

// =============== SET PLAYER TEAM
void AVCPlayerController::ServerSetTeam_Implementation(ETeamType PlayerTeam)
{
	UWorld * World = GetWorld();
	if(World != nullptr)
	{
		AVCGameState * MyGameState = World->GetGameState<AVCCoreGameState>();
		AVCGameMode * MyGameMode = World->GetAuthGameMode<AVCCoreGameMode>();
		AVCPlayerState * MyPlayerState = GetPlayerState<AVCCorePlayerState>();
		if(MyPlayerState != nullptr && MyGameState != nullptr  && MyGameMode != nullptr)
		{
			TMap<ETeamType, int> TeamSizes;
			for (APlayerState * pPlayerState : MyGameState->PlayerArray)
			{
				if(MyPlayerState != nullptr)
				{
					int Size = TeamSizes.FindRef(MyPlayerState->GetTeamType()) + 1;
					TeamSizes.Add(MyPlayerState->GetTeamType(), Size);
				}
			}
			//if(TeamSizes.FindRef(PlayerTeam) < MyGameMode->TeamMaxSize)
			//{
				MyPlayerState->SetTeamType(PlayerTeam);
			//}
		}
	}
}

bool AVCPlayerController::ServerSetTeam_Validate(ETeamType PlayerTeam)
{
	return true;
}
// =============== SET PLAYER NAME FROM GAME INSTANCE
void AVCPlayerController::ServerSetPlayerName_Implementation(const FString& NewPlayerName)
{
	AVCPlayerState * MyPlayerState = GetPlayerState<AVCPlayerState>();
	if (MyPlayerState != nullptr)
	{
		MyPlayerState->SetPlayerName(NewPlayerName);
	}
}

bool AVCPlayerController::ServerSetPlayerName_Validate(const FString& NewPlayerName)
{
	return true;
}


// =================== BASE end ============================

void AVCPlayerController::ToggleEscapeMenu()
{
	if(InGameMenu == nullptr)
	{
		return;
	}
	
	if(IsLocalPlayerController() == true)
	{
		if (InGameMenu->IsCurrentlyActive() == true)
		{
			InGameMenu->SetMenuActive(false);
			if(MyCharacter != nullptr)
			{
				MyCharacter->EnableCharacterInput();
			}
		}
		else
		{
			InGameMenu->SetMenuActive(true);
			if (MyCharacter != nullptr)
			{
				MyCharacter->DisableCharacterInput();
			}
		}
	}
}


void AVCPlayerController::ToggleCharacterInfo(bool IsOn)
{
	if (IsLocalPlayerController() && PlayerHUD)
	{

	}
}

void AVCPlayerController::ToggleGameStats(bool IsOn)
{
	// TODO
	// VCGameState->ShowGameStats(bool bIsShowing)
}


void AVCPlayerController::ServerStartMatch_Implementation()
{
	UWorld * World = GetWorld();
	if(World != nullptr)
	{
		AVCCoreGameMode * MyGameMode = World->GetAuthGameMode<AVCCoreGameMode>();
		if(MyGameMode != nullptr)
		{
			MyGameMode->StartVitaCoreMatch();
		}
	}
}

bool AVCPlayerController::ServerStartMatch_Validate()
{
	return true;
}


// =============================== USER INTERFACE ============================

void AVCPlayerController::ShowMissionObjective()
{
	// TODO
	// VCGameState->ShowCurrentObjective();
}

void AVCPlayerController::NotifyPlayerDamage(float Damage)
{
	if(PlayerHUD != nullptr)
	{
		PlayerHUD->NotifyDamage(Damage);
	}
}




