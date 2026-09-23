#include "OpenBattleExchangeBox.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "OpenBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOpenBattleExchangeBox::AOpenBattleExchangeBox()
{
    bReplicates = true;
    SetReplicateMovement(false);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
    Mesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.0f));
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }
}

int32 AOpenBattleExchangeBox::RedeemFor(AOpenBattleCharacter* Character)
{
    if (!HasAuthority() || !Character)
    {
        return 0;
    }

    return Character->RedeemCarriedIds();
}
