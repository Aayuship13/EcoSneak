// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "EcoSneakCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//GunsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	//GunsMesh->SetOnlyOwnerSee(true);
	//GunsMesh->bCastDynamicShadow = false;
	//GunsMesh->CastShadow = false;

}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	

	// Attach gun mesh to the parent mesh
	//GunsMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

