// Fill out your copyright notice in the Description page of Project Settings.


#include "MontagePauseNotify.h"

void UMontagePauseNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	MeshComp->GetAnimInstance()->Montage_Pause();
}
