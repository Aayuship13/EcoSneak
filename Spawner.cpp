// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "PickUp.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASpawner::SpawnTrash()
{
	if (PickUp)
	{

		FVector SpawnLocation = GetActorLocation() + FVector(FMath::RandRange(-2000, 2000), FMath::RandRange(-2000, 2000), 0);
		FRotator SpawnRotation = GetActorRotation() + FRotator(0, FMath::RandRange(0, 360), 0);
		SpawnLocation.Z = GetActorLocation().Z;
		AActor* NewTrash = GetWorld()->SpawnActor<AActor>(PickUp, SpawnLocation, SpawnRotation);

	}
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle SpawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawner::SpawnTrash, SpawnInterval, true, SpawnInterval);
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

