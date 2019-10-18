// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpComponent.h"

UPickUpComponent::UPickUpComponent() {
	BodyInstance.SetCollisionProfileNameDeferred(FName("Custom"));
	BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BodyInstance.SetObjectType(ECollisionChannel::ECC_WorldDynamic);
	BodyInstance.SetResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BodyInstance.SetResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereRadius = 64;
}