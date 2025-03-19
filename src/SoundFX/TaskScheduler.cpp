#include "TaskScheduler.h"

namespace SoundFX {

    TaskScheduler::TaskScheduler() : stopFlag(false) {
    }

    TaskScheduler::~TaskScheduler() {
        Stop();
    }

    void
        TaskScheduler::Start(int intervalMilliseconds) {
        stopFlag        = false;
        schedulerThread = std::thread([this, intervalMilliseconds] {
            while (!stopFlag) {
                auto start = std::chrono::steady_clock::now();

                std::lock_guard lock(taskMutex);
                for (auto &[task, repeat] : tasks) {
                    // SKSE::GetTaskInterface()->AddTask([task]() { std::thread(task).detach(); });

                    // Tasks were sometimes executed consecutively within the same interval, causing
                    // a delay. By using std::async, each task runs independently
                    std::async(std::launch::async, [task, this] {
                        if (!stopFlag) {
                            task();
                        }
                    });

                    if (!repeat) {
                        task = nullptr;
                    }
                }
                std::erase_if(tasks, [](const auto &t) { return t.first == nullptr; });

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
        std::lock_guard lock(taskMutex);
        tasks.emplace_back(std::move(task), repeat);
    }
}