// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DProjectile.h"
#include "S2DCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/RendererSettings.h"

AS2DProjectile::AS2DProjectile()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCapsuleRadius(5.f);
	CollisionComponent->SetCapsuleHalfHeight(8.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	
	SetActorTickInterval(0.f);
	
}

void AS2DProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AS2DProjectile::OnBeginOverlap);
	
}

void AS2DProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (GetFired()) MoveProjectile();
	
}

void AS2DProjectile::FireProjectile(AS2DCharacter* OwnerChar, const FVector Direction, FVector Position)
{
	OwnerCharacter = OwnerChar;

	XValue = Direction.X;

	if (XValue == 1.f) Sprite->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	Position.X += 25.f * XValue;

	SetActorLocation(Position);

	SetFired(true);
	
}


void AS2DProjectile::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(AS2DCharacter::StaticClass()))
	{
		DestroyProjectile();
		return;
	} 

	AS2DCharacter* HitCharacter = Cast<AS2DCharacter>(OtherActor);
	
	if (HitCharacter->GetCharacterType() == ES2DCharacterType::Trigger || OwnerCharacter->GetCharacterType() == HitCharacter->GetCharacterType()) return;
	
	HitCharacter->CharacterTakeDamage(AttackDamage, GetActorLocation());
	
	DestroyProjectile();
	
}

void AS2DProjectile::MoveProjectile()
{
	// make projectile move forward
	// FireDirection

	FVector Loc = GetActorLocation();
	
	const float Offset = (GetActorForwardVector().Normalize()*XValue) * (GetWorld()->GetDeltaSeconds() * ProjectileSpeed);

	Loc.X += Offset;

	SetActorLocation(Loc);
	
}
