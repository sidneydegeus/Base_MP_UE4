// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EEquipWeaponState : uint8
{
	Unarmed_To_Unarmed,
	Unarmed_To_Melee,
	Unarmed_To_Ranged,

	Melee_To_Melee,
	Melee_To_Unarmed,
	Melee_To_Ranged,

	Ranged_To_Ranged,
	Ranged_To_Unarmed,
	Ranged_To_Melee
};

USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_USTRUCT_BODY()

public:
	//UPROPERTY(EditDefaultsOnly)
	//FName HolsterSocket;

	UPROPERTY()
	class ABaseWeapon* Weapon;
};

UCLASS(config = Game, abstract)
class BASE_MP_UE4_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

///Variables
public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	bool bSwappingWeapon;

	//TODO: Character State... maybe move to a struct?
	//TODO make protected, use getters instead
	UPROPERTY(Replicated)
	bool bJump;

	UPROPERTY(Replicated)
	float AimPitch;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UPlayerUI> UIClass;
	UPROPERTY()
	UPlayerUI* UI;

	UPROPERTY(Replicated)
	class ABaseWeapon* Unarmed;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EquippedWeapon)
	class ABaseWeapon* EquippedWeapon;
	UPROPERTY(Replicated)
	class ABaseWeapon* MeleeWeaponSlot;
	UPROPERTY(Replicated)
	class ABaseWeapon* RangedWeaponSlot;

	UPROPERTY(BlueprintReadWrite)
	class ABaseWeapon* OverlappedWeapon;

	UPROPERTY()
	class UCharacterAnimInstance* CharacterAnimInstance;

	UPROPERTY(EditDefaultsOnly)
	float AdditionalAimPitch;

private:
	UPROPERTY(VisibleAnywhere)
	class UInteractionComponent* InteractionComponent;

	UPROPERTY()
	class ABaseWeapon* WeaponToEquip;
	UPROPERTY()
	class ABaseWeapon* WeaponToUnarm;



///Functions 

public:
	ABaseCharacter();
	ABaseWeapon* GetEquippedWeapon() { return EquippedWeapon; };

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

// Movement
protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	virtual void Jump() override;
	virtual void StopJumping() override;

	void Interact();

// PickUp
protected:
	UFUNCTION(BlueprintCallable)
	void PickUp();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PickUp(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner);
	void Server_PickUp_Implementation(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner);
	bool Server_PickUp_Validate(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner) { return true; };

private:
	ABaseWeapon* SpawnPickedUpWeapon(struct FWeaponData Data, AActor* WeaponOwner, ABaseWeapon* OldWeapon);

// Weapon Equip
public:
	void EquipWeapon();
	void UnequipWeapon();

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetUnarmed();
	void Server_SetUnarmed_Implementation();
	bool Server_SetUnarmed_Validate() { return true; }

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAimPitch(float Value);
	void Server_SetAimPitch_Implementation(float Value) { AimPitch = Value; };
	bool Server_SetAimPitch_Validate(float Value) { return true; }

	void StartEquipWeapon(ABaseWeapon* Weapon);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartEquipWeapon(ABaseWeapon* Weapon);
	void Server_StartEquipWeapon_Implementation(ABaseWeapon* Weapon);
	bool Server_StartEquipWeapon_Validate(ABaseWeapon* Weapon) { return true; };

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_WeaponEquip(EEquipWeaponState State);
	void Multicast_WeaponEquip_Implementation(EEquipWeaponState State);

private:
	ABaseWeapon* DetermineWeaponToArm(ABaseWeapon* Weapon);
	EEquipWeaponState DetermineEquipWeaponState(ABaseWeapon* Weapon);
	void DetermineWeaponControlInput();
	UFUNCTION()
	void OnRep_EquippedWeapon();

// Possession
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	UFUNCTION(Client, Reliable)
	void Client_PossessedBy(APlayerController* PlayerController);
	virtual void Client_PossessedBy_Implementation(APlayerController* PlayerController);

	UFUNCTION(Client, Reliable)
	void Client_UnPossessed();
	virtual void Client_UnPossessed_Implementation();

private:
	void Fire();

	void WeaponSlot_1();
	void WeaponSlot_2();


};
