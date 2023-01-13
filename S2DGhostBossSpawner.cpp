// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DGhostBossSpawner.h"
#include "Components/BoxComponent.h"
#include "S2DCharacter.h"
#include "S2DGhost.h"
#include "S2DSkeleton.h"
#include "S2DCat.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AS2DGhostBossSpawner::AS2DGhostBossSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collsion"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetRelativeScale3D(FVector(.49, .49, 1.f));
	BoxCollision->SetCollisionProfileName(TEXT("S2DTrigger"));
	
}

// Called when the game starts or when spawned
void AS2DGhostBossSpawner::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AS2DGhostBossSpawner::OnBeginOverlap);

	if (!GhostBossObject) UE_LOG(LogTemp, Warning, TEXT("GHOST BOSS SPAWNER - NO GHOST OBJECT"));

	if (!DoorTileMap) UE_LOG(LogTemp, Warning, TEXT("GHOST BOSS SPAWNER - NO DOOR REFERENCE"));

	
	GetEntranceMonsters();
	
}

// Called every frame
void AS2DGhostBossSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AS2DGhostBossSpawner::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(AS2DCharacter::StaticClass())) return;

	// const AS2DCharacter* Other = Cast<AS2DCharacter>(OtherActor);

	if (Cast<AS2DCharacter>(OtherActor)->GetCharacterType() != ES2DCharacterType::Player) return;

	const FRotator Rotation = FRotator(0.f, 0.f, 0.f);

	const FVector GhostLocation = FVector(450.f, 0.f, 300.f);
	
	GetWorld()->SpawnActor<AS2DGhost>(GhostBossObject, GhostLocation, Rotation);

	DoorTileMap->SetActorHiddenInGame(false);
	DoorTileMap->SetActorEnableCollision(true);

	KillEntranceMonsters();

	DestroySpawner();
	
}

void AS2DGhostBossSpawner::DestroySpawner()
{
	DoorTileMap = nullptr;
	
	this->Destroy();
	
}

void AS2DGhostBossSpawner::GetEntranceMonsters()
{
	UE_LOG(LogTemp, Warning, TEXT("GET ACTORS"));
	
	
	TArray<AActor*> Temp;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AS2DSkeleton::StaticClass(), Temp);
	for (AActor* Actor : Temp)
	{
		if (Actor) EntranceMonsters.Add(Cast<AS2DCharacter>(Actor));
	}
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AS2DCat::StaticClass(), Temp);
	for (AActor* Actor : Temp)
	{
		if (Actor) EntranceMonsters.Add(Cast<AS2DCharacter>(Actor));
	}
	
}

void AS2DGhostBossSpawner::KillEntranceMonsters()
{
	for (AS2DCharacter* Character : EntranceMonsters)
	{
		if (Character) Character->Destroy();
	}
	
}
