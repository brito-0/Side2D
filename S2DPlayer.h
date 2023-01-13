// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S2DCharacter.h"
#include "S2DPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPaperFlipbook;
class UUserWidget;

/**
 * 
 */
UCLASS()
class SIDE2D_API AS2DPlayer : public AS2DCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS2DPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * pauses the game and adds the pause menu to the viewport or unpauses the game and removes pause menu from the viewport
	 */
	UFUNCTION(BlueprintCallable)
	void PauseGame();

	/**
	 * pauses the game and adds the end menu to the viewport
	 */
	UFUNCTION(BlueprintCallable)
	void StartEndMenu();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CharacterFlipbookManager() override;

	virtual void CharacterJumpStateManager() override;

	virtual void CharacterDeath() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbIdle;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbRun;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbJump;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbJumpLast;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbStagger;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbAttack;
	UPROPERTY(EditAnywhere, Category = Sprite)
	UPaperFlipbook* FbDead;

	FTimerHandle JumpFbHandle;
	
	void FBJumpLastDelay();
	

	/** Called for side to side input */
	void MoveRight(float Value);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PauseWidget;
	UPROPERTY()
	UUserWidget* PauseMenu;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> EndWidget;
	UPROPERTY()
	UUserWidget* EndMenu;

	



	
	// void TestDelayFunction();
	// void TestFunction();
	
};
