// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/Unarmed.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterStateDelegate);

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

UENUM(BlueprintType)
enum class ECharacterHealthState : uint8
{
	Alive,
	Dead
	// injured?
};

UENUM(BlueprintType)
enum class EManeuverType : uint8
{
	None,
	SideStep,
	Dodge
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

USTRUCT(BlueprintType)
struct FManeuverInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsManeuvering;

	UPROPERTY(BlueprintReadOnly)
	float ManeuverAngle;

	FVector ForwardManeuverDirection;
	FVector RightManeuverDirection;
	float ForwardInputValue;
	float RightInputValue;

	UPROPERTY(BlueprintReadOnly)
	EManeuverType ManeuverType;
};

USTRUCT()
struct FMovementInput
{
	GENERATED_USTRUCT_BODY()

public:
	bool bLeft;
	bool bRight;
	bool bForward;
	bool bBackward;
};

UCLASS(config = Game)
class BASE_MP_UE4_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

///Variables
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(Replicated)
	float AimPitch;

	UPROPERTY(BlueprintReadOnly)
	FManeuverInfo ManeuverInfo;

protected:
	UPROPERTY(EditAnywhere, Category = Setup)
	TSubclassOf<class ABaseWeapon> DefaultWeaponClass = AUnarmed::StaticClass();

	UPROPERTY(Replicated)
	class ABaseWeapon* Unarmed;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EquippedWeapon)
	class ABaseWeapon* EquippedWeapon;
	UPROPERTY(Replicated)
	class ABaseWeapon* MeleeWeaponSlot;
	UPROPERTY(Replicated)
	class ABaseWeapon* RangedWeaponSlot;

	UPROPERTY(BlueprintReadWrite)
	TArray<ABaseWeapon*> OverlappedWeapons;

	UPROPERTY()
	class UCharacterAnimInstance* CharacterAnimInstance;

	UPROPERTY(EditDefaultsOnly)
	float AdditionalAimPitch;

	UPROPERTY(EditDefaultsOnly, Category = Statistics)
	int32 MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category = Delays)
	float DestroyCharacterDeathDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = Delays)
	float LeaveCombatDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = Delays)
	float ResetAttackDelay = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float DodgeSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float SideStepSpeed = 450.f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float MaxCombatWalkSpeed = 300.f;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	int32 CurrentHealth;

	FMovementInput MovementInput;

	float DefaultMaxWalkSpeed;

	UPROPERTY(VisibleAnywhere)
	class UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere)
	class UExitPawnComponent* ExitComponent;

	UPROPERTY(VisibleAnywhere)
	class ULockOnComponent* LockOnComponent;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<class UPlayerUI> UIClass;
	UPROPERTY()
	UPlayerUI* UI;

private:
	UPROPERTY()
	class ABaseWeapon* WeaponToEquip;
	UPROPERTY()
	class ABaseWeapon* WeaponToUnarm;

	FTimerHandle LeaveCombatHandle;
	FTimerHandle IsAttackingHandle;
	
	float PreviousMaxSpeed;

// States
public:
	bool bSwappingWeapon;

	//TODO: Character State... maybe move to a struct?
	//TODO make protected, use getters instead
	UPROPERTY(Replicated)
	bool bJump;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAlive = true;



protected:
	bool bIsAttacking;
	bool bInCombat;





///Functions 

public:
	ABaseCharacter();
	ABaseWeapon* GetEquippedWeapon() { return EquippedWeapon; };
	//void RequestWeaponAnimation();
	void SetIsAttacking(bool Value);
	bool GetInCombat() { return bInCombat; };

	void AddOverlappedWeapon(ABaseWeapon* Weapon) { 
		if (Weapon && OverlappedWeapons.Contains(Weapon) == false) {
			OverlappedWeapons.Add(Weapon);
		}
	};
	void RemoveOverlappedWeapon(ABaseWeapon* Weapon) { 
		if (Weapon && OverlappedWeapons.Contains(Weapon)) {
			OverlappedWeapons.Remove(Weapon);
		}
	};

	ULockOnComponent* GetLockOnComponent() { return LockOnComponent; };

	UCharacterAnimInstance* GetCharacterAnimInstance() { return CharacterAnimInstance; };

	void AttackAnimation();

	void OnStartAttack();
	void OnStopAttack(bool bCancelAnimation = false);

	UFUNCTION(BlueprintCallable)
	float DistanceToCharacter(const ACharacter* OtherCharacter);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCurrentHealth(int32 Value);
	void Server_SetCurrentHealth_Implementation(int32 Value);
	bool Server_SetCurrentHealth_Validate(int32 Value) { return true; };

	UFUNCTION()
	virtual void OnRep_CurrentHealth();

	virtual void OnDeath();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeath(int32 Index);
	void Multicast_OnDeath_Implementation(int32 Index);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AttackAnimation();
	void Server_AttackAnimation_Implementation();
	bool Server_AttackAnimation_Validate() { return true; };

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttackAnimation(int32 Index);
	void Multicast_AttackAnimation_Implementation(int32 Index);

	UFUNCTION(BlueprintNativeEvent)
	void TargetKilled();
	void TargetKilled_Implementation();

	
