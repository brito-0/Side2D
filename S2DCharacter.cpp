// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "S2DProjectile.h"

// Sets default values
AS2DCharacter::AS2DCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f));
	
	SetActorTickInterval(0.f);

}

// Called when the game starts or when spawned
void AS2DCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetCharacterMaxHealth() > 1000.f) SetCharacterMaxHealth(1000.f);
	CharacterCurrentHealth = GetCharacterMaxHealth();

	if (GetCharacterType() == ES2DCharacterType::NPC)
	{
		// UE_LOG(LogTemp, Warning, TEXT("collision"));
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AS2DCharacter::OnBeginOverlap);
	}
	else if (GetCharacterType() == ES2DCharacterType::Trigger) GetCharacterMovement()->SetMovementMode(MOVE_None);
	
}

// Called every frame
void AS2DCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AS2DCharacter::SetCharacterState(ES2DCharacterState State)
{
	if (CharacterState == ES2DCharacterState::Dead) return;

	GetWorldTimerManager().ClearTimer(ReturnStateNormal);

	switch (State)
	{
	case ES2DCharacterState::Normal:
		{
			CharacterState = ES2DCharacterState::Normal;
			
			UE_LOG(LogTemp, Warning, TEXT("%s State -> Normal"), *GetName());
			break;
		}
		
	case ES2DCharacterState::Jump:
		{
			CharacterState = ES2DCharacterState::Jump;
			
			UE_LOG(LogTemp, Warning, TEXT("%s State -> Jump"), *GetName());
			break;
		}
		
	case ES2DCharacterState::Dash:
		{
			CharacterState = ES2DCharacterState::Dash;

			if (CharacterInvincibleAvailable)
			{
				SetInvincibleAvailableFalse();
				
				SetInvincibleTrue();

				FTimerHandle CharacterStopInvincibilityHandle;
				GetWorldTimerManager().SetTimer(CharacterStopInvincibilityHandle, this, &AS2DCharacter::SetInvincibleFalse, .15, false);

				FTimerHandle CharacterInvincibilityAvailableHandle;
				GetWorldTimerManager().SetTimer(CharacterInvincibilityAvailableHandle, this, &AS2DCharacter::SetInvincibleAvailableTrue, 1.f, false);
			}
			
			// FTimerHandle CharacterStopDashHandle;
			GetWorldTimerManager().SetTimer(ReturnStateNormal, this, &AS2DCharacter::ChangeCharacterStateNormal, .2f, false);
			
			UE_LOG(LogTemp, Warning, TEXT("%s State -> Dash"), *GetName());
			break;
		}

	case ES2DCharacterState::Attack:
		{
			CharacterState = ES2DCharacterState::Attack;

			// FTimerHandle CharacterStopAttackHandle;
			GetWorldTimerManager().SetTimer(ReturnStateNormal, this, &AS2DCharacter::ChangeCharacterStateNormal, .3f, false);
			
			UE_LOG(LogTemp, Warning, TEXT("%s State -> Attack"), *GetName());
			break;
		}

	case ES2DCharacterState::RangedAttack:
		{
			CharacterState = ES2DCharacterState::RangedAttack;

			RangedCooldown = true;
			FTimerHandle RangedCooldownHandle;
			GetWorldTimerManager().SetTimer(RangedCooldownHandle, this, &AS2DCharacter::SetRangedCooldownFalse, 1.f, false);
			
			GetWorldTimerManager().SetTimer(ReturnStateNormal, this, &AS2DCharacter::ChangeCharacterStateNormal, .5f, false);
			
			UE_LOG(LogTemp, Warning, TEXT("%s State -> Ranged Attack"), *GetName());
			break;
		}

	case ES2DCharacterState::Stagger:
		{
			StopNPCMovement();
			
			CharacterKnockback(KnockbackOrigin);
			
			CharacterState = ES2DCharacterState::Stagger;

			if (GetCharacterType() == ES2DCharacterType::Player)
			{
				SetInvincibleTrue();

				FTimerHandle CharacterStopInvincibilityHandle;
				GetWorldTimerManager().SetTimer(CharacterStopInvincibilityHandle, this, &AS2DCharacter::SetInvincibleFalse, StaggerInvincibilityDuration, false);
			}
			
			// FTimerHandle CharacterStopStaggerHandle;
			GetWorldTimerManager().SetTimer(ReturnStateNormal, this, &AS2DCharacter::ChangeCharacterStateNormal, .2f, false);
			
			UE_LOG(LogTemp, Warning, TEXT("%s State -> Stagger"), *GetName());
			break;
		}

	case ES2DCharacterState::Follow:
		{
			CharacterState = ES2DCharacterState::Follow;

			CharacterFollow();

			UE_LOG(LogTemp, Warning, TEXT("%s State -> Follow"), *GetName());
			break;
		}

	case ES2DCharacterState::Charge:
		{
			CharacterState = ES2DCharacterState::Charge;

			CharacterCharge();

			UE_LOG(LogTemp, Warning, TEXT("%s State -> Charge"), *GetName());
			break;
		}

	case ES2DCharacterState::Invisible:
		{
			CharacterState = ES2DCharacterState::Invisible;
			
			CharacterInvisible();

			UE_LOG(LogTemp, Warning, TEXT("%s State -> Invisible"), *GetName());
			break;
		}

	case ES2DCharacterState::Wait:
		{
			CharacterState = ES2DCharacterState::Wait;
			
			CharacterWait();

			UE_LOG(LogTemp, Warning, TEXT("%s State -> Wait"), *GetName());
			break;
		}
		
	case ES2DCharacterState::Dead:
		{
			CharacterState = ES2DCharacterState::Dead;
			
			UE_LOG(LogTemp, Warning, TEXT("%s State -> Dead"), *GetName());
			break;
		}
	}
}


