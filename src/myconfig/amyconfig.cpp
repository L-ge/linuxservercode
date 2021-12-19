#include "amyconfig.h"

#include <fstream>

AMyConfig::AMyConfig(std::string sFileName)
{
    initIniFile(sFileName);
}

AMyConfig::~AMyConfig()
{

}

std::string AMyConfig::getCfgValue(const std::string &sSection, const std::string &sKey, std::string sDefValue)
{
    using std::string;
    string sKeyTmp = sSection + "/" + sKey;
    if(m_mpIni.count(sKeyTmp) > 0)
        return m_mpIni[sKeyTmp];
    return sDefValue;
}

bool AMyConfig::initIniFile(const std::string &sFileName)
{
    using namespace std;
    ifstream file(sFileName.c_str());
    if(false == file.is_open())
        return false;

    string sLine, sSection;
    while (getline(file, sLine))
    {
        int nLenCurLine = sLine.length();
        if(nLenCurLine > 0)
        {
            if(0 == sLine.find('[') && nLenCurLine-1 == sLine.find(']'))
            {
                sSection = sLine.substr(1, nLenCurLine-2);
            }
            else if(string::npos != sLine.find('='))
            {
                string sKey = sLine.substr(0, sLine.find('='));
                string sValue = sLine.substr(sLine.find('=')+1);
                string sKeyTmp = sSection + "/" + sKey;
                m_mpIni[sKeyTmp] = sValue;
            }
        }
    }
    
    return true;
}