// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogDisa/PuzzleActor.h"
#include "Components/BillboardComponent.h"
#include <vector>
#include "PuzzlePyatnashky.generated.h"

UCLASS()
class FROGDISA_API APuzzlePyatnashky : public APuzzleActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APuzzlePyatnashky();
	std::vector<std::vector<UStaticMeshComponent*>> field_meshes;
	std::vector<std::vector<UStaticMeshComponent*>> field_meshes_true;
	std::vector<std::vector<UBillboardComponent*>> field_text;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh6;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh7;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh8;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Mesh9;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* Text1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* Text2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* Text3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* Text4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* Text5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* Text6;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* Text7;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* Text8;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBillboardComponent* Text9;

public:

	UFUNCTION(BlueprintCallable)
	void EditField(TArray<int> arr1, TArray<int> arr2, TArray<int> arr3);
	UFUNCTION(BlueprintCallable)
	bool Check(UStaticMeshComponent* Mesh);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
