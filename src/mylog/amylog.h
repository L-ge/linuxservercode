#ifndef AMYLOG_H
#define AMYLOG_H

#include <list>
#include <string>
#include <mutex>

enum ELOGLEVEL
{
	LL_TRACE=0, 
    LL_DEBUG, 
    LL_INFO, 
    LL_WARN, 
    LL_ERROR,
};

class AMyLog
{
public:
    ~AMyLog();
    static AMyLog &getInstance()
    {
        static AMyLog instance;
        return instance;
    }

    void setLogFileName(const std::string &sLogFileName);
    void writeLogLevel(ELOGLEVEL logLevel, const std::string &sLog);

private:
    AMyLog();

private:
    

private:
    std::string m_sLogFileName;
    std::list<std::string> m_lstLog;
    std::mutex m_mutex;
};

extern void logTrace(const std::string &sLog);
extern void logDebug(const std::string &sLog);
extern void logInfo(const std::string &sLog);
extern void logWarning(const std::string &sLog);
extern void logError(const std::string &sLog);

#endif  // AMYLOG_H