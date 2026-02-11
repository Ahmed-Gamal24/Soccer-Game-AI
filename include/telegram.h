#pragma once

// Message types for the telegram messaging system
enum MessageType {
    Msg_ReceiveBall,
    Msg_SupportAttacker,
    Msg_GoHome,
    Msg_PassToMe,
    Msg_TeamReady
};

struct Telegram {
    int sender;
    int receiver;
    MessageType msg;
    double dispatchTime;
    void* extraInfo;

    Telegram() : sender(-1), receiver(-1), msg(Msg_ReceiveBall), dispatchTime(0), extraInfo(nullptr) {}
    Telegram(int s, int r, MessageType m, double t, void* info = nullptr) 
        : sender(s), receiver(r), msg(m), dispatchTime(t), extraInfo(info) {}
};

// Type alias for compatibility
using clsTelegram = Telegram;