void AS2DCharacter::CharacterDeath()
{
	ChangeCharacterStateDead();

	// CharacterKnockback(KnockbackOrigin);

	// FTimerHandle DisableCollisionHandle;
	// GetWorldTimerManager().SetTimer(DisableCollisionHandle, this, &AS2DCharacter::SetCollisionFalse, 1.5f, false);
	
	// SetActorEnableCollision(false);
}

void AS2DCharacter::CharacterJump()
{
	if (GetCharacterState() != ES2DCharacterState::Normal || !GetCharacterMovement()->IsMovingOnGround()) return;


	// FTimerHandle JumpStateChangeHandle;
	// GetWorldTimerManager().SetTimer(JumpStateChangeHandle, this, &AS2DCharacter::ChangeCharacterStateJump, .1f, false);
	
	ChangeCharacterStateJump();
	
	Jump();
}

void AS2DCharacter::CharacterDash()
{
	if (!CanDash || (GetCharacterState() != ES2DCharacterState::Normal && GetCharacterState() != ES2DCharacterState::Jump)) return;

	ChangeCharacterStateDash();

	FVector Impulse = GetActorForwardVector() * (DashDistance * DashModifier);
	
	Impulse.Y = 0.f;
	Impulse.Z = 0.f;
	
	GetCharacterMovement()->AddImpulse(Impulse, true);
	
}

void AS2DCharacter::CharacterFlipbookManager()
{
	// 
}

void AS2DCharacter::CharacterJumpStateManager()
{
	// 
}


bool AS2DCharacter::CharacterTakeDamage(const float Damage, const FVector KnockbackLoc)
{
	if (CharacterInvincible || GetCharacterState() == ES2DCharacterState::Dead) return false;
	
	KnockbackOrigin = KnockbackLoc;

	if (GetCharacterCurrentHealth() - Damage <= 0.f)
	{
		CharacterCurrentHealth = 0.f;
		CharacterDeath();
		return true;
	}

	CharacterCurrentHealth -= Damage;
	ChangeCharacterStateStagger();
	return true;
	
}

void AS2DCharacter::CharacterKnockback(FVector KnockbackLoc)
{
	if (GetCharacterState() == ES2DCharacterState::Stagger) return;

	FVector Direction;

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		// reduce force of impact if the character is grounded and the knockback location is bellow it
		if (KnockbackLoc.Z > GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()/2.f)
			Direction = UKismetMathLibrary::FindLookAtRotation(KnockbackLoc, GetActorLocation()).Vector() * (KnockbackDistance * KnockbackModifier);
		else
			Direction = UKismetMathLibrary::FindLookAtRotation(KnockbackLoc, GetActorLocation()).Vector() * (KnockbackDistance * (KnockbackModifier * .25));
	} // reduce force of impact if the character is in the air
	else
	{
		if (KnockbackLoc.Z < GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()/2.f)
			Direction = UKismetMathLibrary::FindLookAtRotation(KnockbackLoc, GetActorLocation()).Vector() * (KnockbackDistance * (KnockbackModifier * .4f));
		else if (KnockbackLoc.Z > GetActorLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight()/2.f)
			Direction = UKismetMathLibrary::FindLookAtRotation(KnockbackLoc, GetActorLocation()).Vector() * (KnockbackDistance * (KnockbackModifier * .4f));
		else
			Direction = UKismetMathLibrary::FindLookAtRotation(KnockbackLoc, GetActorLocation()).Vector() * (KnockbackDistance * (KnockbackModifier * .55));
	}

	GetCharacterMovement()->AddImpulse(Direction, true);
}

