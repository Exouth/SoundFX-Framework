#include "MiscItemEventHandler.h"
#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        MiscItemEventHandler::ProcessEvent(const RE::TESContainerChangedEvent *event,
                                           RE::BSTEventSource<RE::TESContainerChangedEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents(
            {ProcessPickUpEvent(event), ProcessDropEvent(event)});
    }

    RE::BSEventNotifyControl
        MiscItemEventHandler::ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) {

        if (event == nullptr || event->newContainer == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObj);
        if (!item) {
            spdlog::warn("Failed to lookup item for PickUpEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &miscitems = jsonLoader.getItems("miscItems");
        for (const auto &[miscitemName, miscitemEvents] : miscitems) {
            const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                miscitemEvents.editorID, miscitemEvents.pluginName);

            if (resolvedFormID == item->formID) {
                for (const auto &jsonEvent : miscitemEvents.events) {
                    if (jsonEvent.type == "PickUp") {
                        float randomValue = static_cast<float>(rand()) / RAND_MAX;
                        if (randomValue <= jsonEvent.chance) {
                            PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        MiscItemEventHandler::ProcessDropEvent(const RE::TESContainerChangedEvent *event) {

        if (event == nullptr || event->newContainer != 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObj);
        if (!item) {
            spdlog::warn("Failed to lookup item for PickUpEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &miscitems = jsonLoader.getItems("miscItems");
        for (const auto &[miscitemName, miscitemEvents] : miscitems) {
            const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                miscitemEvents.editorID, miscitemEvents.pluginName);

            if (resolvedFormID == item->formID) {
                for (const auto &jsonEvent : miscitemEvents.events) {
                    if (jsonEvent.type == "Drop") {
                        float randomValue = static_cast<float>(rand()) / RAND_MAX;
                        if (randomValue <= jsonEvent.chance) {
                            PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}