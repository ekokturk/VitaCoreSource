// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "VCHeroCharacter.h"
#include "VCCoreGameState.h"
#include "Engine/Engine.h"
#include "VCSpawner.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "VCCharacter.h"
#include "VCHealthAttributeSet.h"

AVCAIController::AVCAIController()
	:bShouldTargetDamager(true),
	bShouldCheckEnemyCharactersAround(true),
	EnemyCharactersCheckRadius(400.f)
{
	BehaviorComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AVCAIController::UpdateTargets()
{
	if(bShouldCheckEnemyCharactersAround)
	{
		const TArray<AVCCharacter*> CharactersAround = GetCharactersAroundPawn(EnemyCharactersCheckRadius, EAffiliationType::ENEMY);
		for(const auto& CharacterAround :CharactersAround)
		{
			Targets.AddUnique(CharacterAround);
		}
	}

	AActor* NewFocusedTarget = nullptr;
	float MinimumDistance = 10000.f;
	for(const auto& Target : Targets)
	{
		const float DistanceBetween = FVector::Dist(MyCharacter->GetActorLocation(), Target->GetActorLocation());
		if(DistanceBetween < MinimumDistance)
		{
			MinimumDistance = DistanceBetween;
			NewFocusedTarget = Target;
		}
	}

	SetFocusedTarget(NewFocusedTarget);
}


void AVCAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateTargets();
}

void AVCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	MyCharacter = Cast<AVCCharacter>(InPawn);
	if(MyCharacter)
	{
		MyCharacter->GetOnEntityDeathDelegate().AddUObject(this, &AVCAIController::OnPawnDead);
		
		if(MyCharacter->GetHealthAttributeSet())
		{
			// Add callback for damage event
			MyCharacter->GetHealthAttributeSet()->OnDamage.AddUObject(this, &AVCAIController::OnPawnDamaged);
		}
	}
	
	// start the behaviour tree
	if (BehaviorComponent != nullptr && BehaviorTree != nullptr)
	{
		if (BehaviorTree->BlackboardAsset != nullptr)
		{
			BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		}

		TargetKey = "Target";
		BlackboardComponent->SetValueAsObject("SelfActor", InPawn);
		BlackboardComponent->SetValueAsVector("SpawnLocation", InPawn->GetActorLocation());
		
		BehaviorComponent->StartTree(*BehaviorTree);
	}

}

void AVCAIController::OnUnPossess()
{
	Super::OnUnPossess();

	if (MyCharacter)
	{
		// Clear delegates
		MyCharacter->GetOnEntityDeathDelegate().RemoveAll(this);
		
		if(MyCharacter->GetHealthAttributeSet())
		{
			MyCharacter->GetHealthAttributeSet()->OnDamage.RemoveAll(this);
		}
	}
	
	MyCharacter = nullptr;
	
	if(BehaviorComponent != nullptr)
	{
		BehaviorComponent->StopTree();
	}
}

void AVCAIController::SetFocusedTarget(AActor * TargetActor)
{
	// TODO Change to a custom function or improve interpolation
	SetFocus(TargetActor);
	FocusedTarget = TargetActor;
	if (BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsObject(TargetKey, TargetActor);
		//AVCSpawner * OwnSpawner = Cast<AVCSpawner>(GetPawn()->GetOwner());
		//if (OwnSpawner != nullptr)
		//{
		//	BlackboardComponent->SetValueAsObject("Spawner", OwnSpawner);
		//}

	}
}

void AVCAIController::ResetTarget()
{
	ClearFocus(EAIFocusPriority::Gameplay);
	FocusedTarget = nullptr;
	
	if (BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsObject(TargetKey, nullptr);
	}
}

TArray<AVCCharacter*> AVCAIController::GetCharactersAroundPawn(float Radius, EAffiliationType AffiliationType) const
{
	TArray<AVCCharacter*> CharactersAround;
	if(MyCharacter)
	{
		TArray<FOverlapResult> Overlaps;
		const FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECC_Pawn);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyCharacter);
		if (GetWorld()->OverlapMultiByObjectType(Overlaps, MyCharacter->GetActorLocation(),
			FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(Radius), QueryParams))
		{
			for (FOverlapResult& OverlapResult : Overlaps)
			{
				AVCCharacter * OtherCharacter = Cast<AVCCharacter>(OverlapResult.GetActor());
				if(MyCharacter->GetAffiliation(OtherCharacter) == AffiliationType)
				{
					CharactersAround.Emplace(OtherCharacter);
				}
			}
		}
	}
	

	return CharactersAround;

}

void AVCAIController::OnPawnDamaged(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth)
{
	// Add damager to the targets
	if(bShouldTargetDamager)
	{
		Targets.AddUnique(Damager);
	}

	// Update focus target
	
}

void AVCAIController::OnPawnDead(const FDeathInfo)
{
	ResetTarget();
}


