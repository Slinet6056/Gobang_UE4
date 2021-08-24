// Copyright Epic Games, Inc. All Rights Reserved.

#include "GobangGameMode.h"
#include "GobangPlayerController.h"
#include "GobangPawn.h"

AGobangGameMode::AGobangGameMode()
{
	// no pawn by default
	DefaultPawnClass = AGobangPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AGobangPlayerController::StaticClass();
}
