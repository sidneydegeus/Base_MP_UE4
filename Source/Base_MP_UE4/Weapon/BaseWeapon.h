// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile/BaseProjectile.h"
#include "BaseWeapon.generated.h"

UENUM()
enum class EWeaponType : uint8
{
	//TODO: change location of this later? or something with sub categories or something
	Unarmed,
	Ranged,
	Melee
};


USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<class ABaseWeapon> WeaponBlueprint;

	UPROPERTY(EditDefaultsOnly)
	FName HolsterSocket;
};

UCLASS(abstract)
class BASE_MP_UE4_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;

protected:
	UPROPERTY()
	class UMeshComponent* Mesh;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly)
	FWeaponData WeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponName;

public:	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	ABaseWeapon();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void ActivateTick(bool bReset);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual void Fire();

	virtual void AimAt(FVector HitLocation);

	FWeaponData GetWeaponData() { return WeaponData; };
	void SetWeaponData(FWeaponData NewWeaponData) { WeaponData = NewWeaponData; };

	void DisablePickUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisplayWeaponName(bool bDisplay);
	void DisplayWeaponName_Implementation(bool bDisplay);

protected:
	void FindMesh();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire();
	virtual void Server_Fire_Implementation();
	virtual bool Server_Fire_Validate();

private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DisablePickUp();
	void Multicast_DisablePickUp_Implementation();
};
