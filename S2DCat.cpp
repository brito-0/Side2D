// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DCat.h"
#include "AIController.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AS2DCat::AS2DCat()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetCharacterType(ES2DCharacterType::NPC);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NPC"));

	// configure cat stats
	SetCharacterMaxHealth(150.f);
	SetKnockbackModifier(.4f);
	
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	GetCapsuleComponent()->SetCapsuleRadius(25.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(25.f);

	GetSprite()->SetRelativeLocation(FVector(-5.f, 0.f, 2.f));
	GetSprite()->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	SetCanDash(true);
	SetCanRangedAttack(false);

	SetActorTickInterval(0.2f);
	
}

void AS2DCat::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	SetNPCController(Cast<AAIController>(GetController()));

	SetPlayerCharacter(Cast<AS2DCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)));


	PrevLoc = GetActorLocation();
	
}

void AS2DCat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	CharacterFlipbookManager();

	if (GetCharacterState() == ES2DCharacterState::Normal && GetDistanceTo(GetPlayerCharacter()) < 500.f) ChangeCharacterStateFollow();
	else if (GetCharacterState() == ES2DCharacterState::Follow)
	{
		if (GetDistanceTo(GetPlayerCharacter()) >= 500.f) ChangeCharacterStateNormal();
		else CharacterFollow();
	}
	
}

void AS2DCat::CharacterFollow()
{
	if (GetCharacterState() != ES2DCharacterState::Follow) return;

	FaceTarget(GetPlayerCharacter());

	// if stuck jump
	if (PrevLoc == GetActorLocation()) CatJump();

	if (!NPCController->IsReplicatingMovement()) NPCController->MoveToActor(GetPlayerCharacter(), -1, false, false, true);

	PrevLoc = GetActorLocation();
	
}

void AS2DCat::CatJump()
{
	Jump();

	FTimerHandle JumpDashHandle;
	GetWorldTimerManager().SetTimer(JumpDashHandle, this, &AS2DCat::CatDash, .2f, false);
	
}

void AS2DCat::CatDash()
{
	FVector Impulse = GetActorForwardVector() * 250.f;
	Impulse.Y = 0.f;
	Impulse.Z = 0.f;
	
	GetCharacterMovement()->AddImpulse(Impulse, true);
}

void AS2DCat::CharacterFlipbookManager()
{
	if (GetCharacterState() == ES2DCharacterState::Dead) return;
	
	if (GetCharacterMovement()->IsFalling()) GetSprite()->SetFlipbook(FbJump);
	else GetSprite()->SetFlipbook(FbNormal);
	
}

bool AS2DCat::CharacterTakeDamage(const float Damage, const FVector KnockbackLoc)
{
	NPCController->StopMovement();
	
	return AS2DCharacter::CharacterTakeDamage(Damage, KnockbackLoc);
	
}


void AS2DCat::CharacterDeath()
{
	AS2DCharacter::CharacterDeath();

	NPCController->StopMovement();

	GetSprite()->SetFlipbook(FbDead);

	FTimerHandle DestroyHandle;
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AS2DCat::DestroyCat, GetSprite()->GetFlipbookLength(), false);
	
}
