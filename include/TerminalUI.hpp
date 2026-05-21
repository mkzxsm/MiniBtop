#pragma once
#include <string>

class TerminalUI {
public:
    static const std::string RESET;
    static const std::string BOLD;
    static const std::string GREEN;
    static const std::string CYAN;
    static const std::string RED;
    static const std::string YELLOW;

    static void clearScreen();

    static int getTerminalWidth();
    static std::string repeatStr(const std::string& str, int count);
    static std::string centerText(const std::string& text, int width);
    static std::string drawBar(float percentage, int width);
};