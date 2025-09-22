#include "reactiontimetester.h"

ReactionTimeTester::ReactionTimeTester(QObject *parent) : QObject(parent)
{

}


long long ReactionTimeTester::GetElapsedTime(void)
{
    return elapsedTimeInMilliseconds;
}

void ReactionTimeTester::ChangeState(enumState state)
{
    m.lock();
    fsmState = state;
    m.unlock();
}

enumState ReactionTimeTester::GetState(void)
{
    enumState ret;

    m.lock();
    ret = fsmState;
    m.unlock();

    return ret;
}

void ReactionTimeTester::Timer_UserReaction(std::promise<long long> promise)
{
    auto timeStart = std::chrono::steady_clock::now();

    while(runTimer)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    auto timeEnd = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart);
    promise.set_value(duration.count());
}

void ReactionTimeTester::Timer_Delay(uint32_t delay)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    ChangeState(waiting);
    emit NotifyUser();

    runTimer = true;
    promise = new std::promise<long long>();
    future = (*promise).get_future();
    timerThread = new std::thread(&ReactionTimeTester::Timer_UserReaction, this, std::move(*promise));
}

void ReactionTimeTester::EvaluateStatus(void)
{
    switch (GetState())
    {
        case stopped:
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> delay_0_to_xs(0, 5000);

            uint32_t delay = delay_0_to_xs(rng);

            elapsedTimeInMilliseconds = 0;

            delayThread = new std::thread (&ReactionTimeTester::Timer_Delay, this, delay);
            ChangeState(started);
            emit NewRound();
        }
        break;
        case started:
           //message user
            elapsedTimeInMilliseconds += 250;
            emit Penalty();
        break;
        case waiting:
        {
            runTimer = false;
            if ((*timerThread).joinable())
            {
                timerThread->join();
            }
            elapsedTimeInMilliseconds += future.get();

            delete(promise);
            emit SetResult();

            ChangeState(stopped);
        }
        break;
    }
}
