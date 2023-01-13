// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "S2DSpikeFloor.generated.h"

class UBoxComponent;

UCLASS()
class SIDE2D_API AS2DSpikeFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS2DSpikeFloor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;

	FTimerHandle ResetCollisionHandle;
	
	/**
	 * deals damage to characters that enter the collision capsule 
	 */
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	/**
	 * turns the collision capsule off and on 
	 */
	void ResetCollisionCapsule();

	UPROPERTY(EditAnywhere)
	float AttackDamage = 50.f;

};
