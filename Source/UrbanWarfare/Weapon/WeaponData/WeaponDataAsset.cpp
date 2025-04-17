// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataAsset.h"

void UWeaponDataAsset::InitiallizeValues()
{
	RoundInterval = 60.f / RoundPerMinute;
	//CompressedRPM.SetFloat(RoundPerMinute);
	//CompressedDamage.SetFloat(Damage);
}
