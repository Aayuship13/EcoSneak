
#include "EcoSneakCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PickUp.h"

// AEcoSneakCharacter

AEcoSneakCharacter::AEcoSneakCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	bIsJumping = false;
	bIsSprinting = false;
	bIsAiming = false;
	
	

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GunsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunsMesh->SetOnlyOwnerSee(true);
	GunsMesh->bCastDynamicShadow = false;
	GunsMesh->CastShadow = false;

}

// Input


void AEcoSneakCharacter::BeginPlay()
{
	Super::BeginPlay();
	GunsMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEcoSneakCharacter::OnHit);
}

void AEcoSneakCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEcoSneakCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AEcoSneakCharacter::OnJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprinting", IE_Pressed, this, &AEcoSneakCharacter::OnStartSprinting);
	PlayerInputComponent->BindAction("Sprinting", IE_Released, this, &AEcoSneakCharacter::OnStopSprinting);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AEcoSneakCharacter::OnStartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AEcoSneakCharacter::OnStopAiming);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEcoSneakCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEcoSneakCharacter::MoveRight);

	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AEcoSneakCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AEcoSneakCharacter::LookUpAtRate);

}

void AEcoSneakCharacter::PlayJumpAnimation()
{
	if (JumpAnimationMontage != nullptr && GetMesh() != nullptr)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		// Play the jump animation montage on the character's mesh
		//GetMesh()->GetAnimInstance()->Montage_Play(JumpAnimationMontage, 1.0f);
		//GetCharacterMovement()->SetMovementMode(InitialMovementMode);
	}
}

void AEcoSneakCharacter::OnStartAiming()
{
	bIsAiming = true;
}

void AEcoSneakCharacter::OnStopAiming()
{
	bIsAiming = false;
}

void AEcoSneakCharacter::OnJump()
{
	//InitialMovementMode = GetCharacterMovement()->MovementMode;
	bIsJumping = true; // Set jump flag
}

void AEcoSneakCharacter::OnStartSprinting()
{
	bIsSprinting = true;
	UpdateSpeed();
}

void AEcoSneakCharacter::OnStopSprinting()
{
	bIsSprinting = false;
	UpdateSpeed();
}

void AEcoSneakCharacter::UpdateSpeed()
{
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 650.0f;

	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void AEcoSneakCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEcoSneakCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AEcoSneakCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEcoSneakCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
void AEcoSneakCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	APickUp* Item = Cast<APickUp>(OtherActor);
	if (Item)
	{
		if (Item->ActorHasTag("Trash"))
		{
			Trash += 1;
			Item->OnPickup();
		}
	}
}


