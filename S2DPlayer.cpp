// Fill out your copyright notice in the Description page of Project Settings.


#include "S2DPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperFlipbookComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AS2DPlayer::AS2DPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetCharacterType(ES2DCharacterType::Player);

	// configure player stats
	SetCharacterMaxHealth(300.f);
	SetKnockbackModifier(1.f);

	// configure player movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 445.f;
	GetCharacterMovement()->AirControl = .7f;

	GetCharacterMovement()->MaxWalkSpeed = 225.f;

	GetCapsuleComponent()->SetCapsuleRadius(13.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(23.5f);

	GetSprite()->SetRelativeLocation(FVector(-4.5f, 0.f, 21.f));

	// camera boom and camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bInheritPitch = false;
	CameraBoom->SetAbsolute(false, true, false);
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	CameraBoom->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	CameraBoom->bUsePawnControlRotation = false;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	SetCanDash(true);
	SetCanRangedAttack(false);

	SetAttackDelayTime(.1f);

	SetActorTickInterval(0.f);
}

// Called when the game starts or when spawned
void AS2DPlayer::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());

	if (PauseWidget) PauseMenu = CreateWidget<UUserWidget>(GetWorld(),  PauseWidget);
	else UE_LOG(LogTemp, Warning, TEXT("PLAYER - NO PAUSE WIDGET"));
	if (EndWidget) EndMenu = CreateWidget<UUserWidget>(GetWorld(), EndWidget);
	else UE_LOG(LogTemp, Warning, TEXT("PLAYER - NO END WIDGET"));
	
}

// Called every frame
void AS2DPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CharacterJumpStateManager();

	CharacterFlipbookManager();

}

void AS2DPlayer::CharacterFlipbookManager()
{
	// changes the players flip book when necessary
	
	if (GetCharacterState() == ES2DCharacterState::Dead)
	{
		GetSprite()->SetFlipbook(FbDead);
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		switch (GetCharacterState())
		{
		case ES2DCharacterState::Stagger:
			{
				GetSprite()->SetFlipbook(FbStagger);
				break;
			}
		case ES2DCharacterState::Dash:
			{
				GetSprite()->SetFlipbook(FbRun);
				break;
			}
		case ES2DCharacterState::Attack:
			{
				GetWorldTimerManager().ClearTimer(JumpFbHandle);
				GetSprite()->SetFlipbook(FbAttack);
				break;
			}
		case ES2DCharacterState::Jump:
			{
				if (!GetWorldTimerManager().IsTimerActive(JumpFbHandle) && GetSprite()->GetFlipbook() != FbJumpLast)
				{
					GetSprite()->SetFlipbook(FbJump);
					GetWorldTimerManager().SetTimer(JumpFbHandle, this, &AS2DPlayer::FBJumpLastDelay, GetSprite()->GetFlipbookLength(), false);
				}
				break;
			}
			default:
			{
				GetSprite()->SetFlipbook(FbJumpLast);
				break;
			}
		}
	}
	else
	{
		switch (GetCharacterState())
		{
		case ES2DCharacterState::Stagger:
			{
				GetSprite()->SetFlipbook(FbStagger);
				break;
			}
		case ES2DCharacterState::Attack:
			{
				GetSprite()->SetFlipbook(FbAttack);
				break;
			}
		default:
			{
				GetWorldTimerManager().ClearTimer(JumpFbHandle);
				
				if (GetCharacterMovement()->Velocity.Length() != 0.f)
				{
					GetSprite()->SetFlipbook(FbRun);
					return;
				}

				GetSprite()->SetFlipbook(FbIdle);
			}
		}
	}
		
}

void AS2DPlayer::FBJumpLastDelay()
{
	GetSprite()->SetFlipbook(FbJumpLast);
}

void AS2DPlayer::CharacterJumpStateManager()
{
	// changes the state to normal when the player touches the ground
	
	if (GetCharacterState() != ES2DCharacterState::Jump || GetCharacterMovement()->IsFalling()) return;

	GetWorldTimerManager().ClearTimer(JumpFbHandle);
	
	ChangeCharacterStateNormal();
}

void AS2DPlayer::CharacterDeath()
{
	AS2DCharacter::CharacterDeath();

	FTimerHandle EndMenuHandle;
	GetWorldTimerManager().SetTimer(EndMenuHandle, this, &AS2DPlayer::StartEndMenu, .5f, false);
	
}


// Called to bind functionality to input
void AS2DPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &AS2DPlayer::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AS2DPlayer::CharacterJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AS2DPlayer::StopJumping);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AS2DPlayer::CharacterDash);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AS2DPlayer::CharacterAttack);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AS2DPlayer::PauseGame);

	PlayerInputComponent->BindAction("RangedAttack", IE_Pressed, this, &AS2DPlayer::CharacterRangedAttack);




	// PlayerInputComponent->BindAction("Test", IE_Pressed, this, &AS2DPlayer::TestFunction);

}

void AS2DPlayer::MoveRight(float Value)
{
	if (GetCharacterState() == ES2DCharacterState::Dead) return;
	
	if (Controller && Value != 0.f)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// check direction of movement and rotate player
		if (Value < 0)
		{
			Controller->SetControlRotation(FRotator(0.f, 180.f, 0.f));

			// add movement in that direction
			AddMovementInput(Direction, Value*-1);
		}
		else
		{
			Controller->SetControlRotation(FRotator(0.f, 0.f, 0.f));

			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

void AS2DPlayer::PauseGame()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UE_LOG(LogTemp, Warning, TEXT("-GAME UNPAUSED-"));

		// PauseMenu->RemoveFromViewport();
		PauseMenu->RemoveFromParent();

		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
		
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("-GAME PAUSED-"));

	PauseMenu->AddToViewport();
		
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		
	UGameplayStatics::SetGamePaused(GetWorld(), true);

}

void AS2DPlayer::StartEndMenu()
{
	EndMenu->AddToViewport();
		
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}










// void AS2DPlayer::TestDelayFunction()
// {
// 	
// }

// void AS2DPlayer::TestFunction()
// {
// 	UE_LOG(LogTemp, Warning, TEXT("TEST"));
//
// 	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation() + GetActorForwardVector() * 50, 20.f, 12, FColor::Red, 2.f);
// 	
// 	// CharacterKnockback(GetActorLocation() + GetActorForwardVector() * 50);
//
// 	CharacterTakeDamage(200.f, GetActorLocation() + GetActorForwardVector() * 50);
// }
