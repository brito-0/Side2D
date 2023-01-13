// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DGhostMob.h"
#include "PaperFlipbookComponent.h"
#include "S2DGhostBoss.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AS2DGhostMob::AS2DGhostMob()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetCharacterType(ES2DCharacterType::NPC);

	// configure ghost stats
	SetCharacterMaxHealth(50.f);
	SetKnockbackModifier(1.f);

	SetMaxMoveSpeed(175.f);
	SetChargeMoveSpeed(725.f);

	GetCharacterMovement()->MaxFlySpeed = GetMaxMoveSpeed();

	GetCapsuleComponent()->SetCapsuleRadius(13.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(24.f);

	GetSprite()->SetRelativeLocation(FVector(0.f, 0.f, 2.f));

	SetCanDash(false);
	SetCanRangedAttack(false);

	SetActorTickInterval(.2f);
}

void AS2DGhostMob::BeginPlay()
{
	Super::BeginPlay();
	

	GhostBoss = Cast<AS2DGhostBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AS2DGhostBoss::StaticClass()));
	
}

void AS2DGhostMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool AS2DGhostMob::CharacterTakeDamage(const float Damage, const FVector KnockbackLoc)
{	
	if (GetCharacterState() == ES2DCharacterState::Dead) return false;

	GetWorldTimerManager().ClearTimer(ResetChargeHandle);
	GetWorldTimerManager().ClearTimer(ChangeInvisibleHandle);
	
	KnockbackOrigin = KnockbackLoc;

	// last phase mob ghost don't take damage
	if (GetCharacterCurrentlyInvincible())
	{
		ChangeCharacterStateCharge();
		return true;
	}

	if (GetCharacterCurrentHealth() - Damage <= 0.f)
	{
		CharacterCurrentHealth = 0.f;
		CharacterDeath();
		return true;
	}

	CharacterCurrentHealth -= Damage;
	
	return true;
}

void AS2DGhostMob::MobGhostInvincible(const bool Value)
{
	if (!Value) return;

	SetInvincibleTrue();

	GetSprite()->SetFlipbook(GetFbHalo());
	
}

void AS2DGhostMob::CharacterDeath()
{
	AS2DGhost::CharacterDeath();

	if (GhostBoss->GetGhostPhase() == 3) GhostBoss->RemovePhase3Mob(this);
}
