#include "Utility.h"

namespace SoundFX {

    std::string
        FormIDToString(RE::FormID formID, bool asHex) {
        std::stringstream stream;
        stream << std::uppercase << std::setfill('0') << std::setw(8);
        if (asHex) {
            stream << std::hex;
        }
        stream << formID;
        return stream.str();
    }

    std::vector<RE::TESForm *>
        GetAllFormsByEditorID(const std::string &editorID) {
        std::vector<RE::TESForm *> formsWithEditorID;

        const auto &[formsMap, lock] = RE::TESForm::GetAllFormsByEditorID();

        if (!formsMap) {
            return formsWithEditorID;
        }

        for (const auto &[key, form] : *formsMap) {
            if (std::string_view(key) == editorID && form) {
                formsWithEditorID.push_back(form);
            }
        }

        return formsWithEditorID;
    }

    RE::FormID
        GetFormIDFromEditorIDAndPluginName(const std::string &editorID,
                                           const std::string &pluginName) {
        const auto forms = GetAllFormsByEditorID(editorID);

        if (forms.empty()) {
            return 0;
        }

        for (const auto *form : forms) {
            if (!form) {
                continue;
            }

            const auto *formFile = form->GetFile();

            if (formFile && formFile->GetFilename() == pluginName) {
                return form->GetFormID();  // Exact match with pluginName
            }

            // Use master record instead as fallback
            if (formFile && forms.size() == 1) {
                spdlog::debug("Using Fallback Master Record: {:08X} in plugin '{}'",
                              form->GetFormID(),
                              pluginName);
                return form->GetFormID();
            }
        }

        return 0;
    }

    void
        DelayExec(float delayInSeconds, const std::function<void()> &task) {

        std::thread([delayInSeconds, task] {
            std::this_thread::sleep_for(std::chrono::duration<float>(delayInSeconds));

            SKSE::GetTaskInterface()->AddTask(task);
        }).detach();
    }

    float
        GenerateRandomFloat() {
        thread_local std::mt19937      generator(std::random_device {}());
        std::uniform_real_distribution distribution(0.0f, 1.0f);
        return distribution(generator);
    }

}