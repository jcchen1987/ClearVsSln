//#include<windows.h>
#include<iostream>
#include<afx.h>

using std::cout;
using std::endl;

#define MAX_EXT_NUM 100
int nExtName = 10;
char aachDelFileExt[MAX_EXT_NUM][20] = {"ncb", "exp", "ilk", "pdb", "user", "obj", "htm", "dep", "manifest", "idb"};

void RecursiveDirectory(CString strPath,int n);
void BrowserAllSimbleDirectory(CString strPath);
bool ProcessFile(CString filename);
void AddExt(char *pExt);

int main(int argc,char** argv)
{
    if(argc < 2)
    {
        return 1;
    }
    
    CString dir(argv[1]);

    if(argc > 2)
    {
        for(int i = 2; i < argc; i++)
        {
            AddExt(argv[i]);
        }        
    }
    
    dir += "/*.*";
    RecursiveDirectory(dir,0);

    return 0;
}


/*******************************
遍历目录及其子目录
********************************/
void RecursiveDirectory(CString strPath,int n)
{
    WIN32_FIND_DATA fd;

    HANDLE fHandle=FindFirstFile(strPath,&fd);
    int i;
    int index=strPath.Find("*",0);
    CString strOut=strPath.Left(index);
   
    if(fHandle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        if(fd.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY)
        {
            ProcessFile(strOut + fd.cFileName);
        }

        //继续搜索目录
        if(fd.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
        {
            int index=strPath.Find("*",0);
            CString directory=strPath.Left(index);
            directory+=CString(fd.cFileName);

            //确定在子目录搜索
            if(directory.Find(".")==-1 )
            {
                char* cdirectory=directory.GetBuffer(0);


                directory+=CString("/*.*");
                RecursiveDirectory(directory,n+1);
            }
        }

    }while(FindNextFile(fHandle,&fd));
}

bool ProcessFile(CString filename)
{
    int index=filename.ReverseFind('.');
    CString ext=filename.Right(filename.GetLength() - index);

    for(int i = 0; i < nExtName; i++)
    {
        CString str(aachDelFileExt[i]);
        if(ext.Find(str) != -1)
        {
            cout<<"Deleting\t"<<filename<<endl;
            DeleteFile(filename);
            return true;
        }        
    }   
    return false;
}

void AddExt(char *pExt)
{
    if(nExtName >= MAX_EXT_NUM)
    {
        return;
    }
    memcpy(aachDelFileExt[nExtName], pExt, 20);
    nExtName++;    
}