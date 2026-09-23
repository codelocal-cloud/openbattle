#include "OpenBattlePrototypeTower.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AOpenBattlePrototypeTower::AOpenBattlePrototypeTower()
{
    bReplicates = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Blocks = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Blocks"));
    Blocks->SetupAttachment(Root);
    Blocks->SetMobility(EComponentMobility::Movable);
    Blocks->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Blocks->SetCollisionResponseToAllChannels(ECR_Block);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Blocks->SetStaticMesh(CubeMesh.Object);
    }
}

void AOpenBattlePrototypeTower::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    BuildTower();
}

void AOpenBattlePrototypeTower::BuildTower()
{
    if (!Blocks)
    {
        return;
    }

    Blocks->ClearInstances();

    const float FullX = 2400.0f;
    const float FullY = 2000.0f;
    const float GapX = 600.0f;
    const float GapY = 900.0f;
    const float GapCenterX = 600.0f;
    const float Thickness = 40.0f;

    const float GapLeft = GapCenterX - GapX * 0.5f;
    const float GapRight = GapCenterX + GapX * 0.5f;
    const float MinX = -FullX * 0.5f;
    const float MaxX = FullX * 0.5f;
    const float MinY = -FullY * 0.5f;
    const float MaxY = FullY * 0.5f;
    const float GapMinY = -GapY * 0.5f;
    const float GapMaxY = GapY * 0.5f;

    auto AddBlock = [this](const FVector& Size, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator)
    {
        const FVector Scale(Size.X / 100.0f, Size.Y / 100.0f, Size.Z / 100.0f);
        Blocks->AddInstance(FTransform(Rotation, Location, Scale));
    };

    for (int32 FloorIndex = 0; FloorIndex < PrototypeFloors; ++FloorIndex)
    {
        const float FloorZ = FloorIndex * FloorHeight;
        const float SlabZ = FloorZ - Thickness * 0.5f;

        const float LeftWidth = GapLeft - MinX;
        AddBlock(
            FVector(LeftWidth, FullY, Thickness),
            FVector(MinX + LeftWidth * 0.5f, 0.0f, SlabZ));

        const float RightWidth = MaxX - GapRight;
        AddBlock(
            FVector(RightWidth, FullY, Thickness),
            FVector(GapRight + RightWidth * 0.5f, 0.0f, SlabZ));

        const float BackDepth = GapMinY - MinY;
        AddBlock(
            FVector(GapX, BackDepth, Thickness),
            FVector(GapCenterX, MinY + BackDepth * 0.5f, SlabZ));

        const float FrontDepth = MaxY - GapMaxY;
        AddBlock(
            FVector(GapX, FrontDepth, Thickness),
            FVector(GapCenterX, GapMaxY + FrontDepth * 0.5f, SlabZ));

        const float RailHeight = 110.0f;
        const float RailThickness = 20.0f;
        const float RailZ = FloorZ + RailHeight * 0.5f;

        AddBlock(FVector(FullX, RailThickness, RailHeight), FVector(0.0f, MinY, RailZ));
        AddBlock(FVector(FullX, RailThickness, RailHeight), FVector(0.0f, MaxY, RailZ));
        AddBlock(FVector(RailThickness, FullY, RailHeight), FVector(MinX, 0.0f, RailZ));
        AddBlock(FVector(RailThickness, FullY, RailHeight), FVector(MaxX, 0.0f, RailZ));
    }

    for (int32 FloorIndex = 0; FloorIndex < PrototypeFloors - 1; ++FloorIndex)
    {
        const float LowerZ = FloorIndex * FloorHeight;
        const float Run = GapY;
        const float Rise = FloorHeight;
        const float RampLength = FMath::Sqrt(Run * Run + Rise * Rise);
        const float RampRoll = FMath::RadiansToDegrees(FMath::Atan2(Rise, Run));

        AddBlock(
            FVector(GapX * 0.65f, RampLength, 30.0f),
            FVector(GapCenterX, 0.0f, LowerZ + Rise * 0.5f),
            FRotator(0.0f, 0.0f, RampRoll));
    }
}
