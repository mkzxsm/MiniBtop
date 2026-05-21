#include "MiniBtopApp.hpp"
#include "TerminalUI.hpp"
#include "MemoryReader.hpp"
#include "ThermalReader.hpp"
#include "ProcessReader.hpp"
#include "SystemInfoReader.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include <csignal>
#include <cstdlib>

void handleExit(int signum) {
    std::cout << "\033[?25h\033[?1049l" << std::flush;
    exit(signum);
}

MiniBtopApp::MiniBtopApp() : isRunning(true) {}

void MiniBtopApp::run() {
    signal(SIGINT, handleExit);

    std::cout << "\033[?1049h\033[?25l" << std::flush;

    TerminalUI::clearScreen();

    auto fmt = [](float val, int prec) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(prec) << val;
        return ss.str();
    };

    while (isRunning) {
        int w = TerminalUI::getTerminalWidth();
        int h = TerminalUI::getTerminalHeight();

        int mid = w / 2;

        int processRows = std::max(1, h - 15);
        int maxProcesses = processRows * 2;

        float cpuUsage = cpuReader.getUsage();
        MemStats memStats = MemoryReader::getStats();
        float currentTemp = ThermalReader::getTemperature();
        NetStats netStats = netReader.getSpeed();
        SysInfo sysInfo = SystemInfoReader::getInfo();
        auto topProcesses = ProcessReader::getTopMemoryProcesses(maxProcesses);

        int barW = std::max(5, mid - 18);

        std::cout << "\033[1;1H";

        auto printBorder = [&](const std::string& left, const std::string& midChar, const std::string& right) {
            std::cout << "\033[2K\r"
                      << TerminalUI::CYAN << left << TerminalUI::repeatStr("─", mid - 2)
                      << midChar << TerminalUI::repeatStr("─", w - mid - 1)
                      << right << "\n" << TerminalUI::RESET;
        };

        auto print2Col = [&](const std::string& c1, const std::string& c2) {
            std::cout << "\033[2K\r"
                      << TerminalUI::CYAN << "│"
                      << "\033[3G" << TerminalUI::RESET << c1
                      << "\033[" << mid << "G" << TerminalUI::CYAN << "│"
                      << "\033[" << mid + 2 << "G" << TerminalUI::RESET << c2
                      << "\033[" << w << "G" << TerminalUI::CYAN << "│\n" << TerminalUI::RESET;
        };

        auto print1Col = [&](const std::string& text) {
            std::cout << "\033[2K\r"
                      << TerminalUI::CYAN << "│"
                      << "\033[3G" << TerminalUI::RESET << text
                      << "\033[" << w << "G" << TerminalUI::CYAN << "│\n" << TerminalUI::RESET;
        };

        printBorder("╭", "┬", "╮");
        print2Col(TerminalUI::BOLD + TerminalUI::YELLOW + "CPU & Memory", TerminalUI::BOLD + TerminalUI::YELLOW + "System & Network");
        printBorder("├", "┼", "┤");

        std::string cpuStr = TerminalUI::BOLD + "CPU " + TerminalUI::drawBar(cpuUsage, barW) + " " + fmt(cpuUsage * 100, 1) + "%";
        std::string userStr = TerminalUI::BOLD + "USER   " + TerminalUI::WHITE + sysInfo.username + "@" + sysInfo.hostname;
        print2Col(cpuStr, userStr);
        print2Col("", "");

        std::string ramStr = TerminalUI::BOLD + "RAM " + TerminalUI::drawBar(memStats.usagePercentage, barW) + " " + fmt(memStats.used / 1024.0f, 1) + "G";
        std::string upStr = TerminalUI::BOLD + "UPTIME " + TerminalUI::WHITE + sysInfo.uptime;
        print2Col(ramStr, upStr);
        print2Col("", "");

        float tempPercent = std::max(0.0f, std::min(currentTemp / 100.0f, 1.0f));
        std::string tmpStr = TerminalUI::BOLD + "TMP " + TerminalUI::drawBar(tempPercent, barW) + " " + fmt(currentTemp, 0) + "°C";
        std::string netDlStr = TerminalUI::BOLD + "NET ▼  " + TerminalUI::GREEN + fmt(netStats.rxSpeedMB, 2) + " MB/s";
        print2Col(tmpStr, netDlStr);

        std::string netUlStr = TerminalUI::BOLD + "NET ▲  " + TerminalUI::RED + fmt(netStats.txSpeedMB, 2) + " MB/s";
        print2Col("", netUlStr);

        printBorder("├", "┴", "┤");
        print1Col(TerminalUI::BOLD + TerminalUI::YELLOW + "Top Processes");
        printBorder("├", "┬", "┤");

        int L = mid - 3;
        int R = w - mid - 2;

        auto formatProc = [&](const ProcessInfo& p, int maxW) {
            if (p.pid == 0) return std::string("");

            int pidW = 7, memW = 7, pctW = 6;
            int nameW = std::max(4, maxW - pidW - memW - pctW);

            std::string name = p.name;
            if (name.length() > static_cast<size_t>(nameW)) name = name.substr(0, nameW - 1) + "…";
            else name += std::string(nameW - name.length(), ' ');

            std::string pidStr = std::to_string(p.pid);
            pidStr += std::string(pidW - pidStr.length(), ' ');

            std::string memStr = std::to_string(p.memoryMB) + "M";
            std::string memPad = std::string(memW - memStr.length(), ' ');

            float pct = (p.memoryMB / (float)memStats.total) * 100.0f;
            std::string pctStr = fmt(pct, 1) + "%";
            std::string pctPad = std::string(pctW - pctStr.length(), ' ');

            return TerminalUI::CYAN + pidStr + TerminalUI::RESET +
                   TerminalUI::WHITE + TerminalUI::BOLD + name + TerminalUI::RESET +
                   memPad + TerminalUI::YELLOW + memStr + TerminalUI::RESET +
                   pctPad + TerminalUI::GREEN + pctStr + TerminalUI::RESET;
        };

        auto formatHeader = [&](int maxW) {
            int pidW = 7, memW = 7, pctW = 6;
            int nameW = std::max(4, maxW - pidW - memW - pctW);

            std::string h = "PID"; h += std::string(pidW - h.length(), ' ');
            std::string n = "NAME"; n += std::string(nameW - n.length(), ' ');
            std::string m = "MEM"; m = std::string(memW - m.length(), ' ') + m;
            std::string p = "%"; p = std::string(pctW - p.length(), ' ') + p;

            return TerminalUI::BOLD + TerminalUI::WHITE + h + n + m + p + TerminalUI::RESET;
        };

        print2Col(formatHeader(L), formatHeader(R));

        for (int i = 0; i < processRows; ++i) {
            ProcessInfo pL = (i < topProcesses.size()) ? topProcesses[i] : ProcessInfo{0, "", 0};
            ProcessInfo pR = (i + processRows < topProcesses.size()) ? topProcesses[i + processRows] : ProcessInfo{0, "", 0};

            print2Col(formatProc(pL, L), formatProc(pR, R));
        }

        std::cout << "\033[2K\r" << TerminalUI::CYAN << "╰" << TerminalUI::repeatStr("─", mid - 2)
                  << "┴" << TerminalUI::repeatStr("─", w - mid - 1) << "╯" << TerminalUI::RESET;

        std::cout << "\033[J" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}