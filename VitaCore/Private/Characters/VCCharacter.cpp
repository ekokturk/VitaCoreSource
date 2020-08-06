// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCCharacter.h"

#include "BrainComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "VCAbilitySystemComponent.h"

#include "AkComponent.h"
#include "AkGameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "VCAIController.h"
#include "VCPlayerController.h"
#include "VCHealthAttributeSet.h"
#include "VCTraitAttributeSet.h"
#include "VCGameState.h"
#include "VCSpawner.h"
#include "VCWidgetComponent.h"
#include "VCWidget_StatusBar.h"
#include "VCTeam.h"

#include "VitaCoreLog.h"

AVCCharacter::AVCCharacter()
	:bShouldHideHealthBarOnDeath(true),
	CameraViewType(ECameraViewTypes::Third_Person),
	TeamType(ETeamType::NEUTRAL),
	CharacterLevel(0),
	bShouldUseTeamLevel(true)
{
	// Camera Spring
	CameraSpringComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringComponent"));
	CameraSpringComponent->TargetArmLength = 300.0f;
	CameraSpringComponent->SocketOffset = FVector(0.0f, 70.0f, 50.0f);
	CameraSpringComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CameraSpringComponent->bUsePawnControlRotation = true;
	CameraSpringComponent->SetupAttachment(RootComponent);
	
	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetActive(false);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetupAttachment(CameraSpringComponent, USpringArmComponent::SocketName);
	
	// Ability Component Setup
	AbilitySystemComponent = CreateDefaultSubobject<UVCAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Attribute SetVCHealthAttributeSet
	HealthAttributeSet = CreateDefaultSubobject<UVCHealthAttributeSet>("HealthAttributeSet");
	TraitAttributeSet = CreateDefaultSubobject<UVCTraitAttributeSet>("TraitAttributeSet");


	
	// Status Bar
	StatusBarComponent = CreateDefaultSubobject<UVCWidgetComponent>(TEXT("StatusBarComponent"));
	StatusBarComponent->SetGenerateOverlapEvents(false);
	StatusBarComponent->SetCollisionProfileName("NoCollision");
	StatusBarComponent->SetupAttachment(RootComponent);

	// Ak Component
	AKComponent = CreateDefaultSubobject<UAkComponent>(TEXT("AkComponent"));
	AKComponent->SetupAttachment(GetMesh());

	// Initialization
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
	AbilitySystemComponent->SetIsReplicated(true); // Replicate ability system
	InitialGroundFriction = GetCharacterMovement()->GroundFriction;

	Tag_StatusDead = FGameplayTag::RequestGameplayTag("Status.Dead");
	Tag_StatusInCombat = FGameplayTag::RequestGameplayTag("Status.InCombat");
 
}

void AVCCharacter::PostActorCreated()
{
	Super::PostActorCreated();
	// Called before deffered spawn to cache Game State
	if (UWorld* World = GetWorld())
	{
		VCGameState = World->GetGameState<AVCGameState>();
	}

}

void AVCCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupStatusBar();
	if (HealthAttributeSet != nullptr)
	{
		HealthAttributeSet->OnDamage.AddUObject(this, &AVCCharacter::OnDamage);
	}

	// Initialize Ability System Component
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(Tag_StatusInCombat, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this, [=](const FGameplayTag Tag, int32 Count)
		{
			Count == 0 ? HandleCombatStateChange(false) : HandleCombatStateChange(true);
		});
		// Initialization has to be done both on client and server (Important)
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AbilitySystemComponent->SetMontageRepAnimPositionMethod(ERepAnimPositionMethod::CurrentSectionId);
		if (HasAuthority())
		{
			InitializeAbilities();
			InitializeEffects();
		}
	}
}

void AVCCharacter::PossessedBy(AController* NewController)
{
	MyAIController = Cast<AVCAIController>(NewController);
	MyPlayerController = Cast<AVCPlayerController>(NewController);

	Super::PossessedBy(NewController);
	// Initialize the controller with VitaCore base classes
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}

	OnControllerChanged();


}

void AVCCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	// Cache controllers on client
	MyAIController = Cast<AVCAIController>(Controller);
	MyPlayerController = Cast<AVCPlayerController>(Controller);
	OnControllerChanged();
}

