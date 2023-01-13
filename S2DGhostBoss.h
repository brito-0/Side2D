// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S2DGhost.h"
#include "S2DGhostBoss.generated.h"

/**
 * 
 */
UCLASS()
class SIDE2D_API AS2DGhostBoss : public AS2DGhost
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS2DGhostBoss();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * removes certain mob ghost from the array MobGhostPhase3
	 * @param Mob mob ghost to remove
	 */
	void RemovePhase3Mob(AS2DGhost* Mob);

	/**
	 * returns the value of GhostPhase
	 */
	uint8 GetGhostPhase() const { return GhostPhase; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual bool CharacterTakeDamage(float Damage, FVector KnockbackLoc) override;

private:
	UPROPERTY(VisibleAnywhere)
	uint8 GhostPhase = 1;

	UPROPERTY(EditAnywhere, Category = MobGhost)
	TSubclassOf<AS2DGhost> MobGhostObject = nullptr;
	UPROPERTY()
	TArray<AS2DGhost*> MobGhostsPhase3;
	UPROPERTY()
	TArray<AS2DGhost*> MobGhostsPhase4;

	UPROPERTY(EditAnywhere)
	uint8 MobGhostsPhase3Size = 7;
	UPROPERTY(EditAnywhere)
	uint8 MobGhostsPhase4Size = 2;

	
	void SpawnPhase3MobGhosts();
	void SpawnPhase4MobGhosts();


	virtual void CharacterDeath() override;

	/**
	 * pauses the game and adds the end menu to the viewport
	 */
	void StartEndMenu();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> EndWidget;
	UPROPERTY()
	UUserWidget* EndMenu;
	
};
