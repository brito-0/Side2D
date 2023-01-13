// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "S2DCharacter.generated.h"

class AS2DProjectile;

UENUM()
enum class ES2DCharacterType : uint8
{
	Player,
	NPC,
	Trigger
};

UENUM()
enum class ES2DCharacterState : uint8
{
	Normal,
	Jump,
	Dash,
	Attack,
	RangedAttack,
	Stagger,
	Follow,
	Charge,
	Invisible,
	Wait,
	Dead
};

UCLASS()
class SIDE2D_API AS2DCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS2DCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// // Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	
	//get functions
	UFUNCTION(BlueprintCallable, Category = CharacterType)
	ES2DCharacterType GetCharacterType() const { return CharacterType; }
	UFUNCTION(BlueprintCallable, Category = CharacterState)
	ES2DCharacterState GetCharacterState() const { return CharacterState; }
	UFUNCTION(BlueprintCallable, Category = CharacterState)
	ES2DCharacterState GetPreviousCharacterState() const { return PreviousCharacterState; }
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterMaxHealth() const { return CharacterMaxHealth; }
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterCurrentHealth() const { return CharacterCurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterInvincibleAvailable() const { return CharacterInvincibleAvailable; }
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	bool GetCharacterCurrentlyInvincible() const { return CharacterInvincible; }

	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar0Value() const;
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar1Value() const;
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar2Value() const;
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar3Value() const;
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar4Value() const;
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar5Value() const;
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar6Value() const;
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar7Value() const;
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar8Value() const;
	UFUNCTION(BlueprintCallable, Category = CharacterStats)
	float GetCharacterHealthBar9Value() const;

	float GetArenaHeight() const { return ArenaHeight; }
	float GetArenaWidth() const { return ArenaWidth; }

	/**
	 * decreases the character current health
	 * @param Damage value by which the current health will be decrease
	 * @param KnockbackLoc position of the attack received
	 */
	virtual bool CharacterTakeDamage(float Damage, FVector KnockbackLoc);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	/**
	 * sets the type for the character
	 * @param Type the character type
	 */
	void SetCharacterType(const ES2DCharacterType Type) { CharacterType = Type; }

	/**
	 * changes the current state to the state given 
	 * @param State new state
	 */
	void SetCharacterState(ES2DCharacterState State);

	// functions to change the character state
	void ChangeCharacterStateNormal() { SetCharacterState(ES2DCharacterState::Normal); }
	void ChangeCharacterStateJump() { SetCharacterState(ES2DCharacterState::Jump); }
	void ChangeCharacterStateDash() { SetCharacterState(ES2DCharacterState::Dash); }
	void ChangeCharacterStateAttack() { SetCharacterState(ES2DCharacterState::Attack); }
	void ChangeCharacterStateRangedAttack() { SetCharacterState(ES2DCharacterState::RangedAttack); }
	void ChangeCharacterStateStagger() { SetCharacterState(ES2DCharacterState::Stagger); }
	void ChangeCharacterStateFollow() { SetCharacterState(ES2DCharacterState::Follow); }
	void ChangeCharacterStateCharge() { SetCharacterState(ES2DCharacterState::Charge); }
	void ChangeCharacterStateInvisible() { SetCharacterState(ES2DCharacterState::Invisible); }
	void ChangeCharacterStateWait() { SetCharacterState(ES2DCharacterState::Wait); }
	void ChangeCharacterStateDead() { SetCharacterState(ES2DCharacterState::Dead); }

	UPROPERTY()
	FVector KnockbackOrigin;

	UPROPERTY(VisibleAnywhere)
	float CharacterCurrentHealth = 0.f;

	/**
	 * changes the max health of the character
	 * @param Value new max health value
	 */
	void SetCharacterMaxHealth(const float Value) { CharacterMaxHealth = Value; }

	/**
	 * changes the knockback modifier of the character
	 * @param Value new knockback modifier
	 */
	void SetKnockbackModifier(const float Value) { KnockbackModifier = Value; }

	/**
	 * makes the character jump, and changes state to jump
	 */
	void CharacterJump();

	/**
	 * makes the character dash, and changes the state to dash
	 */
	void CharacterDash();

	/**
	 * ..... , and changes the state to dead
	 */
	virtual void CharacterDeath();

	/**
	 * changes the characters flip book when necessary
	 */
	virtual void CharacterFlipbookManager();

	/**
	 * changes the character state to normal state when the character lands after a jump
	 */
	virtual void CharacterJumpStateManager();

	/**
	 * pushes the character away from a certain position
	 * @param KnockbackLoc position that the character will be pushed away from
	 */
	void CharacterKnockback(FVector KnockbackLoc);

	/**
	 * uses CharacterAttackDelay after a certain amount of time
	 */
	void CharacterAttack();
	/**
	 * creates an overlap and deals damage to characters inside it
	 */
	void CharacterAttackDelay();

	/**
	 * spawns a projectile that will move forwards
	 */
	void CharacterRangedAttack();

	/**
	 * stops npc movement
	 */
	virtual void StopNPCMovement();

	/**
	 * deals damage to characters that enter the collision capsule 
	 */
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	virtual void StartNPCInvisibility();
	virtual void StopNPCInvisibility();

	/**
	 * sets the value of CharacterInvincibleAvailableCooldown
	 * @param Value new value
	 */
	void SetCharacterInvincibleAvailableCooldown(const float Value) { CharacterInvincibleAvailableCooldown = Value; }

	/**
	 * sets the value of ArenaHeight
	 * @param Value new value
	 */
	void SetArenaHeight(const float Value) { ArenaHeight = Value; }
	/**
	 * sets the value of ArenaWidth
	 * @param Value new value
	 */
	void SetArenaWidth(const float Value) { ArenaWidth = Value; }
	
	virtual void CharacterFollow();
	
	virtual void CharacterCharge();
	
	virtual void CharacterInvisible();

	virtual void CharacterWait();

	/**
	 * sets the variable CharacterInvincible to true
	 */
	void SetInvincibleTrue();

	/**
	 * destroys the character
	 */
	void CharacterDestroy() { this->Destroy(); }

	/**
	 * turns the collision of the character off and then on
	 */
	void ResetCollisionCapsule();

	/**
	 * sets the value of bInvisible
	 * @param Value new value
	 */
	void SetbInvisible(const bool Value) { bInvisible = Value; }
	
	/**
	 * returns the value of bInvisible
	 */
	bool GetbInvisible() const { return bInvisible; }

	/**
	 * checks if the character as uninterrupted line of sight to a certain target, and returns the result
	 * @param Target target to check line of sight
	 */
	bool LineOfSight(AS2DCharacter* Target) const;

	/**
	 * makes the character face a certain target
	 * @param Target target to face
	 */
	void FaceTarget(const AS2DCharacter* Target);

	/**
	 * sets the value of CanDash
	 * @param Value new value
	 */
	void SetCanDash(const bool Value) { CanDash = Value; }
	/**
	 * sets the value of CanRangedAttack
	 * @param Value new value
	 */
	void SetCanRangedAttack(const bool Value) { CanDash = Value; }

	/**
	 * sets the value of AttackDelayTime
	 * @param Value new value
	 */
	void SetAttackDelayTime(const float Value) { AttackDelayTime = Value; }
	/**
	 * returns the value of AttackDelayTime
	 */
	float GetAttackDelayTime() const { return AttackDelayTime; }

private:
	// UPROPERTY(VisibleAnywhere)
	UPROPERTY(EditAnywhere)
	ES2DCharacterType CharacterType;

	// character stats
	UPROPERTY(VisibleAnywhere)
	ES2DCharacterState CharacterState;
	UPROPERTY(VisibleAnywhere)
	ES2DCharacterState PreviousCharacterState;

	UPROPERTY(VisibleAnywhere)
	bool CharacterInvincible = false;

	// void SetInvincibleTrue();
	/**
	 * sets the variable CharacterInvincible to false 
	 */
	void SetInvincibleFalse();

	UPROPERTY(VisibleAnywhere)
	bool CharacterInvincibleAvailable = true;

	UPROPERTY(EditAnywhere)
	float CharacterInvincibleAvailableCooldown = 2.f;

	/**
	 * sets the variable CharacterInvincibleAvailable to true
	 */
	void SetInvincibleAvailableTrue() { CharacterInvincibleAvailable = true; }
	/**
	 * sets the variable CharacterInvincibleAvailable to false
	 */
	void SetInvincibleAvailableFalse() { CharacterInvincibleAvailable = false; }

	/**
	 * turns character collision off
	 */
	void SetCollisionFalse() { SetActorEnableCollision(false); }
	
	UPROPERTY(EditAnywhere)
	bool CanDash = false;
	UPROPERTY(EditAnywhere)
	bool CanRangedAttack = false;
	UPROPERTY(VisibleAnywhere)
	bool CanDoubleJump = false;
	UPROPERTY(VisibleAnywhere)
	bool CanWallJump = false;
	

	/**
	 * max 1000
	 */
	UPROPERTY(EditAnywhere)
	float CharacterMaxHealth = 100.f;
	// UPROPERTY(VisibleAnywhere)
	// float CharacterCurrentHealth = 0.f;

	UPROPERTY(EditAnywhere)
	float AttackDamage = 50.f;
	
	UPROPERTY(VisibleAnywhere)
	float KnockbackDistance = 1250.f;
	UPROPERTY(EditAnywhere)
	float KnockbackModifier = 1.f;

	UPROPERTY(VisibleAnywhere)
	float DashDistance = 2250.f;
	UPROPERTY(EditAnywhere)
	float DashModifier = 1.f;

	UPROPERTY(VisibleAnywhere)
	float StaggerInvincibilityDuration = .7f;

	UPROPERTY(EditAnywhere)
	float AttackOffset = 30.f;
	UPROPERTY(EditAnywhere)
	float AttackSize = 17.5f;

	FTimerHandle ReturnStateNormal;

	UPROPERTY(VisibleAnywhere)
	float ArenaHeight = 1280.f;
	UPROPERTY(VisibleAnywhere)
	float ArenaWidth = 1280.f;

	FTimerHandle ResetCollisionHandle;

	UPROPERTY(VisibleAnywhere)
	bool bInvisible = false;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<AS2DProjectile> SkeletonProjectileObject;

	UPROPERTY()
	AS2DProjectile* SkeletonProjectile;

	UPROPERTY()
	FVector ProjectileLocation = FVector(-5000.f, 0.f, 0.f);

	UPROPERTY(VisibleAnywhere)
	bool RangedCooldown = false;

	/**
	 * sets the variable RangedCooldown to false
	 */
	void SetRangedCooldownFalse() { RangedCooldown = false; }

	UPROPERTY(EditAnywhere)
	float AttackDelayTime = .1f;

	// UPROPERTY()
	// UCapsuleComponent* CapsuleCollider;
};
