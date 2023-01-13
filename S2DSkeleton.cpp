// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DSkeleton.h"
#include "AIController.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AS2DSkeleton::AS2DSkeleton()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetCharacterType(ES2DCharacterType::NPC);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NPC"));
	
	// configure skeleton stats
	SetCharacterMaxHealth(100.f);
	SetKnockbackModifier(0.5f);

	GetCharacterMovement()->MaxWalkSpeed = 0.f;

	GetCapsuleComponent()->SetCapsuleRadius(13.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(22.f);

	GetSprite()->SetRelativeLocation(FVector(0.f, 0.f, 3.8f));
	GetSprite()->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	SetCanDash(false);
	SetCanRangedAttack(true);

	SetActorTickInterval(0.1f);
	
}

void AS2DSkeleton::BeginPlay()
{
	Super::BeginPlay();

	
	// GetWorld()->DebugDrawTraceTag = "TraceTag";

	
	SpawnDefaultController();

	SetNPCController(Cast<AAIController>(GetController()));

	SetPlayerCharacter(Cast<AS2DCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)));

	// selects skeleton type
	if (FMath::RandBool()) SetSkeletonType(ES2DSkeletonType::Clothed);

	ChangeCharacterStateWait();
	
}

void AS2DSkeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (GetCharacterState() == ES2DCharacterState::Wait)
	{
		if (GetDistanceTo(GetPlayerCharacter()) < 345.f && LineOfSight(GetPlayerCharacter()))
		{
			SkeletonWakeUp();
		}
		return;
	}

	CharacterFlipbookManager();

	if (GetCharacterState() == ES2DCharacterState::Normal && LineOfSight(GetPlayerCharacter()))
	{
		FaceTarget(GetPlayerCharacter());
		
		// attack player
		CharacterRangedAttack();
	}
	
}

void AS2DSkeleton::CharacterFlipbookManager()
{
	switch (GetCharacterState())
	{
	default: { break; }
	case ES2DCharacterState::Normal:
		{
			GetSprite()->SetFlipbook(FbUseIdle);
			break;
		}
	case ES2DCharacterState::RangedAttack:
		{
			GetSprite()->SetFlipbook(FbUseAttack);
			break;
		}
	case ES2DCharacterState::Stagger:
		{
			GetSprite()->SetFlipbook(FbUseStagger);
			break;
		}
	}
	
}


void AS2DSkeleton::CharacterDeath()
{
	AS2DCharacter::CharacterDeath();

	GetSprite()->SetFlipbook(FbDead);

	FTimerHandle DestroyHandle;
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AS2DSkeleton::DestroySkeleton, GetSprite()->GetFlipbookLength(), false);
	
}

void AS2DSkeleton::CharacterWait()
{
	//
}

void AS2DSkeleton::SkeletonWakeUp()
{
	switch (GetSkeletonType())
	{
	case ES2DSkeletonType::Normal:
		{
			FbUseIdle = FbNormalIdle;
			FbUseRise = FbNormalRise;
			FbUseAttack = FbNormalAttack;
			FbUseStagger = FbNormalStagger;
			break;
		}
	case ES2DSkeletonType::Clothed:
		{
			FbUseIdle = FbClothedIdle;
			FbUseRise = FbClothedRise;
			FbUseAttack = FbClothedAttack;
			FbUseStagger = FbClothedStagger;
			break;
		}
	}

	GetSprite()->SetFlipbook(FbUseRise);

	FTimerHandle RiseHandle;
	GetWorldTimerManager().SetTimer(RiseHandle, this, &AS2DSkeleton::SkeletonWakeUpDelay, GetSprite()->GetFlipbookLength(), false);
	
}

void AS2DSkeleton::SkeletonWakeUpDelay()
{
	GetSprite()->SetFlipbook(FbUseIdle);
	
	ChangeCharacterStateNormal();
	
}
