#include "dbgout.h"
#include <sstream>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <iomanip>


namespace dbg
{
    dbgout::dbgout() : m_pos(&std::cout),
                       m_defMsgLevel(DBG_DEBUG),
                       m_begin(true),
                       m_dbgLevel(DBG_DEBUG)
    {
        m_curMsgLevel = m_defMsgLevel;
    }

    dbgout::~dbgout()
    {
        if(!m_begin)
        {
            *m_pos << std::endl;
        }
    }

    void dbgout::redirect(std::ostream &os)
    {
        m_pos = &os;
    }

    dbgout& dbgout::flush(dbgout& so, dbgLevel_t msglev)
    {
        if(!so.m_begin)
        {
            *so.m_pos << std::endl;
            so.m_begin = true;
        }
        so.m_curMsgLevel = msglev;
        return so;
    }

    dbgout & dbgout::operator<< (dbgout & (*_f)(dbgout &))
    {
        return(*_f)(*this);
    }

    void dbgout::setDbgLevel(dbgLevel_t level)
    {
        m_dbgLevel = level;
    }

    void dbgout::setDefaultMsgLevel(dbgLevel_t level)
    {
        if(level < DBG_NOTSET)
        {
            m_defMsgLevel = level;
        }
    }

    dbgLevel_t dbgout::getDefaultMsgLevel() const
    {
        return m_defMsgLevel;
    }

    dbgLevel_t dbgout::getCurrentMsgLevel() const
    {
        return m_curMsgLevel;
    }

    std::string dbgout::time()
    {
        std::stringstream ss;
        struct timeval  tv;
        (void)gettimeofday(&tv, NULL);
        struct tm * ti;
        ti = localtime(&tv.tv_sec);
        ss << std::setfill('0');
        ss << std::setw(4) << ti->tm_year + 1900 << '/'
           << std::setw(2) << ti->tm_mon + 1 << '/'
           << std::setw(2) << ti->tm_mday << ' '
           << std::setw(2) << ti->tm_hour << ':'
           << std::setw(2) << ti->tm_min << ':'
           << std::setw(2) << ti->tm_sec << ':'
           << std::setw(3) << tv.tv_usec/1000;
        return ss.str();
    }

    std::string dbgout::bufToHEXstr(uint8_t buf[], uint32_t len)
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for(uint32_t i = 0; i < len; i++)
        {
            ss  << std::setw(2) << static_cast<int>(buf[i]) << ' ';
        }
        return ss.str();
    }

    dbgout dout;

    dbgout& endl(dbgout& so)
    {
        return so.flush(so, so.getDefaultMsgLevel());
    }

    dbgout& info(dbgout& so)
    {
        if(so.getCurrentMsgLevel() != DBG_INFO)
        {
            (void)so.flush(so, DBG_INFO);
        }
        return so;
    }

    dbgout& dbg(dbgout& so)
    {
        if(so.getCurrentMsgLevel() != DBG_DEBUG)
        {
            (void)so.flush(so, DBG_DEBUG);
        }
        return so;
    }

    dbgout& warn(dbgout& so)
    {
        if(so.getCurrentMsgLevel() != DBG_WARNING)
        {
            (void)so.flush(so, DBG_WARNING);
        }
        return so;
    }

    dbgout& err(dbgout& so)
    {
        if(so.getCurrentMsgLevel() != DBG_ERROR)
        {
            (void)so.flush(so, DBG_ERROR);
        }
        return so;
    }

}
