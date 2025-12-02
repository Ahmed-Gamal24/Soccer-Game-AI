#pragma once

#include "state.h"
#include "fieldPlayer.h"
#include "stateMachine.h"

// Forward declaration
class clsFieldPlayer;

class GlobalPlayerState : public clsState<clsFieldPlayer> {
public:
    void Enter(clsFieldPlayer* entity) override;
    void Execute(clsFieldPlayer* entity) override;
    void Exit(clsFieldPlayer* entity) override;
    bool OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) override;
};

class ChaseBall : public clsState<clsFieldPlayer> {
public:
    void Enter(clsFieldPlayer* entity) override;
    void Execute(clsFieldPlayer* entity) override;
    void Exit(clsFieldPlayer* entity) override;
    bool OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) override;
};

class Dribble : public clsState<clsFieldPlayer> {
public:
    void Enter(clsFieldPlayer* entity) override;
    void Execute(clsFieldPlayer* entity) override;
    void Exit(clsFieldPlayer* entity) override;
    bool OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) override;
};

class ReceiveBall : public clsState<clsFieldPlayer> {
public:
    void Enter(clsFieldPlayer* entity) override;
    void Execute(clsFieldPlayer* entity) override;
    void Exit(clsFieldPlayer* entity) override;
    bool OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) override;
};

class SupportAttacker : public clsState<clsFieldPlayer> {
public:
    void Enter(clsFieldPlayer* entity) override;
    void Execute(clsFieldPlayer* entity) override;
    void Exit(clsFieldPlayer* entity) override;
    bool OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) override;
};

class ReturnToHomeRegion : public clsState<clsFieldPlayer> {
public:
    void Enter(clsFieldPlayer* entity) override;
    void Execute(clsFieldPlayer* entity) override;
    void Exit(clsFieldPlayer* entity) override;
    bool OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) override;
};

class Wait : public clsState<clsFieldPlayer> {
public:
    void Enter(clsFieldPlayer* entity) override;
    void Execute(clsFieldPlayer* entity) override;
    void Exit(clsFieldPlayer* entity) override;
    bool OnMessage(clsFieldPlayer* entity, const clsTelegram& msg) override;
};

