#pragma once

namespace SoundFX {

    /// A lightweight task scheduler to perform tasks periodically or repeatedly.
    /// This scheduler has been performance-tested on an Intel i9-14900K processor, managing 50
    /// tasks at the same time without consuming any noticeable CPU or RAM. FPS
    /// performance was unaffected even when running with 50 concurrent threads every second
    class TaskScheduler {
      public:
        using Task = std::function<void()>;

        TaskScheduler();
        ~TaskScheduler();

        TaskScheduler(const TaskScheduler &) = delete;
        TaskScheduler &
            operator=(const TaskScheduler &) = delete;
        TaskScheduler(TaskScheduler &&)      = delete;
        TaskScheduler &
            operator=(TaskScheduler &&) = delete;

        void
            Start(int intervalMilliseconds = 500);

        void
            Stop();

        void
            AddTask(Task task, bool repeat = false);

      private:
        std::atomic<bool>                  stopFlag;
        std::thread                        schedulerThread;
        std::vector<std::pair<Task, bool>> tasks;
        std::mutex                         taskMutex;
    };
}