// Movement
public:
	void OnManeuverEnd();

protected:
	virtual void Jump() override;
	virtual void StopJumping() override;

	void Interact();

	void AddControllerYawInput(float Val) override;
	void AddControllerPitchInput(float Val) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAimPitch(float Value);
	void Server_SetAimPitch_Implementation(float Value) { AimPitch = Value; };
	bool Server_SetAimPitch_Validate(float Value) { return true; }

	void Maneuver(float Angle, EManeuverType ManeuverType, float ForwardInputValue, float RightInputValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Maneuver(float Angle, EManeuverType ManeuverType);
	void Server_Maneuver_Implementation(float Angle, EManeuverType ManeuverType);
	bool Server_Maneuver_Validate(float Angle, EManeuverType ManeuverType) { return true; };
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Maneuver(float Angle, EManeuverType ManeuverType);
	void Multicast_Maneuver_Implementation(float Angle, EManeuverType ManeuverType);

	void SideStepForward();
	void SideStepBackward();
	void SideStepLeft();
	void SideStepRight();

	void Dodge();
	void DodgeFreeCamera();
	void DodgeLockedCamera();

// PickUp
protected:
	UFUNCTION(BlueprintCallable)
	void PickUp();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PickUp(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner, bool EquipOnPickup = false);
	void Server_PickUp_Implementation(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner, bool EquipOnPickup = false);
	bool Server_PickUp_Validate(ABaseWeapon* WeaponToPickup, AActor* WeaponOwner, bool EquipOnPickup = false) { return true; };

private:
	ABaseWeapon* SpawnPickedUpWeapon(struct FWeaponData Data, AActor* WeaponOwner, ABaseWeapon* OldWeapon);

// Weapon Equip
public:
	void EquipWeapon();
	void UnequipWeapon();

protected:
	void SpawnDefaultWeapon();

	void WeaponSlot_1();
	void WeaponSlot_2();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetUnarmed();
	void Server_SetUnarmed_Implementation();
	bool Server_SetUnarmed_Validate() { return true; }

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetDefaultWeapon();
	void Server_SetDefaultWeapon_Implementation();
	bool Server_SetDefaultWeapon_Validate() { return true; }

	void StartEquipWeapon(ABaseWeapon* Weapon, bool UseAnimation = true);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartEquipWeapon(ABaseWeapon* Weapon, bool UseAnimation = true);
	void Server_StartEquipWeapon_Implementation(ABaseWeapon* Weapon, bool UseAnimation = true);
	bool Server_StartEquipWeapon_Validate(ABaseWeapon* Weapon, bool UseAnimation = true) { return true; };

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_WeaponEquip(EEquipWeaponState State, bool UseAnimation);
	void Multicast_WeaponEquip_Implementation(EEquipWeaponState State, bool UseAnimation);

	UFUNCTION()
	virtual void OnRep_EquippedWeapon();

	void DetermineWeaponControlInput(bool Combat);

private:
	ABaseWeapon* DetermineWeaponToArm(ABaseWeapon* Weapon);
	EEquipWeaponState DetermineEquipWeaponState(ABaseWeapon* Weapon);


public:
	void Fire();

// Combat, Attacking
protected:
	void EnterCombat();
	void ResetCombatTimer();
	void OnLeaveCombat();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

// Utility
private:	
	FVector GetForwardDirection();
	FVector GetRightDirection();

	void UpdateUI();




};
