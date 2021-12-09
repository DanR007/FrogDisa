#include "PuzzlePyatnashky.h"

#include <vector>
#include <iostream>

//std::map<FString, UTexture2D*> map_images;

//std::vector <std::vector<int>> field_numbers;
// Sets default values
APuzzlePyatnashky::APuzzlePyatnashky()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	RootComponent = MainMesh;

	Mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh1"));
	Mesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh2"));
	Mesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh3"));
	Mesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh4"));
	Mesh5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh5"));
	Mesh6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh6"));
	Mesh7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh7"));
	Mesh8 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh8"));
	Mesh9 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh9"));

	Text1 = CreateDefaultSubobject<UTextRenderComponent >(TEXT("text1"));
	Text2 = CreateDefaultSubobject<UTextRenderComponent >(TEXT("text2"));
	Text3 = CreateDefaultSubobject<UTextRenderComponent >(TEXT("text3"));
	Text4 = CreateDefaultSubobject<UTextRenderComponent >(TEXT("text4"));
	Text5 = CreateDefaultSubobject<UTextRenderComponent >(TEXT("text5"));
	Text6 = CreateDefaultSubobject<UTextRenderComponent >(TEXT("text6"));
	Text7 = CreateDefaultSubobject<UTextRenderComponent >(TEXT("text7"));
	Text8 = CreateDefaultSubobject<UTextRenderComponent >(TEXT("text8"));
	Text9 = CreateDefaultSubobject<UTextRenderComponent >(TEXT("text9"));

	//field_meshes = { {Mesh2,Mesh4,Mesh3},{Mesh1,Mesh6, Mesh8},{Mesh7,Mesh5,Mesh9} };
	//field_meshes_true = { {Mesh1,Mesh2,Mesh3},{Mesh4,Mesh5, Mesh6},{Mesh7,Mesh8,Mesh9} };



	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APuzzlePyatnashky::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APuzzlePyatnashky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzlePyatnashky::EditField(TArray<UStaticMeshComponent*> arrMesh, TArray<UTextRenderComponent*> arrText, TArray<FVector> arrVector, TArray<FString> arrTextName)
{
	field_meshes.resize(3);
	field_text.resize(3);
	
	for (int i = 0; i < 3; i++)
	{
		field_meshes[i].resize(3);
		field_text[i].resize(3);
		for (int j = 0; j < 3; j++)
		{
			field_meshes[i][j] = arrMesh[i * 3 + j];
			field_text[i][j] = arrText[i * 3 + j];

			field_meshes[i][j]->SetRelativeLocation(arrVector[i * 3 + j]);
			field_text[i][j]->SetupAttachment(field_meshes[i][j]);
			field_text[i][j]->SetRelativeLocation(FVector::ZeroVector + FVector(10, 0, 0));
			field_text[i][j]->Text.FromString(arrTextName[i * 3 + j]);
			field_text[i][j]->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
			field_text[i][j]->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
		}
	}
}

bool APuzzlePyatnashky::Check(UStaticMeshComponent* Mesh)
{
	int moveX[4] = { 1,0,-1,0 };
	int moveY[4] = { 0,1,0,-1 };
	bool flag = true;
	for (int k = 0; k < 3; k++)
	{
		for (int i = 0; i < 3; i++)
			if (Mesh == field_meshes[k][i])
			{
				for (int j = 0; j < 4; j++)
				{
					if (k + moveY[j] < 3 &&
						k + moveY[j] >= 0 && i + moveX[j] < 3 && i + moveX[j] >= 0)
					{
						if (field_meshes[k + moveY[j]][i + moveX[j]] == Mesh9 && flag)
						{
							FVector first_Pos = Mesh9->GetComponentLocation();
							Mesh9->SetWorldLocation(Mesh->GetComponentLocation(),false, nullptr, ETeleportType::TeleportPhysics);
							Mesh->SetWorldLocation(first_Pos);

							field_meshes[k][i] = Mesh9;
							field_meshes[k + moveY[j]][i + moveX[j]] = Mesh;
							flag = false;
						}
					}
				}
			}

	}
	return field_meshes == field_meshes_true;
}