#pragma once
#include "Screen.hpp"
#include "common.h"
#include <dirent.h>
#include <queue>

class CopyScreen : public Screen {
public:
    CopyScreen();
    ~CopyScreen() override;

    void Draw() override;

    bool Update(Input &input) override;

private:
    enum CopyState {
        COPY_STATE_SELECT,
        COPY_STATE_ERROR,
        COPY_STATE_CREATE_DIRECTORY,
        COPY_STATE_OPEN_DIR,
        COPY_STATE_COPY_FILE,
        COPY_STATE_DONE,
    };
    CopyState mCopyState = COPY_STATE_SELECT;

    std::string mDestinationDirectory;
    std::string mSourceDirectory = HAX_SOURCE_PATH;
    DIR *mSourceDirectoryHandle        = nullptr;

    std::queue<std::string> mFilesToCopy;

    const int mDefaultXPos     = 32;
    const int mDefaultYPos     = 128;
    const int mDefaultFontSize = 56;

    void DrawSimpleText(const std::string &text) const;
    void DrawSimpleText(const std::vector<std::string> &texts) const;
};
