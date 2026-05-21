#include "ThermalReader.hpp"
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

float ThermalReader::getTemperature() {
    std::string targetPath = "";

    try {
        for (const auto& entry : fs::directory_iterator("/sys/class/hwmon/")) {
            std::ifstream nameFile(entry.path() / "name");
            if (nameFile.is_open()) {
                std::string name;
                nameFile >> name;

                if (name == "coretemp" || name == "k10temp" || name == "zenpower") {
                    if (fs::exists(entry.path() / "temp1_input")) {
                        targetPath = entry.path().string() + "/temp1_input";
                        break;
                    }
                }
            }
        }
    } catch (...) {
    }

    if (targetPath.empty()) {
        targetPath = "/sys/class/thermal/thermal_zone0/temp";
    }

    std::ifstream file(targetPath);
    if (file.is_open()) {
        long milliCelsius;
        if (file >> milliCelsius) {
            return static_cast<float>(milliCelsius) / 1000.0f;
        }
    }

    return -1.0f;
}