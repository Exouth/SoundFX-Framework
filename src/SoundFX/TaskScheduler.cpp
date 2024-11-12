#include "TaskScheduler.h"
#include "SKSE/API.h"

TaskScheduler::TaskScheduler() : stopFlag(false) {
}

TaskScheduler::~TaskScheduler() {
    Stop();
}

void
    TaskScheduler::Start(int intervalMilliseconds) {
    stopFlag        = false;
    schedulerThread = std::thread([this, intervalMilliseconds]() {
        while (!stopFlag) {
            auto start = std::chrono::steady_clock::now();

            std::lock_guard<std::mutex> lock(taskMutex);
            for (auto &[task, repeat] : tasks) {
                SKSE::GetTaskInterface()->AddTask([task]() { std::thread(task).detach(); });

                if (!repeat) {
                    task = nullptr;
                }
            }
            tasks.erase(std::remove_if(tasks.begin(),
                                       tasks.end(),
                                       [](const auto &t) { return t.first == nullptr; }),
                        tasks.end());

            auto end = std::chrono::steady_clock::now();
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMilliseconds)
                                        - (end - start));
        }
    });
}

void
    TaskScheduler::Stop() {
    stopFlag = true;
    if (schedulerThread.joinable()) {
        schedulerThread.join();
    }
}

void
    TaskScheduler::AddTask(Task task, bool repeat) {
    std::lock_guard<std::mutex> lock(taskMutex);
    tasks.emplace_back(std::move(task), repeat);
}