// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S2DCharacter.h"
#include "Paper2D/Classes/PaperTileMapActor.h"
#include "S2DTrigger.generated.h"

class UPaperFlipbook;

/**
 * 
 */
UCLASS()
class SIDE2D_API AS2DTrigger : public AS2DCharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS2DTrigger();

	// // Called every frame
	// virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CharacterDeath() override;

private:
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbDead;
	
	UPROPERTY(EditAnywhere)
	APaperTileMapActor* DoorTileMap;

	void DestroyTrigger();

};