void AVCCharacter::UnPossessed()
{
	Super::UnPossessed();
	// Remove VitaCore controller references
	MyAIController = nullptr;
	MyPlayerController = nullptr;
	OnControllerChanged();
}

void AVCCharacter::OnControllerChanged()
{
	if(MyPlayerController)
	{
		//if (CameraComponent) CameraComponent->Activate();
	}
	else
	{
		//if (CameraComponent) CameraComponent->Deactivate();
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->AbilityFailedCallbacks.RemoveAll(this);
		}
	}

	SetupStatusBar();
}

void AVCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AVCCharacter::DoJump);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVCCharacter::DoMoveVertical);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVCCharacter::DoMoveHorizontal);

	PlayerInputComponent->BindAxis("LookUp", this, &AVCCharacter::DoLookVertical);
	PlayerInputComponent->BindAxis("LookRight", this, &AVCCharacter::DoLookHorizontal);

	AbilitySystemComponent->BindVCAbilityActivationToInputComponent(PlayerInputComponent, AbilityInputBindEnum);
	
}

void AVCCharacter::DoMoveVertical(float InputValue)
{
	if (Controller && InputValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, InputValue);
	}
}

void AVCCharacter::DoMoveHorizontal(float InputValue)
{
	if (Controller && InputValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, InputValue);
	}
}

void AVCCharacter::DoLookVertical(float InputValue)
{
	AddControllerPitchInput(InputValue);
}

void AVCCharacter::DoLookHorizontal(float InputValue)
{
	AddControllerYawInput(InputValue);
}

void AVCCharacter::DoJump()
{
	Jump();
}


FRotator AVCCharacter::GetLookDirection() const
{
	const FVector WorldSpaceDirection = GetBaseAimRotation().Vector();
	const FVector LocalSpaceDirection = ActorToWorld().InverseTransformVectorNoScale(WorldSpaceDirection);
	const FRotator LookLocalSpace = LocalSpaceDirection.Rotation();
	return LookLocalSpace;
}

UCameraComponent* AVCCharacter::GetCameraComponent() const
{
	return CameraComponent;
}

USpringArmComponent* AVCCharacter::GetSpringArmComponent() const
{
	return CameraSpringComponent;
}

AVCAIController* AVCCharacter::GetAIController() const
{
	return MyAIController;
}

AVCPlayerController* AVCCharacter::GetPlayerController() const
{
	return MyPlayerController;
}

void AVCCharacter::SetCameraView(ECameraViewTypes NewCameraViewType)
{
	if(CameraViewProperties.Contains(NewCameraViewType))
	{
		CameraViewType = NewCameraViewType;
		CameraViewProperties[NewCameraViewType].SetCameraView(CameraComponent, CameraSpringComponent);
		return;
	}
	VC_LOG(Error, "CameraViewIndex for the character %s not found!", *GetName())
}

FString AVCCharacter::GetEntityName() const
{
	return FString();
}

void AVCCharacter::SetStatusBarVisibility(bool IsVisible)
{
	if(StatusBarComponent != nullptr)
	{
		StatusBarComponent->SetVisibility(IsVisible);
	}
}

void AVCCharacter::SetupStatusBar()
{
	if(StatusBarComponent)
	{
		UVCWidget_StatusBar* StatusBar = Cast<UVCWidget_StatusBar>(StatusBarComponent->GetUserWidgetObject());
		if (StatusBar != nullptr)
		{
			StatusBar->SetOwningCharacter(this);
		}
	}
}

void AVCCharacter::OnDamage(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth)
{
	if(AKComponent)
	{
		// Play hurt audio
		UAkGameplayStatics::PostEvent(AkEvent_OnDamageHurt, this, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>());
	}
}

// Enable Player or AI Controller
void AVCCharacter::EnableCharacterInput()
{
	// Enable Player Controller
	if (MyPlayerController != nullptr)
	{
		EnableInput(MyPlayerController);
		return;
	}
	// Enable AI Controller
	if (MyAIController != nullptr)
	{
		UBrainComponent * MyBrainComponent = MyAIController->GetBrainComponent();
		if (MyBrainComponent != nullptr)
		{
			MyBrainComponent->RestartLogic();
		}
	}
}

