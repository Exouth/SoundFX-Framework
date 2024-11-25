#include "NpcInteractionEventHandler.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    void
        NpcInteractionEventHandler::SetupNpcInteractionTasks() {
        StartNpcInteractionTask([this]() { ProcessDialogOpenTask(); }, true);
        scheduler.Start(500);  // Run every 0.5 Second
    }

    void
        NpcInteractionEventHandler::ProcessDialogOpenTask() {
    }
}