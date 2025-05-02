
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "HealthComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnUiHealthChange, int8)
DECLARE_DELEGATE_OneParam(FOnUiArmorChange, int8)

DECLARE_DELEGATE_OneParam(FOnDeathFlagChange, bool)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	inline float GetCurrentHealth() const { return CurrentHealth; }
	inline float GetCurrentArmor() const { return CurrentArmor; }
	inline float GetMaxHealth() const { return MaxHealth; }
	inline float GetMaxArmor() const { return MaxArmor; }
	inline void SetLocalHostFlag() { bIsLocalHost = true; }

	void Server_OnDamage(const float InDamage);
protected:
	virtual void BeginPlay() override;
	
private:
	bool InitConstruct();

	UFUNCTION()
	void OnRep_UiHealth();
	UFUNCTION()
	void OnRep_UiArmor();

	UFUNCTION()
	void OnRep_bIsDeath();

	void Server_OnRespawnQue();

	UFUNCTION(Server, Reliable)
	void Server_TempRespawn();

public:
	FOnUiHealthChange OnUiHealthChange;
	FOnUiArmorChange OnUiArmorChange;

	FOnDeathFlagChange OnDeathFlagChange;
private:
	// 서버에서 보유 중임. 클라이언트도 보유 중이지만, 로직에 영향을 주지 않을 예정.
	float CurrentHealth = 0.f;
	float CurrentArmor = 0.f;

	float MaxHealth = 100.f;
	float MaxArmor = 100.f;

	bool bIsLocalHost = false;

	UPROPERTY(ReplicatedUsing = OnRep_UiHealth)
	int8 UiHealth = 0;
	UPROPERTY(ReplicatedUsing = OnRep_UiArmor)
	int8 UiArmor = 0;

	UPROPERTY(ReplicatedUsing = OnRep_bIsDeath)
	bool bIsDeath = false;

	FTimerHandle Respawnhandle;

	FTimerHandle OnRepDeathHandle;
private:
	UPROPERTY()
	TObjectPtr<class APlayerBase> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class URegisterInputComponent> RegisterInputComponent;
	UPROPERTY()
	TObjectPtr<class UWarfareAnim> FirstAnimInst;
	UPROPERTY()
	TObjectPtr<class UWarfareAnim> ThirdAnimInst;

	bool bIsInitSuccess = false;
	FTimerHandle InitHandle;
};