// Disable Player or AI Controller
void AVCCharacter::DisableCharacterInput()
{
	// Disable Player Controllerdo
	if (MyPlayerController != nullptr)
	{
		DisableInput(MyPlayerController);
		return;
	}
	// Disable AI Controller
	if (MyAIController != nullptr)
	{
		UBrainComponent * MyBrainComponent = MyAIController->GetBrainComponent();
		if(MyBrainComponent != nullptr)
		{
			////MyBrainComponent->StopLogic("Stopped");
			//GetCharacterMovement()->StopActiveMovement();
			//GetCharacterMovement()->StopMovementImmediately();
		}
	}
}

void AVCCharacter::HandleCombatStateChange(const bool IsInCombat)
{
	
}

int AVCCharacter::GetCharacterLevel() const
{
	if(bShouldUseTeamLevel && MyTeam)
	{
		return MyTeam->GetTeamLevel();
	}
	return CharacterLevel;
}

void AVCCharacter::SetGroundFriction(bool IsFrictionOn)
{
	UCharacterMovementComponent * MyMovementComponent = GetCharacterMovement();
	
	if(IsFrictionOn == true)
	{
		MyMovementComponent->GroundFriction = InitialGroundFriction;
	}
	else
	{
		MyMovementComponent->GroundFriction = 0.0f;
	}
}

void AVCCharacter::DoStun(float Duration)
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		DisableCharacterInput();
		World->GetTimerManager().SetTimer(TimerHandle_Stun, this, &AVCCharacter::EnableCharacterInput, Duration, false);
	}
}

void AVCCharacter::HandleTeamChange(ETeamType PrevTeamType, ETeamType NewTeamType)
{
	if(VCGameState)
	{
		// leave the old team
		if(MyTeam)
		{
			MyTeam->LeaveTeam(this);
		}

		MyTeam = VCGameState->GetTeam(NewTeamType);

		// Set new team reference and join the team
		if(MyTeam)
		{
			MyTeam->JoinTeam(this);
		}
	}
}

void AVCCharacter::OnRep_TeamType(ETeamType PrevTeamType)
{
	HandleTeamChange(PrevTeamType, TeamType);
	OnTeamChanged.Broadcast(PrevTeamType, TeamType);
}


EAffiliationType AVCCharacter::GetAffiliation(AActor* Other) const
{
	// TODO Make Affiliation mask to check team types
	if (IVCEntityInterface* OtherEntity = Cast<IVCEntityInterface>(Other))
	{
		const ETeamType OtherTeamType = OtherEntity->GetTeamType();
		
		if(MyTeam)
		{
			return MyTeam->GetTeamAffiliation(OtherTeamType);
		}
		
		// TODO Make Affiliation mask to check team types
		if (OtherTeamType == ETeamType::NEUTRAL)
		{
			return EAffiliationType::NEUTRAL;
		}
		return OtherTeamType == GetTeamType() ? EAffiliationType::ALLY : EAffiliationType::ENEMY;
	}
	return EAffiliationType::NEUTRAL;
}

void AVCCharacter::SetTeamType(ETeamType NewTeamType)
{
	const ETeamType OldTeamType = TeamType;

	TeamType = NewTeamType;
	HandleTeamChange(OldTeamType, TeamType);
	OnTeamChanged.Broadcast(OldTeamType, TeamType);
}

bool AVCCharacter::IsAlive() const
{
	return !CharacterDeathInfo.IsDead;
}

void AVCCharacter::DoHit(const FHitInfo& HitInfo)
{
	
}

void AVCCharacter::DoSpawn()
{
	check(HasAuthority())
	
	if(AbilitySystemComponent && SpawnAbility)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(SpawnAbility);
	}

	// Clear Death info
	CharacterDeathInfo = FDeathInfo();
	
	if(MyTeam)
	{
		MyTeam->SpawnCharacter(this);
	}
}

void AVCCharacter::DoDeath(FDeathInfo DeathInfo)
{
	check(HasAuthority())
	
	CharacterDeathInfo = DeathInfo;
	if (AbilitySystemComponent && DeathAbility)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(DeathAbility);
	}
	HandleDeathStateChange(CharacterDeathInfo.IsDead);

	if(MyTeam)
	{
		if(UWorld * World = GetWorld())
		{
			FTimerManager& TimerManager = World->GetTimerManager();
			TimerManager.ClearTimer(TimerHandle_Respawn);
			TimerManager.SetTimer(TimerHandle_Respawn, this, &AVCCharacter::DoSpawn, MyTeam->GetRespawnTime());
		}
	}

	// TODO Should be interface method
	if (AVCCharacter* KillerCharacter = Cast<AVCCharacter>(DeathInfo.KillerActor))
	{
		if (KillerCharacter == this) {
			return;
		}
		const int XP = TraitAttributeSet ? TraitAttributeSet->GetXPReward() : 0.f;
		KillerCharacter->ApplyXP(XP);
	}
	
}

