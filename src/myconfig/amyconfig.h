#ifndef AMYCONFIG_H
#define AMYCONFIG_H

#include <map>
#include <string>

class AMyConfig
{
public:
    explicit AMyConfig(std::string sFileName);
    ~AMyConfig();

    std::string getCfgValue(const std::string &sSection, const std::string &sKey, std::string sDefValue="");

private:
    bool initIniFile(const std::string &sFileName);

private:
    std::map<std::string, std::string> m_mpIni;
};


#endif  // AMYCONFIG_H