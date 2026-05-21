#include "TerminalUI.hpp"
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

const std::string TerminalUI::RESET = "\033[0m";
const std::string TerminalUI::BOLD = "\033[1m";
const std::string TerminalUI::GREEN = "\033[32m";
const std::string TerminalUI::YELLOW = "\033[33m";
const std::string TerminalUI::RED = "\033[31m";
const std::string TerminalUI::CYAN = "\033[36m";
const std::string TerminalUI::WHITE = "\033[37m";
const std::string TerminalUI::DARK_GRAY = "\033[90m";

void TerminalUI::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

int TerminalUI::getTerminalWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return (w.ws_col > 10) ? w.ws_col : 80;
}

int TerminalUI::getTerminalHeight() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return (w.ws_row > 10) ? w.ws_row : 24;
}

std::string TerminalUI::repeatStr(const std::string& str, int count) {
    if (count <= 0) return "";
    std::string result = "";
    for (int i = 0; i < count; ++i) result += str;
    return result;
}

std::string TerminalUI::centerText(const std::string& text, int width) {
    if (text.length() >= static_cast<size_t>(width)) return text;
    int leftPad = (width - text.length()) / 2;
    int rightPad = width - text.length() - leftPad;
    return std::string(leftPad, ' ') + text + std::string(rightPad, ' ');
}

std::string TerminalUI::drawBar(float percentage, int width) {
    if (width <= 0) return "";
    int fillWidth = static_cast<int>(width * percentage);
    std::string bar = "";

    std::string color = (percentage > 0.8f) ? RED : ((percentage > 0.4f) ? YELLOW : GREEN);

    bar += color;
    for (int i = 0; i < fillWidth; ++i) bar += "█";

    bar += DARK_GRAY;
    for (int i = fillWidth; i < width; ++i) bar += "░";

    bar += RESET;
    return bar;
}