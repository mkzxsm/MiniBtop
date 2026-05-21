#include "MiniBtopApp.hpp"
#include "TerminalUI.hpp"
#include "MemoryReader.hpp"
#include "ThermalReader.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>

MiniBtopApp::MiniBtopApp() : isRunning(true) {}

void MiniBtopApp::run() {
    TerminalUI::clearScreen();

    while (isRunning) {
        float cpuUsage = cpuReader.getUsage();
        MemStats memStats = MemoryReader::getStats();
        float currentTemp = ThermalReader::getTemperature();

        int termWidth = TerminalUI::getTerminalWidth();
        int contentWidth = std::max(0, termWidth - 4);

        std::string cpuColor = (cpuUsage > 0.8) ? TerminalUI::RED : ((cpuUsage > 0.4) ? TerminalUI::YELLOW : TerminalUI::GREEN);
        std::string memColor = (memStats.usagePercentage > 0.8) ? TerminalUI::RED : ((memStats.usagePercentage > 0.4) ? TerminalUI::YELLOW : TerminalUI::GREEN);

        std::cout << "\033[1;1H";

        std::cout << TerminalUI::CYAN;
        std::cout << "╭" << TerminalUI::repeatStr("─", std::max(0, termWidth - 2)) << "╮\n";
        std::string title = (termWidth < 15) ? "BTOP" : "MINI-BTOP";
        std::cout << "│" << TerminalUI::BOLD << TerminalUI::centerText(title, std::max(0, termWidth - 2)) << TerminalUI::RESET << TerminalUI::CYAN << "│\n";
        std::cout << "├" << TerminalUI::repeatStr("─", std::max(0, termWidth - 2)) << "┤\n" << TerminalUI::RESET;

        std::stringstream cpuStream;
        if (termWidth > 35) cpuStream << "CPU Usage: ";
        else cpuStream << "CPU: ";

        cpuStream << std::fixed << std::setprecision(1) << (cpuUsage * 100) << "%";

        if (currentTemp >= 0) {
            std::string tempColor = (currentTemp > 80.0f) ? TerminalUI::RED : ((currentTemp > 65.0f) ? TerminalUI::YELLOW : TerminalUI::GREEN);

            if (termWidth > 35) {
                cpuStream << "   Temp: " << tempColor << std::fixed << std::setprecision(1) << currentTemp << " °C" << TerminalUI::RESET << TerminalUI::BOLD;
            } else if (termWidth > 20) {
                cpuStream << "  " << tempColor << std::fixed << std::setprecision(0) << currentTemp << "°C" << TerminalUI::RESET << TerminalUI::BOLD;
            }
        }

        std::cout << "│ " << TerminalUI::BOLD << cpuStream.str() << TerminalUI::RESET;
        std::cout << "\033[K\r\033[" << std::max(0, termWidth - 1) << "C│\n";

        if (contentWidth > 0) std::cout << "│ " << cpuColor << TerminalUI::drawBar(cpuUsage, contentWidth) << TerminalUI::RESET << " │\n";
        else std::cout << "│\033[K\r\033[" << std::max(0, termWidth - 1) << "C│\n";

        std::cout << "│\033[K\r\033[" << std::max(0, termWidth - 1) << "C│\n";

        std::stringstream ramStream;
        if (termWidth >= 45) {
            ramStream << "RAM Usage: " << memStats.used << " MB / " << memStats.total << " MB (" << std::fixed << std::setprecision(1) << (memStats.usagePercentage * 100) << "%)";
        } else if (termWidth >= 30) {
            ramStream << "RAM: " << (memStats.used / 1024) << "GB/" << (memStats.total / 1024) << "GB (" << std::fixed << std::setprecision(0) << (memStats.usagePercentage * 100) << "%)";
        } else {
            ramStream << "RAM: " << std::fixed << std::setprecision(1) << (memStats.usagePercentage * 100) << "%";
        }

        std::string ramStr = ramStream.str();
        if (ramStr.length() > static_cast<size_t>(std::max(0, termWidth - 4))) ramStr = ramStr.substr(0, std::max(0, termWidth - 4));

        std::cout << "│ " << TerminalUI::BOLD << ramStr << TerminalUI::RESET;
        std::cout << "\033[K\r\033[" << std::max(0, termWidth - 1) << "C│\n";

        if (contentWidth > 0) std::cout << "│ " << memColor << TerminalUI::drawBar(memStats.usagePercentage, contentWidth) << TerminalUI::RESET << " │\n";
        else std::cout << "│\033[K\r\033[" << std::max(0, termWidth - 1) << "C│\n";


        std::cout << TerminalUI::CYAN;
        std::cout << "╰" << TerminalUI::repeatStr("─", std::max(0, termWidth - 2)) << "╯\n" << TerminalUI::RESET;
        if (termWidth > 30) std::cout << "  Press Ctrl+C for exit...\n";
        else std::cout << "  Ctrl+C to exit\n";

        std::cout << "\033[J";

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}