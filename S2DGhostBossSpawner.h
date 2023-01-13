// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Paper2D/Classes/PaperTileMapActor.h"
#include "S2DGhostBossSpawner.generated.h"

class AS2DCharacter;
class UBoxComponent;
class AS2DGhost;

UCLASS()
class SIDE2D_API AS2DGhostBossSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS2DGhostBossSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;
	
	/**
	 * spawns the ghost boss and the door 
	 */
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	/**
	 * destroys the spawner
	 */
	void DestroySpawner();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AS2DGhost> GhostBossObject;

	UPROPERTY(EditAnywhere)
	APaperTileMapActor* DoorTileMap;

	UPROPERTY(VisibleAnywhere)
	TArray<AS2DCharacter*> EntranceMonsters;

	/**
	 * places all of the skeleton characters and cat characters within the first room inside of the EntranceMonsters array
	 */
	void GetEntranceMonsters();

	/**
	 * destroys the characters within EntranceMonsters array
	 */
	void KillEntranceMonsters();

};
