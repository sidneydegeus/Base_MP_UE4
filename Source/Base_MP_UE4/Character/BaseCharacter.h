// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FName, ABaseWeapon*> WeaponSlots;

	UPROPERTY(BlueprintReadOnly, Replicated)
	class ABaseWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadWrite)
	bool bSwappingWeapon;



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
	void PickUp(ABaseWeapon* PickedUpWeapon);
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

private:
	void Fire();

	void ActionBar1();
	void ActionBar2();
};
