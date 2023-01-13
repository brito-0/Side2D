// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DGhost.h"

#include "AIController.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AS2DGhost::AS2DGhost()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetCharacterType(ES2DCharacterType::NPC);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ghost"));
	
	GetCapsuleComponent()->SetEnableGravity(false);
	GetSprite()->SetEnableGravity(false);
	
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	GetCharacterMovement()->DefaultWaterMovementMode = MOVE_Flying;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->BrakingFriction = 75.f;

	SetArenaHeight(1280.f);
	SetArenaWidth(1280.f);
	
	
	SetActorTickInterval(0.2f);
	
}

void AS2DGhost::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	SetNPCController(Cast<AAIController>(GetController()));

	SetPlayerCharacter(Cast<AS2DCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)));
	

	ChangeCharacterStateInvisible();
	
}

void AS2DGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (GetCharacterState() == ES2DCharacterState::Follow) CharacterFollow();
	
}

bool AS2DGhost::CharacterTakeDamage(float Damage, FVector KnockbackLoc)
{
	//
	return false;
}

// void AS2DGhost::CharacterFlipbookManager()
// {
// 	
// }

void AS2DGhost::StartNPCInvisibility()
{
	SetbInvisible(true);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AS2DGhost::StopNPCInvisibility()
{
	SetbInvisible(false);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AS2DGhost::StopNPCMovement()
{
	NPCController->StopMovement();
}

FVector AS2DGhost::RandomLocOutsideArena() const
{
	FVector Location;
	Location.Y = 0.f;

	if (FMath::RandBool())
	{
		if (FMath::RandBool())
			Location.X = (GetArenaWidth()/2)*-1 - 150.f;
		else
			Location.X = GetArenaWidth()/2 + 150.f;

		Location.Z = FMath::RandRange((GetArenaHeight()/2)*-1 - 150.f, GetArenaHeight()/2 + 150.f);
	}
	else
	{
		if (FMath::RandBool())
			Location.Z = (GetArenaHeight()/2)*-1 - 150.f;
		else
			Location.Z = GetArenaHeight()/2 + 150.f;
	}

	return Location;
	
}

FVector AS2DGhost::RandomLocOutsideSidesArena() const
{
	FVector Location;
	Location.Y = 0.f;
	
	if (FMath::RandBool())
		Location.X = (GetArenaWidth()/2)*-1 - 100.f;
	else
		Location.X = GetArenaWidth()/2 + 100.f;
	
	Location.Z = FMath::RandRange((GetArenaHeight()/2)*-1 + 150.f, GetArenaHeight()/2 - 150.f);

	return Location;
	
}

FVector AS2DGhost::RandomLocInsideArena()
{
	FVector Location;
	Location.Y = 0.f;
	
	Location.X = FMath::RandRange((GetArenaWidth()/2)*-1 + 90.f, GetArenaWidth()/2 - 90.f);
	Location.Z = FMath::RandRange((GetArenaHeight()/2)*-1 + 90.f, GetArenaHeight()/2 - 90.f);

	if ((Location - GetPlayerCharacter()->GetActorLocation()).Length() < 150.f)
		return RandomLocInsideArena();
	
	return Location;
	
}

FVector AS2DGhost::RandomLocOutsidePlayerFOV() const
{
	// x - 355.f
	// z - 200.f

	constexpr float XOffSet = 355.f;
	constexpr float ZOffSet = 200.f;
	
	FVector Location;
	Location.Y = 0.f;

	if (FMath::RandBool())
		Location.X = GetPlayerCharacter()->GetActorLocation().X - XOffSet;
	else
		Location.X = GetPlayerCharacter()->GetActorLocation().X + XOffSet;

	Location.Z = FMath::RandRange(GetPlayerCharacter()->GetActorLocation().Z - ZOffSet, GetPlayerCharacter()->GetActorLocation().Z + ZOffSet);

	return Location;
	
}

void AS2DGhost::CharacterCharge()
{	
	GhostCharge();
}

void AS2DGhost::CharacterInvisible()
{
	// stop movement
	// make character invisible for a couple of seconds
	// get new position
	// make the character visible again
	// and after .5 seconds change state to normal (follow???)

	// reset movement speed ???
	

	NPCController->StopMovement();

	StartNPCInvisibility();

	SetActorLocation(RandomLocInsideArena());

	// delay ghost reappearing
	FTimerHandle StopInvisibilityHandle;
	GetWorldTimerManager().SetTimer(StopInvisibilityHandle, this, &AS2DGhost::StopNPCInvisibility, 1.5f, false);

	// delay ghost moving
	FTimerHandle StartFollowHandle;
	GetWorldTimerManager().SetTimer(StartFollowHandle, this, &AS2DGhost::ChangeCharacterStateFollow, 2.f, false);
	
}

void AS2DGhost::CharacterFollow()
{
	if (GetCharacterState() != ES2DCharacterState::Follow) return;

	GetCharacterMovement()->MaxFlySpeed = GetMaxMoveSpeed();

	const FVector Loc = GetPlayerCharacter()->GetActorLocation() + UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetPlayerCharacter()->GetActorLocation()).Vector() * 55.f;
	
	if (NPCController) NPCController->MoveToLocation(Loc, -1.f, false, false, false);

	// change state to invisible after a while
	if (!GetWorldTimerManager().IsTimerActive(ChangeInvisibleHandle))
		GetWorldTimerManager().SetTimer(ChangeInvisibleHandle, this, &AS2DGhost::ChangeCharacterStateInvisible, FMath::RandRange(6.f, 9.f), false);
	
}

void AS2DGhost::GhostCharge()
{
	if (GetCharacterState() != ES2DCharacterState::Charge) return;

	StartNPCInvisibility();
	
	// move ghost outside the arena
	// wait a random amount of time (3-5 seconds ???)
	// ghost charge delay

	NPCController->StopMovement();

	// SetActorLocation(RandomLocOutsideArena());
	// SetActorLocation(RandomLocOutsideSidesArena());

	FTimerHandle ChargeDelayHandle;
	GetWorldTimerManager().SetTimer(ChargeDelayHandle, this, &AS2DGhost::GhostChargeDelay, FMath::RandRange(2.f, 4.f), false);
}

void AS2DGhost::GhostChargeDelay()
{
	GetCharacterMovement()->MaxFlySpeed = ChargeMoveSpeed;

	SetActorLocation(RandomLocOutsidePlayerFOV());

	const FVector ChargeFinal = GetActorLocation() + UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetPlayerCharacter()->GetActorLocation()).Vector() * 1000.f;
	// const FVector ChargeFinal = GetActorLocation() +
		// UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), (GetPlayerCharacter()->GetCharacterMovement()->Velocity * .5f)).Vector() * 1000.f;

	StopNPCInvisibility();
	
	NPCController->MoveToLocation(ChargeFinal, -1, true, false, false);

	GetWorldTimerManager().SetTimer(ChargeDelaySelfDamageHandle, this, &AS2DGhost::ChargeSelfDamage, 3.3f, false);
	
	GetWorldTimerManager().SetTimer(ResetChargeHandle, this, &AS2DGhost::ChangeCharacterStateCharge, 3.5f, false);
}

void AS2DGhost::CharacterWait()
{
	NPCController->StopMovement();

	StartNPCInvisibility();

	SetActorLocation(RandomLocOutsideSidesArena());
}


void AS2DGhost::MobGhostInvincible(bool Value)
{
	// 
}

void AS2DGhost::CharacterDeath()
{
	ChangeCharacterStateDead();

	NPCController->StopMovement();

	
	GetSprite()->SetFlipbook(FbDead);
	
	FTimerHandle DisappearHandle;
	GetWorldTimerManager().SetTimer(DisappearHandle, this, &AS2DGhost::StartNPCInvisibility, GetSprite()->GetFlipbookLength(), false);

	FTimerHandle DestroyHandle;
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AS2DGhost::CharacterDestroy, GetSprite()->GetFlipbookLength(), false);
	
}

void AS2DGhost::ChargeSelfDamage()
{
	CharacterTakeDamage(GetCharacterMaxHealth() * .02, GetActorLocation());
}
