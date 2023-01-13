// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S2DCharacter.h"
#include "S2DCat.generated.h"

class UPaperFlipbook;
class AAIController;

/**
 * 
 */
UCLASS()
class SIDE2D_API AS2DCat : public AS2DCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS2DCat();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void CharacterDeath() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SetNPCController(AAIController* PController) { NPCController = PController; }
	AAIController* GetNPCController() const { return NPCController; }

	void SetPlayerCharacter(AS2DCharacter* Player) { PlayerCharacter = Player; }
	AS2DCharacter* GetPlayerCharacter() const { return PlayerCharacter; }
	
	virtual bool CharacterTakeDamage(float Damage, FVector KnockbackLoc) override;

	virtual void CharacterFlipbookManager() override;

	virtual void CharacterFollow() override;

private:
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbNormal;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbJump;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbDead;

	UPROPERTY(VisibleAnywhere)
	AAIController* NPCController;

	UPROPERTY(VisibleAnywhere)
	AS2DCharacter* PlayerCharacter;

	UPROPERTY()
	FVector PrevLoc;

	/**
	 * makes the cat jump
	 */
	void CatJump();
	/**
	 * makes the cat dash forwards
	 */
	void CatDash();


	void DestroyCat() { this->Destroy(); }
	
};
