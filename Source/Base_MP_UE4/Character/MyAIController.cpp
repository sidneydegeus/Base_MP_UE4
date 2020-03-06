// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Character/AICharacter.h"
#include "Character/PlayerCharacter.h"
#include "Waypoint.h"

AMyAIController::AMyAIController() {
	PrimaryActorTick.bCanEverTick = true;
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perfection Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMyAIController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AMyAIController::BeginPlay() {
	Super::BeginPlay();
	PossessedCharacter = Cast<AAICharacter>(GetPawn());
}

void AMyAIController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (!HasAuthority() && PossessedCharacter) return;

	if (!DetectedCharacter) {
		//if (PossessedCharacter && PossessedCharacter->NextWaypoint) {
		//	MoveToActor(PossessedCharacter->NextWaypoint, 5.f);
		//}
		//else {
		//	wander
		//}
		if (!bIsWandering)
			Wander();
		
	}
	else if (DetectedCharacter) {
		bIsWandering = false;
		auto MovementResult = EPathFollowingRequestResult::AlreadyAtGoal;
		if (!DetectedCharacter->bIsAlive) {
			DetectedCharacter = nullptr;
		} else if (MoveToActor(DetectedCharacter, PossessedCharacter->GetEquippedWeapon()->GetAttackRange(), true) == MovementResult) {
			PossessedCharacter->Fire();
		}
	}

}

FRotator AMyAIController::GetControlRotation() const {
	if (GetPawn() == nullptr) {
		return FRotator(0.f, 0.f, 0.f);
	}
	return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.f);
}

void AMyAIController::OnPossess(APawn* Pawn) {
	Super::OnPossess(Pawn);
}

void AMyAIController::OnPawnDetected(const TArray<AActor*> &DetectedPawns) {
	for (size_t i = 0; i < DetectedPawns.Num(); i++) {
		//DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
		APlayerCharacter* Player = Cast<APlayerCharacter>(DetectedPawns[i]);
		if (Player && Player->bIsAlive) {
			DetectedCharacter = Player;
			return;
		}
		else {
			DetectedCharacter = nullptr;
		}
	}
}