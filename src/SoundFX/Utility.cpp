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

    float
        CalculateMarkerSize(float distance, float minSize, float maxSize, float scaleFactor) {
        const float t = std::clamp(distance / scaleFactor, 0.0f, 1.0f);
        return std::lerp(maxSize, minSize, t);
    }

    ImU32
        ConvertColor(const ImVec4 &color) {
        return IM_COL32(static_cast<int>(color.x * 255.0f),
                        static_cast<int>(color.y * 255.0f),
                        static_cast<int>(color.z * 255.0f),
                        static_cast<int>(color.w * 255.0f));
    }

    bool
        NaturalStringCompare(const std::string &a, const std::string &b) {
        static const std::regex re(R"(\d+)");  // Regular expression for numbers

        auto aIt = std::sregex_iterator(a.begin(), a.end(), re);
        auto bIt = std::sregex_iterator(b.begin(), b.end(), re);

        std::sregex_iterator end;

        size_t lastPosA = 0, lastPosB = 0;
        while (aIt != end && bIt != end) {
            std::string aPart = a.substr(lastPosA, aIt->position() - lastPosA);
            std::string bPart = b.substr(lastPosB, bIt->position() - lastPosB);

            if (aPart != bPart)
                return aPart < bPart;

            int aNum = std::stoi(aIt->str());
            int bNum = std::stoi(bIt->str());

            if (aNum != bNum)
                return aNum < bNum;

            lastPosA = aIt->position() + aIt->length();
            lastPosB = bIt->position() + bIt->length();
            ++aIt;
            ++bIt;
        }

        return a.substr(lastPosA) < b.substr(lastPosB);
    }
}