// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "BaseCharacter.generated.h"

//UENUM()
//enum class EEquipWeaponState : uint8
//{
//	None,
//	None_To_Equip,
//	Equip_To_None,
//	Equip_To_Equip
//};

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
	ABaseCharacter();

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



protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UPlayerUI> UIClass;

	UPROPERTY(EditDefaultsOnly)
	TMap<EWeaponType, class UAnimMontage*> EquipWeaponMontages;

	UPROPERTY()
	UPlayerUI* UI;

	UPROPERTY(Replicated)
	class ABaseWeapon* Unarmed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateUIEquippedWeapon)
	class ABaseWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	class ABaseWeapon* MeleeWeaponSlot;

	UPROPERTY(Replicated)
	class ABaseWeapon* RangedWeaponSlot;

	UPROPERTY()
	class ABaseWeapon* WeaponToUnarm;

	UPROPERTY(BlueprintReadWrite)
	bool bSwappingWeapon;

	UPROPERTY(BlueprintReadWrite)
	class ABaseWeapon* OverlappedWeapon;

	UPROPERTY()
	class UCharacterAnimInstance* CharacterAnimInstance;

private:
	UPROPERTY(VisibleAnywhere)
	class UInteractionComponent* InteractionComponent;





///Functions 
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Interact();

	UFUNCTION(BlueprintCallable)
	void PickUp();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PickUp(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner);
	void Server_PickUp_Implementation(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner);
	bool Server_PickUp_Validate(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner);

	void EquipWeapon(ABaseWeapon* Weapon);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipWeapon(ABaseWeapon* Weapon);
	void Server_EquipWeapon_Implementation(ABaseWeapon* Weapon);
	bool Server_EquipWeapon_Validate(ABaseWeapon* Weapon);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_WeaponEquip(ABaseWeapon* Weapon);
	void Multicast_WeaponEquip_Implementation(ABaseWeapon* Weapon);

	UFUNCTION(BlueprintCallable)
	void HandleEquip();

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

	//void SwapWeapon();
	//void DrawWeapon();
	void DetermineWeaponControlInput();

	void WeaponSlot_1();
	void WeaponSlot_2();
	ABaseWeapon* DetermineWeaponToArm(ABaseWeapon* Weapon);

	UFUNCTION()
	void OnRep_UpdateUIEquippedWeapon();

	ABaseWeapon* SpawnPickedUpWeapon(struct FWeaponData Data, AActor* WeaponOwner, ABaseWeapon* OldWeapon);
};
