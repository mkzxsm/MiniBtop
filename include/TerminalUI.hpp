#pragma once
#include <string>

class TerminalUI {
public:
    static const std::string RESET;
    static const std::string BOLD;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string RED;
    static const std::string CYAN;
    static const std::string WHITE;
    static const std::string DARK_GRAY;

    static void clearScreen();
    static int getTerminalWidth();
    static int getTerminalHeight();
    static std::string repeatStr(const std::string& str, int count);
    static std::string centerText(const std::string& text, int width);
    static std::string drawBar(float percentage, int width);
};