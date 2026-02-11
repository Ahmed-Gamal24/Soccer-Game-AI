#pragma once

#include "state.h"
#include "stateMachine.h"

// Forward declaration
class clsSoccerTeam;


class Attacking : public clsState<clsSoccerTeam>{


    public:
        void Enter(clsSoccerTeam* entity) override;
        void Execute(clsSoccerTeam* entity) override;
        void Exit(clsSoccerTeam* entity) override;
        bool OnMessage(clsSoccerTeam* entity, const clsTelegram& msg) override;


};

class Defending : public clsState<clsSoccerTeam>{
    public:
        void Enter(clsSoccerTeam* entity) override;
        void Execute(clsSoccerTeam* entity) override;
        void Exit(clsSoccerTeam* entity) override;
        bool OnMessage(clsSoccerTeam* entity, const clsTelegram& msg) override;


};


class PrepareForKickOff : public clsState<clsSoccerTeam>{    
    public:
        void Enter(clsSoccerTeam* entity) override;
        void Execute(clsSoccerTeam* entity) override;
        void Exit(clsSoccerTeam* entity) override;
        bool OnMessage(clsSoccerTeam* entity, const clsTelegram& msg) override;


};

