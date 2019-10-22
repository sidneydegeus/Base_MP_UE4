// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

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

	UPROPERTY()
	UPlayerUI* UI;

	UPROPERTY(EditDefaultsOnly, Replicated)
	TArray<struct FWeaponSlot> WeaponSlots;

	//TODO: Rename to more generic later
	UPROPERTY(Replicated)
	class ABaseWeapon* MeleeWeaponSlot;

	UPROPERTY(Replicated)
	class ABaseWeapon* RangedWeaponSlot;

	UPROPERTY()
	class ABaseWeapon* ActiveWeapon;

	//TODO: change to selected weapon??
	UPROPERTY(BlueprintReadOnly, Replicated)
	class ABaseWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadWrite)
	bool bSwappingWeapon;

	UPROPERTY(BlueprintReadWrite)
	class ABaseWeapon* OverlappedWeapon;

private:
	UPROPERTY(VisibleAnywhere)
	class UInteractionComponent* InteractionComponent;

	UPROPERTY()
	class ABaseWeapon* WeaponToEquip;



///Functions 
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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
	void Multicast_WeaponEquip();
	void Multicast_WeaponEquip_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void WeaponEquipEvent();
	virtual void WeaponEquipEvent_Implementation();

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

	void SwapWeapon();
	void DrawWeapon();
	void SetActiveWeapon(ABaseWeapon* Weapon);

	//void ActionBar1();
	//void ActionBar2();

	ABaseWeapon* SpawnPickedUpWeapon(struct FWeaponData Data, AActor* WeaponOwner, ABaseWeapon* OldWeapon);
	//bool FillEmptyWeaponSlot(struct FWeaponData Data);
};
