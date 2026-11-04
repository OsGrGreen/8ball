// Thread worker

#pragma once

#include "BallUtils.h"

class ThreadWorker {
public:
    ThreadWorker() {};

private:
    WorkQueue m_workerQueue;
};