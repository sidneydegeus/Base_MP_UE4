// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMP_PlayerController.h"
#include "Engine/World.h"
#include "Vehicle/Tank/TankAimingComponent.h"
#include "BaseMP_PlayerState.h"

void ABaseMP_PlayerController::BeginPlay() {
	Super::BeginPlay();
	State = Cast<ABaseMP_PlayerState>(PlayerState);
	if (State == nullptr) return;
	if (!GetPawn()) return;

	//TODO: remove and cleanup class
	//AimComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	//if (!ensure(TankAimingComponent)) return;
	//FoundAimingComponent(TankAimingComponent);

	State->SetMainCharacter(GetPawn());

}

void ABaseMP_PlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (State != nullptr) {

	}
	//AimTowardsCrosshair();
	//if (AimComponent == nullptr) return;
	//AimComponent->AimAt();
}

//void ABaseMP_PlayerController::AimTowardsCrosshair() {
//	if (AimComponent == nullptr) return;
//	AimComponent->AimAt();
//}
//
//bool ABaseMP_PlayerController::GetSightRayHitLocation(FVector& HitLocation) const {
//	/*int32 ViewportSizeX, ViewportSizeY;
//	GetViewportSize(ViewportSizeX, ViewportSizeY);
//	auto ScreenLocation = FVector2D(
//		ViewportSizeX * CrosshairXLocation,
//		ViewportSizeY * CrosshairYLocation
//	);
//
//	FVector LookDirection;
//	if (GetLookDirection(ScreenLocation, LookDirection)) {
//		return GetLookVectorHitLocation(LookDirection, HitLocation);
//	}*/
//	return false;
//}
//
//bool ABaseMP_PlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
//	FVector CameraWorldLocation; // Not used, but must be passed in
//	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
//}
//
//bool ABaseMP_PlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const {
//	FHitResult HitResult;
//	//auto StartLocation = PlayerCameraManager->GetCameraLocation();
//	//auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
//
//	//FCollisionQueryParams CollisionParams;
//	//CollisionParams.AddIgnoredActor(GetPawn());
//	//bool bLineTraceHit = GetWorld()->LineTraceSingleByChannel(
//	//	HitResult,
//	//	StartLocation,
//	//	EndLocation,
//	//	ECollisionChannel::ECC_Visibility,
//	//	CollisionParams
//	//);
//
//	//if (bLineTraceHit) {
//	//	HitLocation = HitResult.Location;
//	//	//DrawDebugLine(GetWorld(), StartLocation, HitLocation, FColor::Green, false, 1, 0, 1); //
//	//	return true;
//	//}
//	//HitLocation = EndLocation;
//	////DrawDebugLine(GetWorld(), StartLocation, HitLocation, FColor::Red, false, 1, 0, 1); //
//	return false;
//}