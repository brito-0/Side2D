// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S2DGhost.h"
#include "S2DGhostMob.generated.h"

class AS2DGhostBoss;

/**
 * 
 */
UCLASS()
class SIDE2D_API AS2DGhostMob : public AS2DGhost
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS2DGhostMob();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * sets the variable bInvincible to true and changes the current flipbook
	 */
	virtual void MobGhostInvincible(bool Value) override;

	virtual void CharacterDeath() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual bool CharacterTakeDamage(float Damage, FVector KnockbackLoc) override;

private:
	UPROPERTY(VisibleAnywhere)
	AS2DGhostBoss* GhostBoss;
	
};
