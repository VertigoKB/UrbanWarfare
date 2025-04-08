// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UrbanWarfare/Common/CommonEnums.h"
#include "DropeedWeapon.generated.h"

UCLASS()
class URBANWARFARE_API ADroppedWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroppedWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnWeaponBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

	// 외부 로직에서 이 클래스의 객체를 스폰 할 경우, 이 함수로 초기화를 진행해야 함.
	bool ExternalInitialize(const uint8 InIdNumber);

private:

	bool InitializePlacedWeapon();

	void SetupComponentsDroppedCollision();

protected:
	UPROPERTY()
	TObjectPtr<class USceneComponent> WeaponRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(EditInstanceOnly)
	uint8 PlacedWeaponInitIdNumber = 0;

	EWeaponType ThisWeaponType = EWeaponType::UnArmed;
	uint8 ThisWeaponIdNumber = 0;

private:
	bool bIsWeaponIdSpecified = false;
};
