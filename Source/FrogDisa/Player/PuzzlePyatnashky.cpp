#include "PuzzlePyatnashky.h"

#include <vector>
#include <iostream>

//std::map<FString, UTexture2D*> map_images;

//std::vector <std::vector<int>> field_numbers;
std::vector<std::vector<UStaticMeshComponent*>> field_meshes;
std::vector<std::vector<UTextRenderComponent*>> field_text;
const std::vector<std::vector<FString>> field_string_true = { {"1","2","3"}, {"4","5","6"},{"7","8","0"} };
std::vector<std::vector<FString>> field_string;
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
	field_meshes = { {Mesh1,Mesh2,Mesh3},{Mesh4,Mesh5, Mesh6},{Mesh7,Mesh8,Mesh9} };
	field_text = { {Text1,Text2,Text3},{Text4,Text5, Text6},{Text7,Text8,Text9} };
	//Mesh9->
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			field_meshes[i][j]->SetupAttachment(RootComponent);
			
		}
	}
	PrimaryActorTick.bCanEverTick = true;
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

void APuzzlePyatnashky::EditField(TArray<UStaticMesh*> arrMesh, TArray<FVector> arrVector, TArray<FString> arrTextName)
{
	field_string.resize(3);
	
	for (int i = 0; i < 3; i++)
	{
		field_string[i].resize(3);

		for (int j = 0; j < 3; j++)
		{
			field_meshes[i][j]->SetStaticMesh(arrMesh[i * 3 + j]);
			
			field_string[i][j] = arrTextName[i * 3 + j];
			
			//field_meshes[i][j]->SetupAttachment(RootComponent);
			field_meshes[i][j]->SetRelativeScale3D(FVector(0.5, 0.3, 0.3));
			field_meshes[i][j]->SetRelativeLocation(arrVector[i * 3 + j]);
			field_text[i][j]->SetupAttachment(field_meshes[i][j]);
			//field_text[i][j]->SetupAttachment(field_meshes[i][j]);
			field_text[i][j]->SetRelativeLocation(FVector::ZeroVector + FVector(76, 0, 0) + arrVector[i * 3 + j]);
			field_text[i][j]->SetText(FText::FromString(arrTextName[i * 3 + j]));
			field_text[i][j]->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
			field_text[i][j]->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
			//field_text[i][j]->Text = FText::FromString("FFFF");
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, field_text[i][j]-> GetAttachmentRoot() == field_meshes[i][j]? TEXT("true") : TEXT("no"));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, field_text[i][j]->Text.ToString());
		}
	}
}

bool APuzzlePyatnashky::Check(UStaticMeshComponent* Mesh)
{
	int moveX[4] = { 1,0,-1,0 };
	int moveY[4] = { 0,1,0,-1 };

	for (int k = 0; k < 3; k++)
	{
		for (int i = 0; i < 3; i++)
			if (Mesh == field_meshes[k][i])
			{
				for (int j = 0; j < 4; j++)
				{
					int nextPosX = i + moveX[j], nextPosY = k + moveY[j];
					if (nextPosY < 3 &&
						nextPosY >= 0 && nextPosX < 3 && nextPosX >= 0)
					{
						if (field_string[nextPosY][nextPosX] == "0")
						{
							UStaticMesh* mesh = field_meshes[nextPosY][nextPosX]->GetStaticMesh();
							UMaterialInterface* material  = field_meshes[nextPosY][nextPosX]->GetMaterial(0);
							//FVector first_Pos = Mesh9->GetComponentLocation();
							//Mesh9->SetWorldLocation(Mesh->GetComponentLocation(),false, nullptr, ETeleportType::TeleportPhysics);
							//Mesh->SetWorldLocation(first_Pos);
							
							field_meshes[nextPosY][nextPosX]->SetMaterial(0, Mesh->GetMaterial(0));
							Mesh->SetMaterial(0, material);
							//field_meshes[k + moveY[j]][i + moveX[j]] = Mesh;
							std::swap(field_string[k][i], field_string[nextPosY][nextPosX]);
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, field_string[nextPosY][nextPosX]);
							field_text[k][i]->SetText(FText::FromString(field_string[k][i]));
							field_text[nextPosY][nextPosX]->SetText(FText::FromString(field_string[nextPosY][nextPosX]));
							return field_string == field_string_true;
						}
					}
				}
			}

	}

	return false;
}

void APuzzlePyatnashky::OnClick_Implementation(UStaticMeshComponent* Mesh)
{
	Check(Mesh);
}