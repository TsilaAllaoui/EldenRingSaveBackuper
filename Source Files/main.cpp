#include <filesystem>
#include <iostream>
#include <fstream>
#include <utils.h>

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
    ///     1 - Getting paths infos
    std::ifstream config("config.cfg");
    if (!config) {
        std::cout << "Config file not found!" << std::endl;
        return 1;
    }

    std::string backupFolder;
    std::getline(config, backupFolder);
    backupFolder = backupFolder.substr(13, -1);
    if (!fs::exists(backupFolder)) {
        try {
            fs::create_directories(backupFolder);
        }
        catch (...) {
            std::cout << "Invalid backup folder path!" << std::endl;
            return 1;
        }
    }
    std::cout << "Backup folder Path: \"" << backupFolder << "\"." << std::endl;

    std::string backupIntervalString;
    auto backupInterval = 30;
    std::getline(config, backupIntervalString);
    backupIntervalString = backupIntervalString.substr(15, -1);
    try {
        backupInterval = std::stoi(backupIntervalString);
        std::cout << "Backup will be made every " << backupInterval << " minutes." << std::endl;
    }
    catch (...) {
        std::cout << "Invalid Backup interval! Backup will be made every 30 minutes!" << std::endl;
    }

    std::string steamId;
    std::getline(config, steamId);
    steamId = steamId.substr(8, -1);
    auto userAppDataRoaming = getCurrentUserAppDataFolder();
    auto steamFolder = std::string(userAppDataRoaming.begin(), userAppDataRoaming.end());
    if (steamId.size() != 17 || !fs::exists(steamFolder + std::string("\\EldenRing\\" + steamId))) {
        std::cout << "Invalid SteamID!" << std::endl;
        return 1;
    }
    std::cout << "SteamID: \"" << backupFolder << "\"." << std::endl;

    ///     2 - Backing up file every backupInterval amount of minutes
    while (true) {
        if (!backupSaveFile(backupFolder, steamId)) {
            std::cout << "Error while backing up save file! Verify available space!" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::minutes(backupInterval));
    }

    return 0;
}
