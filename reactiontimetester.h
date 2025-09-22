#ifndef REACTIONTIMETESTER_H
#define REACTIONTIMETESTER_H

#include <QObject>
#include <chrono>
#include <thread>
#include <future>
#include <QObject>
#include <random>

enum enumState { started, stopped, waiting };

class ReactionTimeTester : public QObject
{
    Q_OBJECT

    bool runTimer = false;
    enumState fsmState = stopped;
    long long elapsedTimeInMilliseconds;
    std::mutex m;
    std::promise<long long> * promise;
    std::future<long long> future;
    std::thread * timerThread;
    std::thread * delayThread;

    void ChangeState(enumState state);
    enumState GetState(void);
    void Timer_UserReaction(std::promise<long long> promise);
    void Timer_Delay(uint32_t delay);

public:
    explicit ReactionTimeTester(QObject *parent = nullptr);
    long long GetElapsedTime(void);
    void EvaluateStatus(void);

signals:
    void NewRound(void);
    void Penalty(void);
    void NotifyUser(void);
    void SetResult(void);
};

#endif // REACTIONTIMETESTER_H
