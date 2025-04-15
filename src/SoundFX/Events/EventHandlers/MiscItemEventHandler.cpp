#include "MiscItemEventHandler.h"
#include "Events/EventHandlerManager.h"
#include "Sound/SoundManager.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        MiscItemEventHandler::ProcessEvent(const RE::TESContainerChangedEvent *event,
                                           RE::BSTEventSource<RE::TESContainerChangedEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents(
            {ProcessPickUpEvent(event), ProcessDropEvent(event)});
    }

    RE::BSEventNotifyControl
        MiscItemEventHandler::ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) const {

        if (event == nullptr || event->newContainer == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObj);
        if (!item) {
            spdlog::warn("Failed to lookup item for PickUpEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        for (const auto &miscitems = jsonLoader->getItems("miscItems");
             const auto &miscitemEvents : miscitems | std::views::values) {
            const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                miscitemEvents.editorID, miscitemEvents.pluginName);

            if (resolvedFormID == item->formID) {
                for (const auto &jsonEvent : miscitemEvents.events) {
                    if (jsonEvent.type == "PickUp") {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            SoundManager::PlaySound(miscitemEvents.name,
                                                    jsonEvent.type,
                                                    jsonEvent.soundEffect,
                                                    {8029.8643f, -69089.37f, 4815.15f},
                                                    1000.0f,
                                                    jsonEvent.volume,
                                                    jsonEvent.isAbsoluteVolume,
                                                    true);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        MiscItemEventHandler::ProcessDropEvent(const RE::TESContainerChangedEvent *event) const {

        if (event == nullptr || event->newContainer != 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObj);
        if (!item) {
            spdlog::warn("Failed to lookup item for PickUpEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        for (const auto &miscitems = jsonLoader->getItems("miscItems");
             const auto &miscitemEvents : miscitems | std::views::values) {
            const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                miscitemEvents.editorID, miscitemEvents.pluginName);

            if (resolvedFormID == item->formID) {
                for (const auto &jsonEvent : miscitemEvents.events) {
                    if (jsonEvent.type == "Drop") {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            SoundManager::PlaySound(miscitemEvents.name,
                                                    jsonEvent.type,
                                                    jsonEvent.soundEffect,
                                                    {8029.8643f, -69089.37f, 4815.15f},
                                                    1000.0f,
                                                    jsonEvent.volume,
                                                    jsonEvent.isAbsoluteVolume,
                                                    true);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
}