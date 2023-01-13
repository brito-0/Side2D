// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S2DCharacter.h"
#include "S2DGhost.generated.h"

class UPaperFlipbook;
class AAIController;

/**
 * 
 */
UCLASS()
class SIDE2D_API AS2DGhost : public AS2DCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS2DGhost();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void MobGhostInvincible(bool Value);

	virtual void CharacterDeath() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool CharacterTakeDamage(float Damage, FVector KnockbackLoc) override;

	// virtual void CharacterFlipbookManager() override;

	/**
	 * makes the ghost invisible and turns its collisions off
	 */
	virtual void StartNPCInvisibility() override;
	/**
	 * makes the ghost visible and turns its collisions on
	 */
	virtual void StopNPCInvisibility() override;

	void SetNPCController(AAIController* PController) { NPCController = PController; }
	AAIController* GetNPCController() const { return NPCController; }

	void SetPlayerCharacter(AS2DCharacter* Player) { PlayerCharacter = Player; }
	AS2DCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	virtual void StopNPCMovement() override;

	/**
	 * gets a random location outside the area of the arena, and returns it
	 */
	FVector RandomLocOutsideArena() const;
	/**
	 * gets a random location outside the area of the arena, limited to the sides, and returns it
	 */
	FVector RandomLocOutsideSidesArena() const;
	/**
	 * gets a random location inside the area of the arena, and returns it
	 */
	FVector RandomLocInsideArena();
	/**
	 * gets a random location outside the fov of the player, and returns it
	 */
	FVector RandomLocOutsidePlayerFOV() const;

	/**
	 * follows the player for a couple of seconds, and then changes state to invisible
	 */
	virtual void CharacterFollow() override;
	
	virtual void CharacterCharge() override;

	/**
	 * makes the ghost invisible, moves it to a different place within the arena, and changes state to follow
	 */
	virtual void CharacterInvisible() override;

	/**
	 * makes the ghost invisible and moves it to a random place outside of the arena
	 */
	virtual void CharacterWait() override;

	/**
	 * stops ghost's movement and runs CharacterChargeDelay after a couple of seconds
	 */
	void GhostCharge();
	/**
	 * moves the ghost to a random place outside of the players fov and moves it in the players direction
	 */
	void GhostChargeDelay();

	/**
	 * sets the value of MaxMoveSpeed
	 * @param Value new value
	 */
	void SetMaxMoveSpeed(const float Value) { MaxMoveSpeed = Value; }
	/**
	 * returns the value of MaxMoveSpeed
	 */
	float GetMaxMoveSpeed() const { return MaxMoveSpeed; }
	/**
	 * sets the value of ChargeMoveSpeed
	 * @param Value new value
	 */
	void SetChargeMoveSpeed(const float Value) { ChargeMoveSpeed = Value; }
	/**
	 * returns the value of ChargeMoveSpeed
	 */
	float GetChargeMoveSpeed() const { return ChargeMoveSpeed; }

	UPROPERTY()
	FTimerHandle ResetChargeHandle;

	UPaperFlipbook* GetFbNormal() const { return FbNormal; }
	UPaperFlipbook* GetFbHalo() const { return FbHalo; }
	UPaperFlipbook* GetFbDead() const { return FbDead; }

	FTimerHandle ChangeInvisibleHandle;

	FTimerHandle ChargeDelaySelfDamageHandle;

private:
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbNormal;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbHalo;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbDead;
	
	UPROPERTY(VisibleAnywhere)
	AAIController* NPCController;

	UPROPERTY(VisibleAnywhere)
	AS2DCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere)
	float MaxMoveSpeed = 100.f;
	UPROPERTY(EditAnywhere)
	float ChargeMoveSpeed = 600.f;
	
	
	UPROPERTY(VisibleAnywhere)
	bool bGhostCharged = false;
	UPROPERTY(VisibleAnywhere)
	uint8 ChangeCount = 0;

	
	void ChargeSelfDamage();

};
