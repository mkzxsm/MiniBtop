#include "TerminalUI.hpp"
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

const std::string TerminalUI::RESET = "\033[0m";
const std::string TerminalUI::BOLD = "\033[1m";
const std::string TerminalUI::GREEN = "\033[32m";
const std::string TerminalUI::CYAN = "\033[36m";
const std::string TerminalUI::RED = "\033[31m";
const std::string TerminalUI::YELLOW = "\033[33m";

void TerminalUI::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

int TerminalUI::getTerminalWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return (w.ws_col > 10) ? w.ws_col : 80;
}

std::string TerminalUI::repeatStr(const std::string& str, int count) {
    std::string result = "";
    for (int i = 0; i < count; ++i) result += str;
    return result;
}

std::string TerminalUI::centerText(const std::string& text, int width) {
    int padding = width - text.length();
    if (padding <= 0) return text;
    int leftPad = padding / 2;
    int rightPad = padding - leftPad;
    return std::string(leftPad, ' ') + text + std::string(rightPad, ' ');
}

std::string TerminalUI::drawBar(float percentage, int width) {
    int fillWidth = static_cast<int>(width * percentage);
    std::string bar = "";
    for (int i = 0; i < width; ++i) {
        if (i < fillWidth) bar += "█";
        else bar += "░";
    }
    return bar;
}