void AS2DCharacter::CharacterAttack()
{
	if (GetCharacterState() != ES2DCharacterState::Normal && GetCharacterState() != ES2DCharacterState::Jump) return;

	ChangeCharacterStateAttack();

	// FTimerHandle AttackDelay;
	// GetWorldTimerManager().SetTimer(AttackDelay, this, &AS2DCharacter::CharacterAttackDelay, GetAttackDelayTime(), false);

	const FVector AttackPos = GetActorLocation() + GetActorForwardVector() * AttackOffset;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_WorldDynamic)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_PhysicsBody)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_Pawn)));
	const TArray<AActor*> IgnoreActor = {this};
	TArray<AActor*> OutActors;

	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), AttackPos, AttackSize, 12, FColor::Emerald, 2.f);
	
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), AttackPos, AttackSize, ObjectTypes, AS2DCharacter::StaticClass(), IgnoreActor, OutActors))
	{
		UE_LOG(LogTemp, Warning, TEXT("CHARACTER HIT"));
		for (AActor* CharacterHit : OutActors)
		{
			if (CharacterHit) Cast<AS2DCharacter>(CharacterHit)->CharacterTakeDamage(AttackDamage, AttackPos);
		}
	}
	
}

void AS2DCharacter::CharacterAttackDelay()
{
	const FVector AttackPos = GetActorLocation() + GetActorForwardVector() * AttackOffset;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_WorldDynamic)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_PhysicsBody)));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType((ECollisionChannel::ECC_Pawn)));
	const TArray<AActor*> IgnoreActor = {this};
	TArray<AActor*> OutActors;

	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), AttackPos, AttackSize, 12, FColor::Emerald, 2.f);
	
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), AttackPos, AttackSize, ObjectTypes, AS2DCharacter::StaticClass(), IgnoreActor, OutActors))
	{
		UE_LOG(LogTemp, Warning, TEXT("CHARACTER HIT"));
		for (AActor* CharacterHit : OutActors)
		{
			if (CharacterHit) Cast<AS2DCharacter>(CharacterHit)->CharacterTakeDamage(AttackDamage, AttackPos);
		}
	}
}

void AS2DCharacter::CharacterRangedAttack()
{
	if (!CanRangedAttack || (GetCharacterState() != ES2DCharacterState::Normal && GetCharacterState() != ES2DCharacterState::Jump) || RangedCooldown) return;

	ChangeCharacterStateRangedAttack();

	if (!SkeletonProjectileObject) return;

	// spawn projectile in the map at a certain location
	SkeletonProjectile = GetWorld()->SpawnActor<AS2DProjectile>(SkeletonProjectileObject, ProjectileLocation, FRotator(0.f, 0.f, 0.f));

	// fire projectile
	SkeletonProjectile->FireProjectile(this, GetActorForwardVector(), GetActorLocation());
	
}

void AS2DCharacter::SetInvincibleTrue()
{
	CharacterInvincible = true;

	// SetActorEnableCollision(false);
}

void AS2DCharacter::SetInvincibleFalse()
{
	CharacterInvincible = false;

	// SetActorEnableCollision(true);
}

void AS2DCharacter::StopNPCMovement()
{
	//
}


void AS2DCharacter::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// stop if other actor isn't AS2DCharacter class or is character state is dead
	if (GetCharacterState() == ES2DCharacterState::Dead || GetbInvisible() || !OtherActor->IsA(AS2DCharacter::StaticClass())) return;

	// UE_LOG(LogTemp, Warning, TEXT("COLLISION HIT"));

	AS2DCharacter* Other = Cast<AS2DCharacter>(OtherActor);

	if (Other->GetCharacterType() != ES2DCharacterType::Player) return;

	GetWorldTimerManager().ClearTimer(ResetCollisionHandle);
	
	Other->CharacterTakeDamage(AttackDamage, GetActorLocation());
	

	GetWorldTimerManager().SetTimer(ResetCollisionHandle, this, &AS2DCharacter::ResetCollisionCapsule, StaggerInvincibilityDuration, false);
	
}

