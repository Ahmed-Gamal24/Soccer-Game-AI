#include "fieldPlayer.h"

clsFieldPlayer::clsFieldPlayer()
    : clsPlayerBase(clsBaseGameEntity::nextValidId++)
{
}


void clsFieldPlayer::update(){}

void clsFieldPlayer::render(){}

bool clsFieldPlayer::handleMessage(clsTelegram telegram){}