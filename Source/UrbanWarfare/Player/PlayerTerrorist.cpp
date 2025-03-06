// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTerrorist.h"

APlayerTerrorist::APlayerTerrorist()
{
	if (!MeshConfig)
		UE_LOG(LogTemp, Log, TEXT("[APlayerTerrorist::APlayerTerrorist] MeshConfig is null."));

	if (MeshConfig)
		TheMesh->SetSkeletalMesh(MeshConfig->Terrorist);
}
