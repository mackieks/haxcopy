#include "AboutScreen.hpp"

AboutScreen::AboutScreen() {
    creditList.emplace_back("Developers:", "YveltalGriffin");
    creditList.emplace_back("", "Based on WiiUCrashLogDumper by Maschell\n and WiiUIdent by GaryOderNichts");

    fontList.emplace_back("Main Font:", "Wii U System Font");
    fontList.emplace_back("Icon Font:", "FontAwesome");
    fontList.emplace_back("Monospace Font:", "Terminus Font");

    linkList.emplace_back("", ScreenListElement{"https://github.com/mackieks/haxcopy", true});
}

AboutScreen::~AboutScreen() = default;

void AboutScreen::Draw() {
    DrawTopBar("About");

    int yOff = 128;
    yOff     = DrawHeader(32, yOff, 896, 0xf121, "Credits");
    yOff     = DrawList(32, yOff, 896, creditList);
    yOff     = DrawHeader(32, yOff, 896, 0xf031, "Fonts");
    yOff     = DrawList(32, yOff, 896, fontList);

    yOff = 128;
    yOff = DrawHeader(992, yOff, 896, 0xf08e, "Source Code");
    yOff = DrawList(992, yOff, 896, linkList);

    DrawBottomBar(nullptr, "\ue044 Exit", "\ue001 Back");
}

bool AboutScreen::Update(Input &input) {
    return !(input.data.buttons_d & Input::BUTTON_B);
}
