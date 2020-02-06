// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile/BaseProjectile.h"
#include "BaseWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	//TODO: change location of this later? or something with sub categories or something
	Unarmed,
	Melee,
	Ranged,
};


USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<class ABaseWeapon> WeaponBlueprint;

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
	class USceneComponent* RootPlaceholderComponent;
	
	UPROPERTY(EditDefaultsOnly)
	class UPickUpComponent* PickupComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UWidgetComponent* PickupWidget;

	UPROPERTY()
	class UMeshComponent* Mesh;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly)
	FWeaponData WeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponName;

	UPROPERTY(EditDefaultsOnly, Category = WeaponControl)
	bool bInCombat_CharacterCameraOrientation = false;
	UPROPERTY(EditDefaultsOnly, Category = WeaponControl)
	bool bInCombat_CharacterUseControllerRotationYaw = true;


	UPROPERTY(EditDefaultsOnly, Category = WeaponControl)
	bool bOutCombat_CharacterCameraOrientation = true;
	UPROPERTY(EditDefaultsOnly, Category = WeaponControl)
	bool bOutCombat_CharacterUseControllerRotationYaw = false;


public:	
	void ActivateTick(bool bReset);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual void Fire();

	virtual void AimAt(FHitResult HitResult, bool bResultHit);

	void DisablePickUp();

	UFUNCTION(BlueprintCallable)
	void DisplayWeaponName(bool bDisplay);

	//getters / setters
	FWeaponData GetWeaponData() { return WeaponData; };
	void SetWeaponData(FWeaponData NewWeaponData) { WeaponData = NewWeaponData; };

	UFUNCTION(BlueprintCallable)
	EWeaponType GetWeaponType() { return WeaponType; };

	FName GetWeaponName() { return WeaponName; };

	bool GetInCombat_CharacterCameraOrientation() { return bInCombat_CharacterCameraOrientation; };
	bool GetInCombat_CharacterUseControllerRotationYaw() { return bInCombat_CharacterUseControllerRotationYaw; };
	bool GetOutCombat_CharacterCameraOrientation() { return bOutCombat_CharacterCameraOrientation; };
	bool GetOutCombat_CharacterUseControllerRotationYaw() { return bOutCombat_CharacterUseControllerRotationYaw; };



protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	ABaseWeapon();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void FindMesh();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire(FTransform Transform);
	virtual void Server_Fire_Implementation(FTransform Transform);
	virtual bool Server_Fire_Validate(FTransform Transform) { return true; };


	//UFUNCTION(Server, Reliable, WithValidation)
	//void Server_Fire(FTransform Transform);
	//virtual void Server_Fire_Implementation(FTransform Transform);
	//virtual bool Server_Fire_Validate(FTransform Transform) { return true; };

private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DisablePickUp();
	void Multicast_DisablePickUp_Implementation();

};
