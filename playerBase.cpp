#include"playerBase.h"

clsPlayerBase::clsPlayerBase():clsMovingEntity(clsBaseGameEntity::nextValidId++){}

clsPlayerBase::clsPlayerBase(int id):clsMovingEntity(id){}

bool clsPlayerBase::canPassForward(clsFieldPlayer receiver,clsFieldPlayer target, int power){}

bool clsPlayerBase::canPassBackward(clsFieldPlayer receiver,clsFieldPlayer target, int power){}

bool clsPlayerBase::withinShootingRange(){}

bool clsPlayerBase::atTarget(){}
