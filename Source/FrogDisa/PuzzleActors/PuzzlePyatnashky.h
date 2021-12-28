// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrogDisa/PuzzleActors/PuzzleActor.h"
#include "Components/TextRenderComponent.h"
#include "Components/BillboardComponent.h"

#include "PuzzlePyatnashky.generated.h"

UCLASS()
class FROGDISA_API APuzzlePyatnashky : public APuzzleActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APuzzlePyatnashky();
	//std::vector<std::vector<UStaticMeshComponent*>> field_meshes;

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


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* Text1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* Text2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* Text3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* Text4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* Text5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* Text6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* Text7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* Text8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* Text9;

public:

	UFUNCTION(BlueprintCallable)
		void EditField(TArray<UStaticMesh*> arrMesh, TArray<FVector> arrVector, TArray<FString> arrTextName);
	UFUNCTION(BlueprintCallable)
		bool Check(UStaticMeshComponent* Mesh);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnClick(UStaticMeshComponent* Mesh);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
