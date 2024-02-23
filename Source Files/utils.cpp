#include <filesystem>
#include <Windows.h>
#include <iostream>
#include <ShlObj.h>
#include <iomanip>
#include <sstream>
#include <utils.h>
#include <chrono>

std::string getCurrentDateTimeFileName() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    localtime_s(&localTime, &time);
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y%m%d_%H%M%S");
    return oss.str();
}
std::wstring getCurrentUserAppDataFolder() {
    wchar_t path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        return std::wstring(path);
    }
    else {
        return L"";
    }
}

namespace fs = std::filesystem;

bool backupSaveFile(const std::string& backupFolder, const std::string& steamId) {

    try {
        auto time = getCurrentDateTimeFileName();
        auto userAppDataRoaming = getCurrentUserAppDataFolder();
        fs::path steamFolder = std::string(userAppDataRoaming.begin(), userAppDataRoaming.end()) + "\\EldenRing\\" + steamId;
        auto src = steamFolder / "ER0000.sl2";
        auto dest = fs::path(backupFolder) / (time + "_ER0000.sl2");

        fs::copy_file(src, dest);
        src = steamFolder / "ER0000.sl2.bak";
        dest = fs::path(backupFolder) / (time + "_ER0000.sl2.bak");
        fs::copy_file(src, dest);
    }
    catch (std::exception& e) {
        std::cout << "Error while copying save: " << e.what() << std::endl;
    }

    return true;
}
