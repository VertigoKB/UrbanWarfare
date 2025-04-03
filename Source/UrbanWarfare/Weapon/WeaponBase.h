// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "WeaponBase.generated.h"

UCLASS()
class URBANWARFARE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline EWeaponType GetWeaponType() const { return WeaponType; }

	UFUNCTION()
	void OnWeaponBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

private:
	void SetupComponentsCollision();

protected:
	UPROPERTY()
	TObjectPtr<class USceneComponent> WeaponRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponType WeaponType = EWeaponType::UnArmed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponItem WeaponItem = EWeaponItem::None;

};
