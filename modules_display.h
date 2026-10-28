#ifndef MODULES_DISPLAY
#define MODULES_DISPLAY

#include<string>
#include <nlohmann/json.hpp>

nlohmann::json loadConfig(const std::string& path);

std::string displayCPU(const nlohmann::json& config);
std::string displayUser(const nlohmann::json& config);
std::string displayOS(const nlohmann::json& config);
std::string displayMemory(const nlohmann::json& config);
std::string displayBox(const nlohmann::json& config, const std::string& name, int autoExtension);
int visibleWidth(const std::string& s);


#endif