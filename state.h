#pragma once

#include "telegram.h"

template <class entity_type>
class clsState{


    public:
        virtual void Enter(entity_type* entity)= 0;
        virtual void Execute(entity_type* entity)= 0;
        virtual void Exit(entity_type* entity)= 0;
        virtual bool OnMessage(entity_type* entity, const clsTelegram& msg) = 0;

        virtual ~clsState(){}
};
