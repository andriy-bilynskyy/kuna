#ifndef DBGOUT_H_
#define DBGOUT_H_

#include <ostream>
#include <stdint.h>
#include <string>

#define __DBG_LOG_S_(x) #x
#define __DBG_LOG_S__(x) __DBG_LOG_S_(x)
#define __S__LINE__ __DBG_LOG_S__(__LINE__)

#define __DBG_LOCATION__  "{" + (std::string(__FUNCTION__) + " " + std::string(__FILE__)\
                           + ":L" + std::string(__S__LINE__)) + "}"

namespace dbg
{
    typedef enum dbgLevel
    {
        DBG_INFO,
        DBG_DEBUG,
        DBG_WARNING,
        DBG_ERROR,
        DBG_NOTSET
    }dbgLevel_t;

    class dbgout
    {
    public:
        dbgout();
        ~dbgout();
        void redirect(std::ostream &os);
        template<class T>
        friend dbgout& operator <<(dbgout& so, T t)
        {
            if(so.m_curMsgLevel >= so.m_dbgLevel)
            {
                if(so.m_begin)
                {
                    so.m_begin = false;
                    *so.m_pos << time();
                    switch(so.m_curMsgLevel)
                    {
                    case DBG_INFO:
                        *so.m_pos << " [INFO_] ";
                        break;
                    case DBG_DEBUG:
                        *so.m_pos << " [DEBUG] ";
                        break;
                    case DBG_WARNING:
                        *so.m_pos << " [WARN_] ";
                        break;
                    case DBG_ERROR:
                        *so.m_pos << " [ERROR] ";
                        break;
                    default:
                        *so.m_pos << " [???__] ";
                        break;
                    }
                }
                *so.m_pos << t;
            }
            return so;
        }
        static dbgout& flush(dbgout& so, dbgLevel_t msglev);
        dbgout & operator<< (dbgout & (*_f)(dbgout &));
        void setDbgLevel(dbgLevel_t level);
        void setDefaultMsgLevel(dbgLevel_t level);
        dbgLevel_t getDefaultMsgLevel() const;
        dbgLevel_t getCurrentMsgLevel() const;
        static std::string bufToHEXstr(uint8_t buf[], uint32_t len);

    private:
        std::ostream                   * m_pos;
        dbgLevel_t                       m_defMsgLevel;
        bool                             m_begin;
        dbgLevel_t                       m_curMsgLevel;
        dbgLevel_t                       m_dbgLevel;

        static std::string time();
        dbgout & operator=(const dbgout &);
        dbgout(const dbgout &);
    };

    extern dbgout dout;

    dbgout& endl(dbgout& so);
    dbgout& info(dbgout& so);
    dbgout& dbg(dbgout& so);
    dbgout& warn(dbgout& so);
    dbgout& err(dbgout& so);

}

#endif