void AVCCharacter::OnRep_DeathInfo(FDeathInfo PrevDeathInfo)
{
	HandleDeathStateChange(CharacterDeathInfo.IsDead);
}

void AVCCharacter::HandleDeathStateChange(bool IsDead)
{
	IsDead ? DisableCharacterInput() : EnableCharacterInput();
	OnDeath.Broadcast(CharacterDeathInfo);
}

FOnEntityDeath& AVCCharacter::GetOnEntityDeathDelegate()
{
	return OnDeath;
}

void AVCCharacter::RefreshAbilities() const
{
	if(AbilityInputBindEnum == nullptr || AbilitySystemComponent == nullptr){
		return;
	}
	
	for(int i = 0; i < AbilityInputBindEnum->NumEnums(); i++)
	{
		if (FGameplayAbilitySpec* Ability = AbilitySystemComponent->FindAbilitySpecFromInputID(i))
		{
			if (Ability->GetPrimaryInstance())
			{
				if (UGameplayEffect* CooldownEffect = Ability->GetPrimaryInstance()->GetCooldownGameplayEffect())
				{
					AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(CooldownEffect->GetClass(), AbilitySystemComponent);
				}
			}
		}
	}
}

UAbilitySystemComponent * AVCCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AVCCharacter::InitializeAbilities() 
{
	if (DeathAbility != nullptr)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DeathAbility.GetDefaultObject(), 1, -1));
	}
	if (SpawnAbility != nullptr)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(SpawnAbility.GetDefaultObject(), 1, -1));
	}
	// Jump to section for replicated montage
}




void AVCCharacter::InitializeEffects()
{
	for (TSubclassOf<UGameplayEffect>& Effect : InitialEffects)
	{
		const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect,
			GetCharacterLevel(), FGameplayEffectContextHandle());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void AVCCharacter::ApplyXP(int XP)
{
	const int PrevLevel = GetCharacterLevel();
	if (bShouldUseTeamLevel)	// If Team level should be used
	{
		if(MyTeam)
		{
			MyTeam->ApplyXP(XP);
		}
	}
	else	// If Character level should be used
	{
		CharacterXP = FMath::Clamp(CharacterXP + XP, 0, CharacterXP + XP);
		int CurrentLevelBracketXP = GetTotalLevelBracketXP(CharacterLevel);
		int PreviousLevelBracketXP = GetTotalLevelBracketXP(CharacterLevel - 1);
		while (CurrentLevelBracketXP <= CharacterXP && IsCharacterLevelMaxed() == false)
		{
			CharacterLevel++;
			CurrentLevelBracketXP = GetTotalLevelBracketXP(CharacterLevel);
		}
		while (PreviousLevelBracketXP > CharacterXP && CharacterLevel > 0)
		{
			CharacterLevel--;
			PreviousLevelBracketXP = GetTotalLevelBracketXP(CharacterLevel - 1);
		}

		CurrentLevelBracketXP = GetTotalLevelBracketXP(CharacterLevel);
		PreviousLevelBracketXP = GetTotalLevelBracketXP(CharacterLevel - 1);
		OnXPChanged.Broadcast(XP, CharacterXP, CharacterXP - PreviousLevelBracketXP, CurrentLevelBracketXP - PreviousLevelBracketXP);
	}

	// If level is changed process and notify
	const int CurrentLevel = GetCharacterLevel();
	if(PrevLevel != CurrentLevel)
	if(PrevLevel != CurrentLevel)
	{
		HandleLevelChange(PrevLevel, CharacterLevel);
		GetOnLevelChangedDelegate().Broadcast(PrevLevel, CharacterLevel);
	}
}

void AVCCharacter::OnRep_CharacterXP(int PrevXP)
{
	// This will be called when team xp is not being used
	const int PreviousLevelBracketXP = GetTotalLevelBracketXP(CharacterLevel - 1);
	const int CurrentLevelBracketXP = GetTotalLevelBracketXP(CharacterLevel);
	OnXPChanged.Broadcast(CharacterXP - PrevXP, CharacterXP, CharacterXP - PreviousLevelBracketXP, CurrentLevelBracketXP - PreviousLevelBracketXP);
}

void AVCCharacter::IncrementLevel()
{
	const int PrevLevel = GetCharacterLevel();
	if (VCGameState)
	{
		if(VCGameState->GetMaxLevel() != PrevLevel)
		{
			const int RequiredXP = GetTotalLevelBracketXP(PrevLevel) - GetCharacterXP();
			ApplyXP(RequiredXP);
		}
	}
	else
	{
		CharacterLevel++;
	}

	const int CurrentLevel = GetCharacterLevel();
	if(CurrentLevel != PrevLevel)
	{
		HandleLevelChange(PrevLevel, CurrentLevel);
		OnLevelChanged.Broadcast(PrevLevel, CurrentLevel);
	}
}

void AVCCharacter::OnRep_CharacterLevel(int PrevLevel)
{
	HandleLevelChange(PrevLevel, CharacterLevel);
	OnLevelChanged.Broadcast(PrevLevel, CharacterLevel);
}

void AVCCharacter::HandleLevelChange(int PrevLevel, int CurrentLevel)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (LevelUpEffect && AbilitySystemComponent)
		{
			const FGameplayEffectSpecHandle LevelEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(LevelUpEffect,
				CurrentLevel, FGameplayEffectContextHandle());
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*LevelEffectSpecHandle.Data.Get());
		}
	}

	if (MyPlayerController && AkEvent_OnLevelUp)
	{
		MyPlayerController->PostLocalAkEvent(AkEvent_OnLevelUp);
	}
}

