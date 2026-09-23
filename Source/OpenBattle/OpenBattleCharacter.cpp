#include "OpenBattleCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "OpenBattleExchangeBox.h"
#include "OpenBattleIdCard.h"
#include "OpenBattlePlayerState.h"

AOpenBattleCharacter::AOpenBattleCharacter()
{
    bReplicates = true;
    SetReplicateMovement(true);

    bUseControllerRotationYaw = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
    FirstPersonCamera->bUsePawnControlRotation = true;
}

void AOpenBattleCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        Health = MaxHealth;
    }
}

void AOpenBattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AOpenBattleCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AOpenBattleCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AOpenBattleCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AOpenBattleCharacter::LookUp);

    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AOpenBattleCharacter::Fire);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AOpenBattleCharacter::Interact);
    PlayerInputComponent->BindAction(TEXT("Alliance"), IE_Pressed, this, &AOpenBattleCharacter::RequestAlliance);
    PlayerInputComponent->BindAction(TEXT("AcceptAlliance"), IE_Pressed, this, &AOpenBattleCharacter::AcceptAlliance);
}

void AOpenBattleCharacter::MoveForward(float Value)
{
    if (!IsAlive() || FMath::IsNearlyZero(Value))
    {
        return;
    }

    AddMovementInput(GetActorForwardVector(), Value);
}

void AOpenBattleCharacter::MoveRight(float Value)
{
    if (!IsAlive() || FMath::IsNearlyZero(Value))
    {
        return;
    }

    AddMovementInput(GetActorRightVector(), Value);
}

void AOpenBattleCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AOpenBattleCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AOpenBattleCharacter::Fire()
{
    if (IsAlive())
    {
        ServerFire();
    }
}

void AOpenBattleCharacter::Interact()
{
    if (IsAlive())
    {
        ServerInteract();
    }
}

void AOpenBattleCharacter::RequestAlliance()
{
    if (IsAlive())
    {
        ServerRequestAlliance();
    }
}

void AOpenBattleCharacter::AcceptAlliance()
{
    if (IsAlive())
    {
        ServerAcceptAlliance();
    }
}

bool AOpenBattleCharacter::TraceFromView(float Range, FHitResult& OutHit) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    GetActorEyesViewPoint(ViewLocation, ViewRotation);

    const FVector End = ViewLocation + ViewRotation.Vector() * Range;

    FCollisionQueryParams Params(SCENE_QUERY_STAT(OpenBattleTrace), true, this);
    Params.AddIgnoredActor(this);

    return GetWorld()->LineTraceSingleByChannel(
        OutHit,
        ViewLocation,
        End,
        ECC_Visibility,
        Params);
}

void AOpenBattleCharacter::ServerFire_Implementation()
{
    if (!IsAlive())
    {
        return;
    }

    const float Now = GetWorld()->GetTimeSeconds();
    if ((Now - LastServerFireTime) < FireCooldown)
    {
        return;
    }
    LastServerFireTime = Now;

    FHitResult Hit;
    if (!TraceFromView(FireRange, Hit) || !Hit.GetActor())
    {
        return;
    }

    FVector ViewLocation;
    FRotator ViewRotation;
    GetActorEyesViewPoint(ViewLocation, ViewRotation);

    UGameplayStatics::ApplyPointDamage(
        Hit.GetActor(),
        DamagePerShot,
        ViewRotation.Vector(),
        Hit,
        GetController(),
        this,
        UDamageType::StaticClass());
}

void AOpenBattleCharacter::ServerInteract_Implementation()
{
    if (!IsAlive())
    {
        return;
    }

    FHitResult Hit;
    if (!TraceFromView(InteractionRange, Hit))
    {
        return;
    }

    if (AOpenBattleExchangeBox* ExchangeBox = Cast<AOpenBattleExchangeBox>(Hit.GetActor()))
    {
        ExchangeBox->RedeemFor(this);
    }
}

void AOpenBattleCharacter::ServerRequestAlliance_Implementation()
{
    if (!IsAlive())
    {
        return;
    }

    FHitResult Hit;
    if (!TraceFromView(AllianceRange, Hit))
    {
        return;
    }

    AOpenBattleCharacter* Target = Cast<AOpenBattleCharacter>(Hit.GetActor());
    if (!Target || Target == this || !Target->IsAlive())
    {
        return;
    }

    if (FVector::DistSquared(GetActorLocation(), Target->GetActorLocation()) > FMath::Square(AllianceRange))
    {
        return;
    }

    Target->PendingAllianceRequester = this;
    Target->ForceNetUpdate();
}

