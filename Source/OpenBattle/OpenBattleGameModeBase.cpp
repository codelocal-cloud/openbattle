#include "OpenBattleGameModeBase.h"

#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "OpenBattleCharacter.h"
#include "OpenBattleExchangeBox.h"
#include "OpenBattleGameState.h"
#include "OpenBattlePlayerState.h"
#include "OpenBattlePrototypeTower.h"
#include "TimerManager.h"

AOpenBattleGameModeBase::AOpenBattleGameModeBase()
{
    DefaultPawnClass = AOpenBattleCharacter::StaticClass();
    GameStateClass = AOpenBattleGameState::StaticClass();
    PlayerStateClass = AOpenBattlePlayerState::StaticClass();
}

void AOpenBattleGameModeBase::InitGame(
    const FString& MapName,
    const FString& Options,
    FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    SpawnPrototypeWorld();
}

void AOpenBattleGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    MatchStartSeconds = GetWorld()->GetTimeSeconds();

    GetWorldTimerManager().SetTimer(
        FloorPressureTimer,
        this,
        &AOpenBattleGameModeBase::UpdateFloorPressure,
        FMath::Max(0.25f, FloorPressureTickSeconds),
        true);
}

void AOpenBattleGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!NewPlayer)
    {
        return;
    }

    AOpenBattlePlayerState* State = NewPlayer->GetPlayerState<AOpenBattlePlayerState>();
    AGameStateBase* BaseGameState = GetGameState<AGameStateBase>();
    if (!State || !BaseGameState)
    {
        return;
    }

    const int32 PlayerIndex = FMath::Max(0, BaseGameState->PlayerArray.IndexOfByKey(State));
    const int32 Floor = FMath::Clamp(
        (PlayerIndex / FMath::Max(1, PlayersPerFloor)) + 1,
        1,
        PrototypeFloors);
    const int32 Slot = PlayerIndex % FMath::Max(1, PlayersPerFloor);

    State->SetOriginFloor(Floor);
    State->SetRole(Floor == PrototypeFloors ? EOBRole::Security : EOBRole::Rebel);

    const int32 StartIndex = (Floor - 1) * PlayersPerFloor + Slot;
    if (PrototypeStarts.IsValidIndex(StartIndex) && NewPlayer->GetPawn())
    {
        const FTransform StartTransform = PrototypeStarts[StartIndex]->GetActorTransform();
        NewPlayer->GetPawn()->SetActorLocationAndRotation(
            StartTransform.GetLocation(),
            StartTransform.GetRotation().Rotator(),
            false,
            nullptr,
            ETeleportType::TeleportPhysics);
    }
}

AActor* AOpenBattleGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    if (Player)
    {
        AOpenBattlePlayerState* State = Player->GetPlayerState<AOpenBattlePlayerState>();
        AGameStateBase* BaseGameState = GetGameState<AGameStateBase>();

        if (State && BaseGameState)
        {
            const int32 PlayerIndex = BaseGameState->PlayerArray.IndexOfByKey(State);
            if (PlayerIndex != INDEX_NONE)
            {
                const int32 Floor = FMath::Clamp(
                    (PlayerIndex / FMath::Max(1, PlayersPerFloor)) + 1,
                    1,
                    PrototypeFloors);
                const int32 Slot = PlayerIndex % FMath::Max(1, PlayersPerFloor);
                const int32 StartIndex = (Floor - 1) * PlayersPerFloor + Slot;

                if (PrototypeStarts.IsValidIndex(StartIndex) && IsValid(PrototypeStarts[StartIndex]))
                {
                    return PrototypeStarts[StartIndex];
                }
            }
        }
    }

    return Super::ChoosePlayerStart_Implementation(Player);
}

void AOpenBattleGameModeBase::SpawnPrototypeWorld()
{
    if (!HasAuthority())
    {
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<AOpenBattlePrototypeTower>(
        AOpenBattlePrototypeTower::StaticClass(),
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        SpawnParams);

    PrototypeStarts.Reset();

    const FVector SlotOffsets[4] =
    {
        FVector(-750.0f, -550.0f, 0.0f),
        FVector(-750.0f,  550.0f, 0.0f),
        FVector(-250.0f, -650.0f, 0.0f),
        FVector(-250.0f,  650.0f, 0.0f)
    };

    for (int32 Floor = 1; Floor <= PrototypeFloors; ++Floor)
    {
        const float FloorZ = (Floor - 1) * 450.0f;

        for (int32 Slot = 0; Slot < PlayersPerFloor; ++Slot)
        {
            const FVector SpawnLocation =
                SlotOffsets[Slot % 4] + FVector(0.0f, 0.0f, FloorZ + 100.0f);

            if (APlayerStart* Start = GetWorld()->SpawnActor<APlayerStart>(
                APlayerStart::StaticClass(),
                SpawnLocation,
                FRotator::ZeroRotator,
                SpawnParams))
            {
                PrototypeStarts.Add(Start);
            }
        }

        GetWorld()->SpawnActor<AOpenBattleExchangeBox>(
            AOpenBattleExchangeBox::StaticClass(),
            FVector(-950.0f, 0.0f, FloorZ + 50.0f),
            FRotator::ZeroRotator,
            SpawnParams);
    }
}

void AOpenBattleGameModeBase::UpdateFloorPressure()
{
    if (!HasAuthority())
    {
        return;
    }

    AOpenBattleGameState* State = GetGameState<AOpenBattleGameState>();
    if (!State)
    {
        return;
    }

    const float Elapsed = GetWorld()->GetTimeSeconds() - MatchStartSeconds;

    for (int32 Floor = 1; Floor <= PrototypeFloors; ++Floor)
    {
        if (Floor == PrototypeFloors)
        {
            State->SetFloorState(Floor, EOBFloorState::Safe);
            continue;
        }

        const float Offset = static_cast<float>(Floor - 1) * 60.0f;
        EOBFloorState NewState = EOBFloorState::Safe;

        if (Elapsed >= 120.0f + Offset)
        {
            NewState = EOBFloorState::Lost;
        }
        else if (Elapsed >= 90.0f + Offset)
        {
            NewState = EOBFloorState::Critical;
        }
        else if (Elapsed >= 60.0f + Offset)
        {
            NewState = EOBFloorState::Unstable;
        }

        State->SetFloorState(Floor, NewState);
    }

    for (TActorIterator<AOpenBattleCharacter> It(GetWorld()); It; ++It)
    {
        AOpenBattleCharacter* Character = *It;
        if (!Character || !Character->IsAlive())
        {
            continue;
        }

        const EOBFloorState FloorState =
            State->GetFloorState(Character->GetCurrentFloor());

        float PressureDamage = 0.0f;
        if (FloorState == EOBFloorState::Critical)
        {
            PressureDamage = 2.0f;
        }
        else if (FloorState == EOBFloorState::Lost)
        {
            PressureDamage = 10.0f;
        }

        if (PressureDamage > 0.0f)
        {
            UGameplayStatics::ApplyDamage(
                Character,
                PressureDamage,
                nullptr,
                this,
                UDamageType::StaticClass());
        }
    }
}
