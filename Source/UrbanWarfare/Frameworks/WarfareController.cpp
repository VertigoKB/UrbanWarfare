// Fill out your copyright notice in the Description page of Project Settings.


#include "WarfareController.h"
//#include "UrbanWarfare/Player/PlayerBase.h"
//#include "UrbanWarfare/Player/Components/PlayerBehaviorComponent.h"
//#include "UrbanWarfare/Frameworks/WarfareGameMode.h"
#include "UrbanWarfare/Frameworks/ControllerComponents/UserInterfaceComponent.h"
#include "UrbanWarfare/Frameworks/ControllerComponents/CinematicComponent.h"
//#include "UrbanWarfare/UI/MainMenu.h"

AWarfareController::AWarfareController()
{
	//APlayerCameraManager* temp;
	//temp->ViewPitchMin
	UserInterfaceComponent = CreateDefaultSubobject<UUserInterfaceComponent>(TEXT("UserInterfaceComponent"));
	CinematicComponent = CreateDefaultSubobject<UCinematicComponent>(TEXT("CinematicComponent"));
}

UUserInterfaceComponent* AWarfareController::GetUserInterfaceComponent() { return UserInterfaceComponent; }

//APlayerBase* AWarfareController::GeWarfarePawn() { return ; }

void AWarfareController::BeginPlay()
{
	Super::BeginPlay();



   /* ALevelSequenceActor*

        if (LevelSequenceActor && LevelSequenceActor->GetSequence())
        {
            FMovieSceneSequencePlaybackSettings PlaybackSettings;
            ULevelSequencePlayer* SequencePlayer;

            // LevelSequencePlayer 생성
            SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
                GetWorld(),
                LevelSequenceActor->GetSequence(),
                PlaybackSettings,
                LevelSequenceActor
            );

            if (SequencePlayer)
            {
                SequencePlayer->Play(); // 시퀀스 재생
            }
        }*/

}

void AWarfareController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AWarfareController::OnUnPossess()
{
	Super::OnUnPossess();
}
