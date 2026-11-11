// Ball Utils
// A collection of 8ball utils

#pragma once

#include "PoolTask.h"
#include "ThreadQueue.h"

typedef std::unique_ptr<PoolTask> Work;
typedef ThreadQueue<Work> WorkQueue;