void AS2DCharacter::ResetCollisionCapsule()
{
	SetActorEnableCollision(false);
	SetActorEnableCollision(true);
}


void AS2DCharacter::StartNPCInvisibility()
{
	//	
}

void AS2DCharacter::StopNPCInvisibility()
{
	//
}

void AS2DCharacter::CharacterFollow()
{
	// 
}


void AS2DCharacter::CharacterCharge()
{
	// 
}

void AS2DCharacter::CharacterInvisible()
{
	// 
}

void AS2DCharacter::CharacterWait()
{
	// 
}

bool AS2DCharacter::LineOfSight(AS2DCharacter* Target) const
{
	FHitResult Out;
	FCollisionQueryParams CSQueryP;
	CSQueryP.TraceTag = "TraceTag";
	CSQueryP.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Out, GetActorLocation(), Target->GetActorLocation(), ECollisionChannel::ECC_PhysicsBody, CSQueryP))
	{
		if (Out.GetActor()->IsA(AS2DCharacter::StaticClass()))
		{
			if (Out.GetActor() == Target)
			{
				return true;
			}
		}
	}
	return false;
	
}

void AS2DCharacter::FaceTarget(const AS2DCharacter* Target)
{
	FRotator Rot = GetActorRotation();

	Rot.Yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()).Yaw;
	
	SetActorRotation(Rot);
	
}


// health bar functions

float AS2DCharacter::GetCharacterHealthBar0Value() const
{
	if (GetCharacterCurrentHealth() >= 100.f) return 1.f;
	if (GetCharacterCurrentHealth() == 50.f) return .5f;
	return 0.f; 
	
}

float AS2DCharacter::GetCharacterHealthBar1Value() const
{
	if (GetCharacterCurrentHealth() >= 200.f) return 1.f;
	if (GetCharacterCurrentHealth() == 150.f) return .5f;
	return 0.f; 
	
}

float AS2DCharacter::GetCharacterHealthBar2Value() const
{
	if (GetCharacterCurrentHealth() >= 300.f) return 1.f;
	if (GetCharacterCurrentHealth() == 250.f) return .5f;
	return 0.f; 
	
}

float AS2DCharacter::GetCharacterHealthBar3Value() const
{
	if (GetCharacterCurrentHealth() >= 400.f) return 1.f;
	if (GetCharacterCurrentHealth() == 350.f) return .5f;
	return 0.f; 
	
}

float AS2DCharacter::GetCharacterHealthBar4Value() const
{
	if (GetCharacterCurrentHealth() >= 500.f) return 1.f;
	if (GetCharacterCurrentHealth() == 450.f) return .5f;
	return 0.f; 
	
}

float AS2DCharacter::GetCharacterHealthBar5Value() const
{
	if (GetCharacterCurrentHealth() >= 600.f) return 1.f;
	if (GetCharacterCurrentHealth() == 550.f) return .5f;
	return 0.f; 
	
}

float AS2DCharacter::GetCharacterHealthBar6Value() const
{
	if (GetCharacterCurrentHealth() >= 700.f) return 1.f;
	if (GetCharacterCurrentHealth() == 650.f) return .5f;
	return 0.f; 
	
}

float AS2DCharacter::GetCharacterHealthBar7Value() const
{
	if (GetCharacterCurrentHealth() >= 800.f) return 1.f;
	if (GetCharacterCurrentHealth() == 750.f) return .5f;
	return 0.f; 
	
}

float AS2DCharacter::GetCharacterHealthBar8Value() const
{
	if (GetCharacterCurrentHealth() >= 900.f) return 1.f;
	if (GetCharacterCurrentHealth() == 850.f) return .5f;
	return 0.f; 
	
}

float AS2DCharacter::GetCharacterHealthBar9Value() const
{
	if (GetCharacterCurrentHealth() >= 1000.f) return 1.f;
	if (GetCharacterCurrentHealth() == 950.f) return .5f;
	return 0.f; 
	
}
