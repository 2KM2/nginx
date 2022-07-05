#pragma once
#include <vector>
using namespace std;


#include "common.h"
#include "ngx_global.h"
class CConfig
{
private:
    CConfig();
public:
    ~CConfig();
private:
	static CConfig *m_instance;
public:
    static CConfig *GetInstance()
    {
      	if(m_instance == NULL)
		{
			//锁
			if(m_instance == NULL)
			{					
				m_instance = new CConfig();
				static CGarhuishou cl; 
			}
			//放锁		
		}
		return m_instance;
    }
    class CGarhuishou  //类中套类，用于释放对象
	{
	public:				
		~CGarhuishou()
		{
			if (CConfig::m_instance)
			{						
				delete CConfig::m_instance;
				CConfig::m_instance = NULL;				
			}
		}
	};
public:
    bool Load(const char *pconfName); //装载配置文件
	const char *GetString(const char *p_itemname);
	int  GetIntDefault(const char *p_itemname,const int def);
public:
	std::vector<LPCConfItem> m_ConfigItemList; //存储配置信息的列表
};