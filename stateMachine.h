#pragma once

#include"state.h"
#include"telegram.h"

template <class entity_type>
class clsStateMachine{

    private:
        entity_type* owner;
        clsState<entity_type>* previousState;
        clsState<entity_type>* currentState;
        clsState<entity_type>* globalState;
    
    public:

        clsStateMachine(entity_type* owner):owner(owner),
                                   currentState(nullptr),
                                   previousState(nullptr),
                                   globalState(nullptr){}

        void setCurrentState(clsState<entity_type>* state);
        void setPreviousState(clsState<entity_type>* state);
        void setGlobalState(clsState<entity_type>* state);

        clsState<entity_type>* getCurrentState()const;
        clsState<entity_type>* getPreviousState()const;
        clsState<entity_type>* getGlobalState()const;
        void updateStateMachine();
        void changeState(clsState<entity_type>* newState);
        bool handleMessage(const clsTelegram msg)const;
        bool revertToPreviousState();
        bool isInState(const clsState<entity_type>& state)const;
        virtual ~clsStateMachine();
};

#include"stateMachine.tpp"