void AOpenBattleCharacter::ServerAcceptAlliance_Implementation()
{
    AOpenBattleCharacter* Requester = PendingAllianceRequester;
    PendingAllianceRequester = nullptr;

    if (!Requester || !Requester->IsAlive() || !IsAlive())
    {
        return;
    }

    if (FVector::DistSquared(GetActorLocation(), Requester->GetActorLocation()) > FMath::Square(AllianceRange))
    {
        return;
    }

    AOpenBattlePlayerState* SelfState = GetPlayerState<AOpenBattlePlayerState>();
    AOpenBattlePlayerState* RequesterState = Requester->GetPlayerState<AOpenBattlePlayerState>();
    if (!SelfState || !RequesterState)
    {
        return;
    }

    const bool bSelfHasAlliance = SelfState->Alliance.IsValid();
    const bool bRequesterHasAlliance = RequesterState->Alliance.IsValid();

    if (bSelfHasAlliance && bRequesterHasAlliance)
    {
        if (SelfState->Alliance.AllianceId == RequesterState->Alliance.AllianceId)
        {
            return;
        }

        // M1 intentionally does not merge two existing alliances.
        return;
    }

    if (bSelfHasAlliance)
    {
        RequesterState->SetAlliance(SelfState->Alliance);
        return;
    }

    if (bRequesterHasAlliance)
    {
        SelfState->SetAlliance(RequesterState->Alliance);
        return;
    }

    FOBAllianceIdentity NewAlliance;
    NewAlliance.AllianceId = FGuid::NewGuid();
    NewAlliance.VisualIndex = static_cast<int32>(GetTypeHash(NewAlliance.AllianceId) % 8);

    SelfState->SetAlliance(NewAlliance);
    RequesterState->SetAlliance(NewAlliance);
}

float AOpenBattleCharacter::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    if (!HasAuthority() || bDead || DamageAmount <= 0.0f)
    {
        return 0.0f;
    }

    const float AppliedDamage = FMath::Min(Health, DamageAmount);
    Health = FMath::Max(0.0f, Health - AppliedDamage);

    if (Health <= 0.0f)
    {
        HandleDeath(EventInstigator);
    }

    return AppliedDamage;
}

void AOpenBattleCharacter::HandleDeath(AController* Killer)
{
    if (!HasAuthority() || bDead)
    {
        return;
    }

    bDead = true;

    AOpenBattlePlayerState* State = GetPlayerState<AOpenBattlePlayerState>();
    if (State)
    {
        if (Killer)
        {
            AOpenBattlePlayerState* KillerState = Killer->GetPlayerState<AOpenBattlePlayerState>();
            if (KillerState && KillerState != State)
            {
                int32 KillReward = 100;

                if (KillerState->OriginFloor == State->OriginFloor)
                {
                    KillReward += 500;
                }

                if (State->Role == EOBRole::Security)
                {
                    KillReward += 500;
                }

                KillerState->AddRankScore(KillReward);
            }
        }

        FOBIdCardRecord Record;
        Record.CardId = FGuid::NewGuid();
        Record.OriginFloor = State->OriginFloor;
        Record.VictimPlayerId = State->GetPlayerId();
        Record.VictimRole = State->Role;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        if (AOpenBattleIdCard* Card = GetWorld()->SpawnActor<AOpenBattleIdCard>(
            AOpenBattleIdCard::StaticClass(),
            GetActorLocation() + FVector(0.0f, 0.0f, 45.0f),
            FRotator::ZeroRotator,
            SpawnParams))
        {
            Card->InitializeCard(Record);
        }
    }

    GetCharacterMovement()->DisableMovement();
    SetActorEnableCollision(false);
    ForceNetUpdate();
}

bool AOpenBattleCharacter::TryCollectIdCard(const FOBIdCardRecord& Record)
{
    if (!HasAuthority() || !IsAlive() || !Record.IsValid())
    {
        return false;
    }

    for (const FOBIdCardRecord& Existing : CarriedIds)
    {
        if (Existing.CardId == Record.CardId)
        {
            return false;
        }
    }

    CarriedIds.Add(Record);
    ForceNetUpdate();
    return true;
}

int32 AOpenBattleCharacter::RedeemCarriedIds()
{
    if (!HasAuthority() || CarriedIds.IsEmpty())
    {
        return 0;
    }

    AOpenBattlePlayerState* State = GetPlayerState<AOpenBattlePlayerState>();
    if (!State)
    {
        return 0;
    }

    int32 RewardScore = 0;
    for (const FOBIdCardRecord& Card : CarriedIds)
    {
        int32 CardReward = 100;

        if (Card.OriginFloor == State->OriginFloor)
        {
            CardReward += 500;
        }

        if (Card.VictimRole == EOBRole::Security)
        {
            CardReward += 500;
        }

        RewardScore += CardReward;
    }

    State->AddRankScore(RewardScore);

    const float HealAmount = FMath::Min(50.0f, 10.0f * static_cast<float>(CarriedIds.Num()));
    Health = FMath::Min(MaxHealth, Health + HealAmount);

    CarriedIds.Reset();
    ForceNetUpdate();

    return RewardScore;
}

int32 AOpenBattleCharacter::GetCurrentFloor() const
{
    const float RelativeZ = GetActorLocation().Z - TowerBaseZ;
    const int32 Floor = 1 + FMath::FloorToInt(RelativeZ / FMath::Max(1.0f, FloorHeight));
    return FMath::Clamp(Floor, 1, 10);
}

void AOpenBattleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AOpenBattleCharacter, Health);
    DOREPLIFETIME(AOpenBattleCharacter, bDead);
    DOREPLIFETIME_CONDITION(AOpenBattleCharacter, CarriedIds, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AOpenBattleCharacter, PendingAllianceRequester, COND_OwnerOnly);
}
