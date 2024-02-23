#pragma once

std::string getCurrentDateTimeFileName();

std::wstring getCurrentUserAppDataFolder();

bool backupSaveFile(const std::string& backupFolder, const std::string& steamId);
