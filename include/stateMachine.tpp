#pragma once

#include"stateMachine.h"

template <class entity_type>
void clsStateMachine<entity_type>:: setCurrentState(clsState<entity_type>* state){
    currentState = state;
}


template <class entity_type>
void clsStateMachine<entity_type>::setPreviousState(clsState<entity_type>* state){
    previousState = state;
}

template <class entity_type>
void clsStateMachine<entity_type>::setGlobalState(clsState<entity_type>* state){
    globalState = state;
}

template <class entity_type>
clsState<entity_type>* clsStateMachine<entity_type>::getCurrentState()const{
    return currentState;
}

template <class entity_type>
clsState<entity_type>* clsStateMachine<entity_type>::getPreviousState()const{
    return previousState;
}

template <class entity_type>
clsState<entity_type>* clsStateMachine<entity_type>::getGlobalState()const{
    return globalState;
}

template <class entity_type>
void clsStateMachine<entity_type>::updateStateMachine(){
    if(globalState){
        globalState->Execute(owner);
    }
    if(currentState){
        currentState->Execute(owner);
    }
}

template <class entity_type>
void clsStateMachine<entity_type>::changeState(clsState<entity_type>* newState){
    if(!newState) return;
    
    previousState = currentState;
    if(currentState){
        currentState->Exit(owner);
    }
    currentState = newState;
    currentState->Enter(owner);
}

template <class entity_type>
bool clsStateMachine<entity_type>::handleMessage(const clsTelegram msg)const{
    //first check if the global state is valid and has a message to handle
    if(globalState && globalState->OnMessage(owner, msg)){
        return true;
    }
    //then check if the current state is valid and has a message to handle
    if(currentState && currentState->OnMessage(owner, msg)){
        return true;
    }
    return false;
}

template <class entity_type>
bool clsStateMachine<entity_type>::revertToPreviousState(){
    changeState(previousState);
    return true;
}

template <class entity_type>
bool clsStateMachine<entity_type>::isInState(const clsState<entity_type>& state)const{
    return currentState == &state;
}

template <class entity_type>
clsStateMachine<entity_type>::~clsStateMachine(){
    delete currentState;
    delete previousState;
    delete globalState;
}