#include"playerBase.h"

clsPlayerBase::clsPlayerBase():clsMovingEntity(clsBaseGameEntity::nextValidId++,
 clsVector2d(0, 0), 10, clsVector2d(1, 1), 1, 10, 10, 10){}

clsPlayerBase::clsPlayerBase(int id):clsMovingEntity(id,
 clsVector2d(0, 0), 10, clsVector2d(1, 1), 1, 10, 10, 10){}

bool clsPlayerBase::canPassForward(clsFieldPlayer receiver,clsFieldPlayer target, int power){}

bool clsPlayerBase::canPassBackward(clsFieldPlayer receiver,clsFieldPlayer target, int power){}

bool clsPlayerBase::withinShootingRange(){}

bool clsPlayerBase::atTarget(){}
