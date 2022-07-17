// Fill out your copyright notice in the Description page of Project Settings.


#include "NonlethalMineActor.h"
#include "../DefaultVariables.h"
#include "FrogDisa/AI/NPCPawn.h"


void ANonlethalMineActor::Explosion()
{
	TArray<AActor*> array_enemy;
	FHitResult hitPoint;

	ExplosionCollision->GetOverlappingActors(array_enemy, APawn::StaticClass());

	for (AActor* actor : array_enemy)
	{
		ANPCPawn* npc = Cast<ANPCPawn>(actor);
		if (npc)
			if (GetWorld()->LineTraceSingleByChannel(hitPoint, GetActorLocation(), actor->GetActorLocation(), ECollisionChannel::ECC_Visibility, queryParams))
			{
				if (npc == hitPoint.GetActor())
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Damage");
					npc->Sleep();
				}
			}
	}

	CallExplosionParticle();

	Destroy();
}
