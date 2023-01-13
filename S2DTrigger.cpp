// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DTrigger.h"

#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AS2DTrigger::AS2DTrigger()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetCharacterType(ES2DCharacterType::Trigger);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ghost"));

	// configure trigger stats
	SetCharacterMaxHealth(1.f);
	SetKnockbackModifier(0.f);
	
	GetCapsuleComponent()->SetEnableGravity(false);
	GetSprite()->SetEnableGravity(false);
	
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_None;
	GetCharacterMovement()->DefaultWaterMovementMode = MOVE_None;
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	GetCapsuleComponent()->SetCapsuleRadius(11.5f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(16.5f);

	GetSprite()->SetRelativeLocation(FVector(-1.f, 0.f, 0.f));
	GetSprite()->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	GetSprite()->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	
}

void AS2DTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// void AS2DTrigger::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// 	
// }

void AS2DTrigger::CharacterDeath()
{
	// hide door
	DoorTileMap->SetActorHiddenInGame(true);
	DoorTileMap->SetActorEnableCollision(false);

	GetSprite()->SetFlipbook(FbDead);

	FTimerHandle DestroyHandle;
	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AS2DTrigger::DestroyTrigger, GetSprite()->GetFlipbookLength(), false);
}

void AS2DTrigger::DestroyTrigger()
{
	DoorTileMap = nullptr;

	this->Destroy();
	
}
