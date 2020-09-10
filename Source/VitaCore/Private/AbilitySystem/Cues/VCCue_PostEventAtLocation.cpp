// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCCue_PostEventAtLocation.h"

#include "AkGameplayStatics.h"

UVCCue_PostEventAtLocation::UVCCue_PostEventAtLocation()
{
}

bool UVCCue_PostEventAtLocation::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const FVector TargetLocation = MyTarget->GetActorLocation();
	UAkGameplayStatics::PostEventAtLocation(AkEvent, TargetLocation, FRotator::ZeroRotator, FString(TEXT("")), MyTarget);

	return true;

}