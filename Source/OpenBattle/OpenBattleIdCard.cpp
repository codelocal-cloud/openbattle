#include "OpenBattleIdCard.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Net/UnrealNetwork.h"
#include "OpenBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOpenBattleIdCard::AOpenBattleIdCard()
{
    bReplicates = true;
    SetReplicateMovement(true);

    PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
    SetRootComponent(PickupSphere);
    PickupSphere->InitSphereRadius(90.0f);
    PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    PickupSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AOpenBattleIdCard::HandlePickupOverlap);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(PickupSphere);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Mesh->SetRelativeScale3D(FVector(0.35f, 0.22f, 0.05f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }
}

void AOpenBattleIdCard::InitializeCard(const FOBIdCardRecord& InRecord)
{
    if (HasAuthority())
    {
        Record = InRecord;
    }
}

void AOpenBattleIdCard::HandlePickupOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!HasAuthority() || !Record.IsValid())
    {
        return;
    }

    AOpenBattleCharacter* Character = Cast<AOpenBattleCharacter>(OtherActor);
    if (Character && Character->TryCollectIdCard(Record))
    {
        Destroy();
    }
}

void AOpenBattleIdCard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AOpenBattleIdCard, Record);
}
