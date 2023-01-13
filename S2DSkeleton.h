// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S2DCharacter.h"
#include "S2DSkeleton.generated.h"

class UPaperFlipbook;
class AAIController;

UENUM()
enum class ES2DSkeletonType : uint8
{
	Normal,
	Clothed
};

/**
 * 
 */
UCLASS()
class SIDE2D_API AS2DSkeleton : public AS2DCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS2DSkeleton();

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
	
	// virtual bool CharacterTakeDamage(float Damage, FVector KnockbackLoc) override;

	virtual void CharacterWait() override;

	void SetSkeletonType(ES2DSkeletonType Type) { SkeletonType = Type; }
	ES2DSkeletonType GetSkeletonType() const { return SkeletonType; }

	/**
	 * sets the flipbooks that will be used with the character and uses SkeletonWakeUpDelay after some time
	 */
	void SkeletonWakeUp();
	/**
	 * changes the character state to normal
	 */
	void SkeletonWakeUpDelay();

	virtual void CharacterFlipbookManager() override;

private:
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbNormalIdle;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbClothedIdle;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbNormalRise;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbClothedRise;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbNormalAttack;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbClothedAttack;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbNormalStagger;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbClothedStagger;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbDead;

	UPaperFlipbook* FbUseIdle;
	UPaperFlipbook* FbUseRise;
	UPaperFlipbook* FbUseAttack;
	UPaperFlipbook* FbUseStagger;
	
	UPROPERTY(VisibleAnywhere)
	AAIController* NPCController;

	UPROPERTY(VisibleAnywhere)
	AS2DCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere)
	ES2DSkeletonType SkeletonType;
	

	void DestroySkeleton() { this->Destroy(); }
	
};
