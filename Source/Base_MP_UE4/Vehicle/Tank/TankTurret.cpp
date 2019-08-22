// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed) {
	auto ClampedRelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
	auto RotationChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto Rotation = RelativeRotation.Yaw + RotationChange;
	SetRelativeRotation(FRotator(0, Rotation, 0));
}