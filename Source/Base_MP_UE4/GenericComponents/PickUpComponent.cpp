// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpComponent.h"
#include "Components/WidgetComponent.h"

UPickUpComponent::UPickUpComponent(const FObjectInitializer& OI) : Super(OI) {
	BodyInstance.SetCollisionProfileNameDeferred(FName("Custom"));
	BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BodyInstance.SetObjectType(ECollisionChannel::ECC_WorldDynamic);
	BodyInstance.SetResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BodyInstance.SetResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

	//PickUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	//PickUpWidgetComponent->RegisterComponent();
	//AttachToComponent(PickUpWidgetComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	//SetupAttachment(PickUpWidgetComponent);
	//SetRelativeLocation(FVector(0, 0, 0));
	//GetOwner()->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);

}

void UPickUpComponent::BeginPlay() {
	//if (GetOwner() == nullptr) return;
	//GetOwner()->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);

}