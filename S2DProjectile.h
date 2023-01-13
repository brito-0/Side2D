// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S2DProjectile.generated.h"

class UCapsuleComponent;
class AS2DCharacter;
class UPaperSpriteComponent;

/**
 * 
 */
UCLASS()
class SIDE2D_API AS2DProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS2DProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * moves the projectile to a different position and sets bFired to true
	 * @param OwnerChar character that fired the projectile
	 * @param Direction direction that the projectile will move towards
	 * @param Position position where the projectile will start moving from
	 */
	void FireProjectile(AS2DCharacter* OwnerChar, FVector Direction, FVector Position);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * deals damage to characters that enter the collision capsule 
	 */
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	/**
	 * returns the value of bFired
	 */
	bool GetFired() const { return bFired; }
	/**
	 * sets the value of bFired
	 * @param Value new value
	 */
	void SetFired(const bool Value) { bFired = Value; }

	/**
	 * moves projectile in a direction set by FireProjectile
	 */
	void MoveProjectile();
	
private:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CollisionComponent;
	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* Sprite;

	UPROPERTY()
	AS2DCharacter* OwnerCharacter;

	UPROPERTY()
	float XValue;

	UPROPERTY(EditAnywhere)
	float AttackDamage = 50.f;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 500.f;

	UPROPERTY(VisibleAnywhere)
	bool bFired = false;

	
	void DestroyProjectile() { this->Destroy(); }
	
};
