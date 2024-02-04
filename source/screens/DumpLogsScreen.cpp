#include "DumpLogsScreen.hpp"
#include "Gfx.hpp"
#include "common.h"
#include "utils/Utils.hpp"
#include <ctime>

DumpLogsScreen::DumpLogsScreen() {
    mDestinationDirectory = HAX_DESTINATION_PATH;
}

DumpLogsScreen::~DumpLogsScreen() = default;

bool subdirectoryExists = false;


void DumpLogsScreen::DrawSimpleText(const std::string &text) const {
    Gfx::Print(Gfx::SCREEN_WIDTH / 2, Gfx::SCREEN_HEIGHT / 2, mDefaultFontSize, Gfx::COLOR_TEXT, text, Gfx::ALIGN_CENTER);
}

void DumpLogsScreen::DrawSimpleText(const std::vector<std::string> &items) const {
    int yOff = mDefaultYPos;
    for (const auto &item : items) {
        Gfx::Print(mDefaultXPos, yOff, mDefaultFontSize, Gfx::COLOR_TEXT, item, Gfx::ALIGN_VERTICAL);
        yOff += Gfx::GetTextHeight(mDefaultFontSize, item);
    }
}

void DumpLogsScreen::Draw() {
    DrawTopBar("Copy /hax to SLC");
    switch (mDumpState) {
        case DUMP_STATE_SELECT:
            DrawSimpleText("Press \ue000 to copy hax folder to SLC");
            break;
        case DUMP_STATE_ERROR:
            DrawSimpleText("Copying failed. Press \ue001 to return");
            break;
        case DUMP_STATE_CREATE_DIRECTORY:
        case DUMP_STATE_OPEN_DIR:
            DrawSimpleText("Preparing to copy...");
            break;
        case DUMP_STATE_COPY_FILE: {
            if (!mFilesToCopy.empty()) {
                std::string sourcePath = "sd:/hax/" + mFilesToCopy.front();
                std::string log        = Utils::sprintf("Copying %s", sourcePath.c_str());
                DrawSimpleText(log);
            } else {
                DrawSimpleText("Copying files...");
            }
            break;
        }
        case DUMP_STATE_DUMPING_DONE: {
            auto userFriendlyDestinationPath = "slc:/sys/hax/";
            DrawSimpleText((std::vector<std::string>){"Copying done!",
                                                      "",
                                                      "Files copied to:",
                                                      userFriendlyDestinationPath});
            break;
        }
    }

    DrawBottomBar(nullptr, "\ue044 Exit", "\ue001 Back");
}

bool DumpLogsScreen::Update(Input &input) {
    switch (mDumpState) {
        case DUMP_STATE_SELECT: {
            if (input.data.buttons_d & Input::BUTTON_B) {
                return false;
            } else if (input.data.buttons_d & Input::BUTTON_A) {
                mDumpState = DUMP_STATE_CREATE_DIRECTORY;
            }
            break;
        }
        case DUMP_STATE_CREATE_DIRECTORY:
            if (!Utils::CreateSubfolder(mDestinationDirectory)) {
                mDumpState = DUMP_STATE_ERROR;
                break;
            }
            mDumpState = DUMP_STATE_OPEN_DIR;
            break;
        case DUMP_STATE_OPEN_DIR:
            mSourceDirectoryHandle = opendir(mSourceDirectory.c_str());
            if (mSourceDirectoryHandle == nullptr) {
                mDumpState = DUMP_STATE_ERROR;
                break;
            }
            struct dirent *dp;
            while ((dp = readdir(mSourceDirectoryHandle)) != nullptr) {
                std::string sourceFullPath = mSourceDirectory + "/" + dp->d_name;
                struct stat filestat {};
                if (stat(sourceFullPath.c_str(), &filestat) < 0 ||
                    (filestat.st_mode & S_IFMT) == S_IFDIR) { // if directory
                    subdirectoryExists = true;
                    continue;
                }
                mFilesToCopy.emplace(dp->d_name);
            }
            mDumpState = DUMP_STATE_COPY_FILE;
            break;
        case DUMP_STATE_COPY_FILE: {
            if (!mFilesToCopy.empty()) {
                std::string filename = mFilesToCopy.front();
                mFilesToCopy.pop();

                std::string sourceFullPath      = mSourceDirectory + "/" + filename;
                std::string destinationFullPath = mDestinationDirectory + "/" + filename;
                if (!Utils::CopyFile(sourceFullPath, destinationFullPath)) {
                    mDumpState = DUMP_STATE_ERROR;
                }
                break;
            } else {
                if (subdirectoryExists) {   // copying done and we have subdir to enter
                    mSourceDirectory      = HAX_PLUGINS_SOURCE_PATH;
                    mDestinationDirectory = HAX_PLUGINS_DESTINATION_PATH;
                    subdirectoryExists    = false;
                    mDumpState            = DUMP_STATE_CREATE_DIRECTORY;
                    break;
                }
                else mDumpState = DUMP_STATE_DUMPING_DONE;
                break;
            }
        }
        case DUMP_STATE_DUMPING_DONE:
        case DUMP_STATE_ERROR:
            if (input.data.buttons_d & Input::BUTTON_B) {
                return false;
            }
            break;
    }

    return true;
}
