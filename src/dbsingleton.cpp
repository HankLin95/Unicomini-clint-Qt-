#include "dbsingleton.h"
dbsingle* dbsingle::instance=NULL;
del_single dbsingle::del;
dbsingle::dbsingle()
{
	printf("�������ݿⵥ����\n");
}
dbsingle* dbsingle::getinstace(char*filename)
{
	if(dbsingle::instance==NULL)
	{
		dbsingle::instance=new dbsingle;
		if(sqlite3_open(filename,&instance->Pdb)==SQLITE_OK)
		{
			printf("�ɹ������ݿ�%s\n",filename);
		}
		else
		{
			printf("�����ݿ����\n");
		    printf("�������%d,������Ϣ%s",sqlite3_errcode(instance->Pdb),sqlite3_errmsg(instance->Pdb));
		}
		return instance;
	}
	else
	{
		return dbsingle::instance;
	}
}
dbsingle* dbsingle::getinstace()
{
	if(dbsingle::instance==NULL)
	{
		dbsingle::instance=new dbsingle;
		return instance;
	}
	else
	{
		return dbsingle::instance;
	}
}
void dbsingle::db_close()
{
	if(instance->Pdb==NULL)
	{
		printf("��δ�����ݿ�\n");
		return;
	}
	else
	{
		sqlite3_close(instance->Pdb);
		printf("�ر����ݿ�\n");
	}
}


void dbsingle::gettable(char*sql,char ***result,int&col,int&row)
{

    if(result==NULL)
        result =new char**;
    if(instance->Pdb==NULL)
    {
        printf("��δ�����ݿ�,������ݿ���ѯ\n");
        return ;
    }
    int ret;
    char *errmsg;
    ret=sqlite3_get_table(instance->Pdb,sql,result,&row,&col,&errmsg);
    if(ret!=SQLITE_OK)
    {
        printf("huoqu���ݿ����\n");
        printf("�������%d,������Ϣ%s",sqlite3_errcode(instance->Pdb),sqlite3_errmsg(instance->Pdb));
    }
    return ;
}

int dbsingle::gettable_return(char*sql,char ***result,int&col,int&row)
{

    if(result==NULL)
        result =new char**;
    if(instance->Pdb==NULL)
    {
        printf("��δ�����ݿ�,������ݿ���ѯ\n");
        return 0;
    }
    int ret;
    char *errmsg;
    ret=sqlite3_get_table(instance->Pdb,sql,result,&row,&col,&errmsg);
    if(ret!=SQLITE_OK)
    {
        printf("huoqu���ݿ����\n");
        printf("�������%d,������Ϣ%s",sqlite3_errcode(instance->Pdb),sqlite3_errmsg(instance->Pdb));
        return 0;
    }
    return 1;
}




dbsingle::~dbsingle ()
{
	printf("���ݿⵥ����������\n");
}
del_single::~del_single()
{
	delete dbsingle::instance;
}
