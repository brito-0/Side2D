// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DSpikeFloor.h"
#include "S2DCat.h"
#include "Components/BoxComponent.h"
#include "S2DCharacter.h"

// Sets default values
AS2DSpikeFloor::AS2DSpikeFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collsion"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetRelativeScale3D(FVector(.49, .49, .49));
	BoxCollision->SetCollisionProfileName(TEXT("S2DTrigger"));

}

// Called when the game starts or when spawned
void AS2DSpikeFloor::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AS2DSpikeFloor::OnBeginOverlap);
	
}

// Called every frame
void AS2DSpikeFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AS2DSpikeFloor::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(AS2DCharacter::StaticClass())) return;
	
	AS2DCharacter* Other = Cast<AS2DCharacter>(OtherActor);

	if (Other->IsA(AS2DCat::StaticClass()))
	{
		Other->CharacterTakeDamage(500.f, GetActorLocation());
		return;
	} 

	if (Other->GetCharacterType() != ES2DCharacterType::Player) return;

	GetWorldTimerManager().ClearTimer(ResetCollisionHandle);
	
	Other->CharacterTakeDamage(AttackDamage, GetActorLocation());

	
	GetWorldTimerManager().SetTimer(ResetCollisionHandle, this, &AS2DSpikeFloor::ResetCollisionCapsule, .7f, false);
	
}

void AS2DSpikeFloor::ResetCollisionCapsule()
{
	SetActorEnableCollision(false);
	SetActorEnableCollision(true);
	
}
