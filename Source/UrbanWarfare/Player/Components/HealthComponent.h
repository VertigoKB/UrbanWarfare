
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "HealthComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnUiHealthChange, int8)
DECLARE_DELEGATE_OneParam(FOnUiArmorChange, int8)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URBANWARFARE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	inline float GetCurrentHealth() const { return CurrentHealth; }
	inline float GetCurrentArmor() const { return CurrentArmor; }
	inline float GetMaxHealth() const { return MaxHealth; }
	inline float GetMaxArmor() const { return MaxArmor; }

	void OnDamage(const float InDamage);
protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnRep_UiHealth();
	UFUNCTION()
	void OnRep_UiArmor();

public:
	FOnUiHealthChange OnUiHealthChange;
	FOnUiArmorChange OnUiArmorChange;

private:
	// 서버에서 보유 중임. 클라이언트도 보유 중이지만, 로직에 영향을 주지 않을 예정.
	float CurrentHealth = 0.f;
	float CurrentArmor = 0.f;

	float MaxHealth = 100.f;
	float MaxArmor = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_UiHealth)
	int8 UiHealth = 0;
	UPROPERTY(ReplicatedUsing = OnRep_UiArmor)
	int8 UiArmor = 0;

};