int AVCCharacter::GetAbilityLevel() const
{
	return GetCharacterLevel();
}

int AVCCharacter::GetCharacterXP() const
{
	if(bShouldUseTeamLevel && MyTeam)
	{
		return MyTeam->GetTeamXP();
	}
	return CharacterXP;
}

int AVCCharacter::GetTotalLevelBracketXP(int Level) const
{
	return VCGameState ? VCGameState->GetLevelBracketTotalXP(Level) : 0;
}

float AVCCharacter::GetCurrentXPRatio() const
{
	const int CurrentLevel = GetCharacterLevel();
	const float CurrentXP = static_cast<float>(GetCharacterXP());
	const float PreviousLevelBracketXP = static_cast<float>(GetTotalLevelBracketXP(CurrentLevel - 1));
	const float CurrentLevelBracketXP = static_cast<float>(GetTotalLevelBracketXP(CurrentLevel));
	return FMath::Clamp((CurrentXP - PreviousLevelBracketXP) / CurrentLevelBracketXP,0.f,1.f);
}

bool AVCCharacter::IsCharacterLevelMaxed() const
{
	if(VCGameState)
	{
		return GetCharacterLevel() >= VCGameState->GetMaxLevel();
	}
	return true;
}

FOnXPChanged& AVCCharacter::GetXPChangedDelegate()
{
	if (bShouldUseTeamLevel && MyTeam)
	{
		return MyTeam->OnXPChanged;
	}
	return OnXPChanged;
}

FOnLevelChanged& AVCCharacter::GetOnLevelChangedDelegate()
{
	if (bShouldUseTeamLevel && MyTeam)
	{
		return MyTeam->OnTeamLevelChanged;
	}
	return OnLevelChanged;
}


UAkComponent* AVCCharacter::GetAkComponent() const
{
	return AKComponent;
}

// ========================== REPLICATION ==========================
void AVCCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	// Dont replicate character level if team level is being used
	DOREPLIFETIME_ACTIVE_OVERRIDE(AVCCharacter, CharacterLevel, !bShouldUseTeamLevel);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AVCCharacter, CharacterXP, !bShouldUseTeamLevel);

}

void AVCCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AVCCharacter, TeamType, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AVCCharacter, CharacterLevel, COND_Custom);
	DOREPLIFETIME_CONDITION(AVCCharacter, CharacterXP, COND_Custom);
	DOREPLIFETIME(AVCCharacter, CharacterDeathInfo);
}

void AVCCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property == nullptr){
		return;
	}

	//const FName PropertyName = PropertyChangedEvent.Property->GetFName();
	//if (PropertyName == GET_MEMBER_NAME_CHECKED(AVCCharacter, CameraViewType))
	//{
	//}
}

