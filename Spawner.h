// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class ECOSNEAK_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> PickUp;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SpawnTrash();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval = 3.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
