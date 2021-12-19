#include "amylog.h"
#include "amylockc.h"

AMyLog::AMyLog()
{
    
}

AMyLog::~AMyLog()
{

}

void AMyLog::setLogFileName(const std::string &sLogFileName)
{
    m_sLogFileName = sLogFileName;
}

void AMyLog::writeLogLevel(ELOGLEVEL logLevel, const std::string &sLog)
{
    // TODO：拼接日志字符串，map存储
    using std::string;
    string sLogFinalWrite;
    
    {
        AMyLockC m(m_mutex);
        m_lstLog.push_back(sLogFinalWrite);
    }
}

void logTrace(const std::string &sLog)
{
    AMyLog::getInstance().writeLogLevel(LL_TRACE, sLog);
}

void logDebug(const std::string &sLog)
{
    AMyLog::getInstance().writeLogLevel(LL_DEBUG, sLog);
}

void logInfo(const std::string &sLog)
{
    AMyLog::getInstance().writeLogLevel(LL_INFO, sLog);
}

void logWarning(const std::string &sLog)
{
    AMyLog::getInstance().writeLogLevel(LL_WARN, sLog);
}

void logError(const std::string &sLog)
{
    AMyLog::getInstance().writeLogLevel(LL_ERROR, sLog);
}