// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMP_PlayerController.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

void ABaseMP_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABaseMP_PlayerController::BeginPlay() {
	Super::BeginPlay();
}

void ABaseMP_PlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!IsLocalController()) return;
	AimTowardsCrosshair();
}

void ABaseMP_PlayerController::AimTowardsCrosshair() {
	if (Weapon != nullptr && Weapon->GetWeaponType() == EWeaponType::Ranged) {
		FHitResult HitResult;
		bool bHitResult = GetSightRayHitLocation(HitResult);
		Weapon->AimAt(HitResult, bHitResult);
	}
}

bool ABaseMP_PlayerController::GetSightRayHitLocation(FHitResult& HitResult) const {
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(
		ViewportSizeX * CrosshairXLocation,
		ViewportSizeY * CrosshairYLocation
	);

	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		return GetLookVectorHitLocation(LookDirection, HitResult);
	}
	return false;
}

bool ABaseMP_PlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
	FVector CameraWorldLocation; // Not used, but must be passed in
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}

// TODO: out hitlocation should be hitresult
bool ABaseMP_PlayerController::GetLookVectorHitLocation(FVector LookDirection, FHitResult& HitResult) const {
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetPawn());
	bool bLineTraceHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility,
		CollisionParams
	);

	if (bLineTraceHit) {
		//HitLocation = HitResult.Location;
		DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Green, false, 1, 0, 1); //
		return true;
	}
	//HitLocation = EndLocation;
	//HitResult.Location = EndLocation;
	DrawDebugLine(GetWorld(), StartLocation, HitResult.TraceEnd, FColor::Red, false, 1, 0, 1); //
	return false;
}