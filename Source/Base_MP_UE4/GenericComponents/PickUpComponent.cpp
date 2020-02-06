// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Character/BaseCharacter.h"
#include "Components/WidgetComponent.h"

//TODO: rename to PickUpRadiusComponent
UPickUpComponent::UPickUpComponent(const FObjectInitializer& OI) : Super(OI) {
	BodyInstance.SetCollisionProfileNameDeferred(FName("Custom"));
	BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BodyInstance.SetObjectType(ECollisionChannel::ECC_GameTraceChannel3); // Custom collision "Item"
	BodyInstance.SetResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BodyInstance.SetResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	OnComponentBeginOverlap.AddDynamic(this, &UPickUpComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UPickUpComponent::OnOverlapEnd);
	//BodyInstance.OnBeg

		//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnOverlapBegin);
	//GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnOverlapEnd);

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

void UPickUpComponent::OnOverlapBegin(UPrimitiveComponent* OverlappingComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
	if (Character && Character->IsLocallyControlled()) {
		ABaseWeapon* Weapon = Cast<ABaseWeapon>(GetOwner());
		if (Weapon) {
			Weapon->DisplayWeaponName(true);
			Character->AddOverlappedWeapon(Weapon);
		}
	}
}

void UPickUpComponent::OnOverlapEnd(UPrimitiveComponent* OverlappingComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
	if (Character && Character->IsLocallyControlled()) {
		ABaseWeapon* Weapon = Cast<ABaseWeapon>(GetOwner());
		if (Weapon) {
			Weapon->DisplayWeaponName(false);
			Character->RemoveOverlappedWeapon(Weapon);
		}
	}
}