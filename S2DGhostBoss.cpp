// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DGhostBoss.h"
#include "PaperFlipbookComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AS2DGhostBoss::AS2DGhostBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetCharacterType(ES2DCharacterType::NPC);

	// configure ghost stats
	SetCharacterMaxHealth(1000.f);
	SetKnockbackModifier(1.2f);
	
	SetMaxMoveSpeed(175.f);
	SetChargeMoveSpeed(700.f);

	GetCharacterMovement()->MaxFlySpeed = GetMaxMoveSpeed();

	GetCapsuleComponent()->SetCapsuleRadius(33.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(60.f);

	GetSprite()->SetRelativeLocation(FVector(0.f, 0.f, 5.f));
	GetSprite()->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));

	SetCanDash(false);
	SetCanRangedAttack(false);

	SetActorTickInterval(.2f);
}

void AS2DGhostBoss::BeginPlay()
{
	Super::BeginPlay();

	if (!MobGhostObject) UE_LOG(LogTemp, Warning, TEXT("GHOST BOSS NO MOB GHOST OBJECT"));

	if (EndWidget) EndMenu = CreateWidget<UUserWidget>(GetWorld(), EndWidget);
	else UE_LOG(LogTemp, Warning, TEXT("GHOSTBOSS - NO END WIDGET"));

	MobGhostsPhase3.Init(nullptr, MobGhostsPhase3Size);
	MobGhostsPhase4.Init(nullptr, MobGhostsPhase4Size);
	
}

void AS2DGhostBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (GetCharacterState() == ES2DCharacterState::Wait && MobGhostsPhase3.IsEmpty())
		CharacterTakeDamage(GetCharacterMaxHealth() * .1f, GetActorLocation());
	
}

bool AS2DGhostBoss::CharacterTakeDamage(const float Damage, const FVector KnockbackLoc)
{
	if (GetCharacterState() == ES2DCharacterState::Dead) return false;

	GetWorldTimerManager().ClearTimer(ResetChargeHandle);
	GetWorldTimerManager().ClearTimer(ChangeInvisibleHandle);
	GetWorldTimerManager().ClearTimer(ChargeDelaySelfDamageHandle);
	
	KnockbackOrigin = KnockbackLoc;

	if (GetCharacterCurrentHealth() - Damage <= 0.f)
	{
		CharacterCurrentHealth = 0.f;
		CharacterDeath();
		return true;
	}

	CharacterCurrentHealth -= Damage;
	
	
	// phase 2
	if (GetCharacterCurrentHealth() <= GetCharacterMaxHealth() * .6f && GhostPhase == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("PHASE 2"));
		++GhostPhase;
		
		ChangeCharacterStateCharge();
		
		return true;
	}
	
	// phase 3
	if (GetCharacterCurrentHealth() <= GetCharacterMaxHealth() * .4f && GhostPhase == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("PHASE 3"));
		++GhostPhase;
	
		ChangeCharacterStateWait();
	
		SpawnPhase3MobGhosts();
		
		return true;
	}
	
	// phase 4
	if (GetCharacterCurrentHealth() <= GetCharacterMaxHealth() * .3f && GhostPhase == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("PHASE 4"));
		++GhostPhase;
	
		GetSprite()->SetFlipbook(GetFbHalo());
	
		ChangeCharacterStateInvisible();
		
		SpawnPhase4MobGhosts();
		
		return true;
	}
	
	if (GetCharacterCurrentHealth() <= GetCharacterMaxHealth() * .05)
	{
		ChangeCharacterStateCharge();
		return true;
	}
	
	
	switch (GhostPhase)
	{
	default: {}
	case 1:
		{
			ChangeCharacterStateInvisible();
			break;
		}
	case 2:
		{
			ChangeCharacterStateCharge();
			break;
		}
	case 3:
		{
			// nothing
			break;
		}
	case 4:
		{
			if (FMath::RandBool()) ChangeCharacterStateInvisible();
			else ChangeCharacterStateCharge();
			break;
		}
	}
	
	return true;
	
}

void AS2DGhostBoss::SpawnPhase3MobGhosts()
{
	// spawn 10 normal mob ghosts

	const FVector Position = FVector(-1000.f, 0.f, 0.f);
	const FRotator Rotation = FRotator(0.f, 0.f, 0.f);

	for (uint8 i = 0; i < MobGhostsPhase3Size; i++)
	{
		MobGhostsPhase3[i] = GetWorld()->SpawnActor<AS2DGhost>(MobGhostObject, Position, Rotation);
	}
	
}

void AS2DGhostBoss::SpawnPhase4MobGhosts()
{
	// spawn 2 mob ghosts with invincibility

	const FVector Position = FVector(-1000.f, 0.f, 0.f);
	const FRotator Rotation = FRotator(0.f, 0.f, 0.f);

	for (uint8 i = 0; i < MobGhostsPhase4Size; i++)
	{
		MobGhostsPhase4[i] = GetWorld()->SpawnActor<AS2DGhost>(MobGhostObject, Position, Rotation);
		MobGhostsPhase4[i]->MobGhostInvincible(true);
	}
	
}

void AS2DGhostBoss::RemovePhase3Mob(AS2DGhost* Mob)
{
	MobGhostsPhase3.Remove(Mob);
}

void AS2DGhostBoss::CharacterDeath()
{
	AS2DGhost::CharacterDeath();

	for (AS2DGhost* Ghost : MobGhostsPhase4) Ghost->CharacterDeath();

	FTimerHandle EndMenuHandle;
	GetWorldTimerManager().SetTimer(EndMenuHandle, this, &AS2DGhostBoss::StartEndMenu, .5f, false);
}


void AS2DGhostBoss::StartEndMenu()
{
	EndMenu->AddToViewport();
		
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}
