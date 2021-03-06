#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <io.h>
using namespace std;
#include <direct.h>

#include "tinyxml/tinyxml.h"
#include "Str.h"
#include "string.h"
#define BUF_SIZE    256

string getRelativedir(const char *pDir){
	int i;
	string rdir = pDir;
	int iLen = strlen(pDir);
	for (i = 0; i < iLen; i++){
		if (pDir[i] == '.' || pDir[i] == '\\' || pDir[i] == '/')
		{
			i++;
		}
		else
		{
			rdir[i] = 0;
			break;
		}

	}
	return rdir;
}

//移除相对路径
const char * rmredir(const char *pDir){
	int i;
	const char* pszDir = NULL;
	int iLen = strlen(pDir);
	for (i = 0; i < iLen; i++){
		if (pDir[i] == '.' || pDir[i] == '\\' || pDir[i] == '/')
		{
			i++;
		}
		else
		{
			pszDir = &pDir[i];
			break;
		}

	}
	return pszDir;
}

void setdir(char* location)
{
	char tmp;
	char* p = location + strlen(location) - 1;
	while (*p != '\\' && p >= location) p--;
	if (p >= location){
		tmp = *p;
		*p = '\0';
		SetCurrentDirectory(location);
		*p = tmp;
	}
}
// "E:\\VS2013\\keil2vs\\output\\Debug"
void getdir(char* curdir){
	//char szFullPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curdir);
}
//获取当前运行文件路径 "E:\\VS2013\\keil2vs\\.\\output\\Debug\\"
string GetFilePath(char * filepath)
{
	//char szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, filepath, MAX_PATH);	//通过文件获取路径
	(strrchr(filepath, '\\'))[1] = 0; // 删除文件名，只获得路径字串  
	string path = filepath;
	cout << "执行文件当前路径: "<< path << endl;
	return path;

}
//循环创建目录
//\\c:\\windows\\user\\..\\.\\list/list.c
//..\\.\\list/list.c
int CreateDir(const   char   *sPathName)
{
	char   DirName[256];
	strcpy(DirName, sPathName);
	int   i;
	int len = strlen(DirName);

	if (DirName[len - 1] != '/')
		strcat(DirName, "/");

	len = strlen(DirName);

	for (i = 1; i < len; i++)
	{
		//if (DirName[i] == ':')i += 2;
		if (DirName[i] == '/' | DirName[i] == '\\')
		{
			DirName[i] = 0;
			if (access(DirName, NULL) != 0)	//不存在该目录
			{
				if (_mkdir(DirName) != 0)	//创建失败
				{
					perror("mkdir   error");
					return   -1;
				}
			}
			DirName[i] = '\\';
		}
	}
	return   0;
}
//文件复制
int copyfile(const char* srcfile,const char* dstfile)
{
	FILE *fp1, *fp2;
	char uvprojx_file1[256], uvprojx_file2[256];
	char buf[1024];
	int total,readsize, writesize;
	char dstdir[256];

			fp1 = fopen(srcfile, "rb");
			if (fp1 == NULL)
			{
				printf("open %s failed /n", uvprojx_file1);
				return -1;
			}
			//创建目录
			strcpy(dstdir,dstfile);
			//strrchr(dstdir, '.')[0] = 0;//移除扩展名
			strrchr(dstdir, '\\')[0] = 0;//移除文件名
			if(CreateDir(dstdir)==-1)//创建失败
				return -1;

			fp2 = fopen(dstfile, "wb+");
			if (fp2 == NULL)
			{
				printf("open %s failed /n", uvprojx_file2);
				fclose(fp1);
				return -1;
			}
			total = 0;
			while ((readsize = fread(buf, 1,sizeof(buf),  fp1))>0)
			{

				writesize = fwrite(buf, 1, readsize, fp2);
				if (writesize !=  readsize)
				{
					printf("write error");
					fclose(fp1);
					fclose(fp2);
					return -2;
				}				
				total += readsize; 
				//memset(buf, 0, sizeof(buf));
			}
			fclose(fp1);
			fclose(fp2);
			//rmdir(uvprojx_file2);
	

}

//创建目录
int makeDir(const char *pDir)
{
	int i = 0;
	int iRet;
	int iLen;
	char* pszDir;

	if (NULL == pDir)
	{
		return 0;
	}
	iLen = strlen(pDir);
	//pszDir = strdup(pDir);
	//移除 
	pszDir = (char *)rmredir(pDir);
	//


	// 创建中间目录  
	for (i = 0; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';

			//如果不存在,创建  
			iRet = _access(pszDir, 0);
			if (iRet != 0)
			{
				iRet = _mkdir(pszDir);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//支持linux,将所有\换成/  
			pszDir[i] = '/';
		}
	}

	iRet = _mkdir(pszDir);
	//free(pszDir);
	return iRet;
}
char filecpy(const char* src, char *dst){
	char * rsrc0;
	char * rsrc;
	const char * rdst;
	FILE *in_file, *out_file;
	char data[BUF_SIZE];
	size_t bytes_in, bytes_out;
	long len = 0;

	if ((in_file = fopen(src, "rb")) == NULL)
	{

		return 2;
	}

	strcpy(data, src);
	strrchr(data, '.')[0] = 0;//移除扩展名

	strrchr(data, '\\')[0] = 0;//移除文件名

	//	CreatDir(data);
	if (_access("src", 0) == -1){
		printf("file not exist");
		_mkdir("src");
		_mkdir("src\\src0");
		_mkdir("src\\src0\\src1");
	}

	//移除相对路径
	rdst = rmredir(src);


	if ((out_file = fopen(rdst, "wb+")) == NULL)
	{

		return 3;
	}


	while ((bytes_in = fread(data, 1, BUF_SIZE, in_file)) > 0)
	{
		bytes_out = fwrite(data, 1, bytes_in, out_file);
		if (bytes_in != bytes_out)
		{
			perror("Fatal write error.\n");
			return 4;
		}
		len += bytes_out;
		printf("copying file .... %d bytes copy\n", len);
	}

	fclose(in_file);
	fclose(out_file);
}

bool make_dsw_file(const char* project_name)
{
	const char* dsw_content =
		"Microsoft Developer Studio Workspace File, Format Version 6.00\r\n"
		"# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!\r\n"
		"\r\n"
		"###############################################################################\r\n"
		"\r\n"
		"Project: \"%s\"=\".\\%s.dsp\" - Package Owner=<4>\r\n"
		"\r\n"
		"Package=<5>\r\n"
		"{{{\r\n"
		"}}}\r\n"
		"\r\n"
		"Package=<4>\r\n"
		"{{{\r\n"
		"}}}\r\n"
		"\r\n"
		"###############################################################################\r\n"
		"\r\n"
		"Global:\r\n"
		"\r\n"
		"Package=<5>\r\n"
		"{{{\r\n"
		"}}}\r\n"
		"\r\n"
		"Package=<3>\r\n"
		"{{{\r\n"
		"}}}\r\n"
		"\r\n"
		"###############################################################################\r\n"
		"\r\n"
		"";

	char dsw[1024] = { 0 };
	string name(project_name);
	name += ".dsw";
	FILE* fp_dsw = fopen(name.c_str(), "wb");
	if (fp_dsw == NULL){
		cout << "错误:无法打开 " << name << " 用于写!" << endl;
		return false;
	}

	int len = _snprintf(dsw, sizeof(dsw), dsw_content, project_name, project_name);
	if (fwrite(dsw, 1, len, fp_dsw) == len){
		cout << "已创建 " << name << " 工作空间文件!" << endl;
		fclose(fp_dsw);
		return true;
	}
	else{
		cout << "错误:文件写入错误!" << endl;
		fclose(fp_dsw);
		remove(name.c_str());
		return false;
	}
}

bool make_dsp_file(const char* project_name, vector<string>& groups, string& define, string& includepath)
{
	const char* dsp_content =
		"# Microsoft Developer Studio Project File - Name=\"%s\" - Package Owner=<4>\r\n"
		"# Microsoft Developer Studio Generated Build File, Format Version 6.00\r\n"
		"# ** DO NOT EDIT **\r\n"
		"\r\n"
		"# TARGTYPE \"Win32 (x86) Console Application\" 0x0103\r\n"
		"\r\n"
		"CFG=%s - Win32 Debug\r\n"
		"!MESSAGE This is not a valid makefile. To build this project using NMAKE,\r\n"
		"!MESSAGE use the Export Makefile command and run\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE NMAKE /f \"%s.mak\".\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE You can specify a configuration when running NMAKE\r\n"
		"!MESSAGE by defining the macro CFG on the command line. For example:\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE NMAKE /f \"%s.mak\" CFG=\"%s - Win32 Debug\"\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE Possible choices for configuration are:\r\n"
		"!MESSAGE \r\n"
		"!MESSAGE \"%s - Win32 Release\" (based on \"Win32 (x86) Console Application\")\r\n"
		"!MESSAGE \"%s - Win32 Debug\" (based on \"Win32 (x86) Console Application\")\r\n"
		"!MESSAGE \r\n" /*在这之前有7个项目名*/
		"\r\n"
		"# Begin Project\r\n"
		"# PROP AllowPerConfigDependencies 0\r\n"
		"# PROP Scc_ProjName \"\"\r\n"
		"# PROP Scc_LocalPath \"\"\r\n"
		"CPP=cl.exe\r\n"
		"RSC=rc.exe\r\n"
		"\r\n"
		"!IF  \"$(CFG)\" == \"%s - Win32 Release\"\r\n" //项目名
		"\r\n"
		"# PROP BASE Use_MFC 0\r\n"
		"# PROP BASE Use_Debug_Libraries 0\r\n"
		"# PROP BASE Output_Dir \"Release\"\r\n"
		"# PROP BASE Intermediate_Dir \"Release\"\r\n"
		"# PROP BASE Target_Dir \"\"\r\n"
		"# PROP Use_MFC 0\r\n"
		"# PROP Use_Debug_Libraries 0\r\n"
		"# PROP Output_Dir \"Release\"\r\n"
		"# PROP Intermediate_Dir \"Release\"\r\n"
		"# PROP Target_Dir \"\"\r\n"
		"# ADD BASE CPP /nologo /W3 /GX /O2 %s /YX /FD /c\r\n" //宏定义:/D \"WIN32\" /D \"NDEBUG\"
		"# ADD CPP /nologo /W3 /GX /O2 %s %s /YX /FD /c\r\n" //目录+宏定义
		"# ADD BASE RSC /l 0x804 /d \"NDEBUG\"\r\n"
		"# ADD RSC /l 0x804 /d \"NDEBUG\"\r\n"
		"BSC32=bscmake.exe\r\n"
		"# ADD BASE BSC32 /nologo\r\n"
		"# ADD BSC32 /nologo\r\n"
		"LINK32=link.exe\r\n"
		"# ADD BASE LINK32 user32.lib /nologo /subsystem:console /machine:I386\r\n"
		"# ADD LINK32 user32.lib /nologo /subsystem:console /machine:I386\r\n"
		"\r\n"
		"!ELSEIF  \"$(CFG)\" == \"%s - Win32 Debug\"\r\n"     //项目名
		"\r\n"
		"# PROP BASE Use_MFC 0\r\n"
		"# PROP BASE Use_Debug_Libraries 1\r\n"
		"# PROP BASE Output_Dir \"Debug\"\r\n"
		"# PROP BASE Intermediate_Dir \"Debug\"\r\n"
		"# PROP BASE Target_Dir \"\"\r\n"
		"# PROP Use_MFC 0\r\n"
		"# PROP Use_Debug_Libraries 1\r\n"
		"# PROP Output_Dir \"Debug\"\r\n"
		"# PROP Intermediate_Dir \"Debug\"\r\n"
		"# PROP Target_Dir \"\"\r\n"
		"# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od %s /YX /FD /GZ  /c\r\n" //宏定义
		"# ADD CPP /nologo /W3 /Gm /GX /ZI /Od %s %s /YX /FD /GZ  /c\r\n" //目录+宏定义
		"# ADD BASE RSC /l 0x804 /d \"_DEBUG\"\r\n"
		"# ADD RSC /l 0x804 /d \"_DEBUG\"\r\n"
		"BSC32=bscmake.exe\r\n"
		"# ADD BASE BSC32 /nologo\r\n"
		"# ADD BSC32 /nologo\r\n"
		"LINK32=link.exe\r\n"
		"# ADD BASE LINK32 user32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\r\n"
		"# ADD LINK32 user32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept\r\n"
		"\r\n"
		"!ENDIF \r\n" /*前面只有2个项目名*/
		"\r\n"
		"# Begin Target\r\n"
		"\r\n"
		"# Name \"%s - Win32 Release\"\r\n"    //这里有两个
		"# Name \"%s - Win32 Debug\"\r\n"
		;

	char dsp[50 * 1024] = { 0 };
	string name(project_name);
	name += ".dsp";
	FILE* fp_dsp = fopen(name.c_str(), "wb");
	if (fp_dsp == NULL){
		cout << "错误:无法打开 " << name << " 用于写!" << endl;
		return false;
	}

	// 定义是逗号分隔的
	char* zDefine = new char[define.length() + 2];
	memset(zDefine, 0, define.length() + 2);
	strcpy(zDefine, define.c_str());
	for (int iii = 0; iii < define.length() + 2; iii++){
		if (zDefine[iii] == ','){
			zDefine[iii] = '\0';
		}
	}
	string strDefine("");
	for (const char* pp = zDefine; *pp;){
		strDefine += "/D \"";
		strDefine += pp;
		strDefine += "\" ";

		while (*pp) pp++;
		pp++;
	}

	// includepath是分号分隔的
	char* zInclude = new char[includepath.length() + 2];
	memset(zInclude, 0, includepath.length() + 2);
	strcpy(zInclude, includepath.c_str());
	for (int jjj = 0; jjj < includepath.length() + 2; jjj++){
		if (zInclude[jjj] == ';'){
			zInclude[jjj] = '\0';
		}
	}

	string strInclude("");
	for (const char* qq = zInclude; *qq;){
		strInclude += "/I \"";
		strInclude += qq;
		strInclude += "\" ";

		while (*qq) qq++;
		qq++;
	}

	int len = 0;
	len += _snprintf(dsp, sizeof(dsp)-len, dsp_content,
		project_name, project_name, project_name, project_name, project_name, project_name, project_name,

		project_name,
		strDefine.c_str(),
		strInclude.c_str(), strDefine.c_str(),

		project_name,
		strDefine.c_str(),
		strInclude.c_str(), strDefine.c_str(),

		project_name,
		project_name
		);

	delete[] zDefine;
	delete[] zInclude;

	for (vector<string>::iterator it = groups.begin(); it != groups.end(); it++){
		len += _snprintf(dsp + len, sizeof(dsp)-len, "%s", it->c_str());
	}

	len += _snprintf(dsp + len, sizeof(dsp)-len, "%s", "# End Target\r\n# End Project\r\n");

	if (fwrite(dsp, 1, len, fp_dsp) == len){
		cout << "已创建 " << name << " 项目文件!" << endl;
		fclose(fp_dsp);
	}
	else{
		cout << "错误:文件写入错误!" << endl;
		fclose(fp_dsp);
		remove(name.c_str());
		return false;
	}
	return true;
}

string get_vs_proj(const char* uvprojfullfilename){
	char srcf[256];
	bool bXmlUtf8 = true;
	TiXmlDocument doc(uvprojfullfilename);
	if (!doc.LoadFile()){
		cout << "错误:TiXmlDocument.LoadFile()" << endl;
		return false;
	}
	bXmlUtf8 = stricmp(doc.FirstChild()->ToDeclaration()->Encoding(), "UTF-8") == 0;

	TiXmlNode* nodeTarget = doc.FirstChild("Project")->FirstChild("Targets")->FirstChild("Target");
	TiXmlNode* nodeGroups = nodeTarget->FirstChild("Groups");
	//Goups包含源代码分组信息
	TiXmlNode* group = nodeGroups->FirstChild("Group");
	//取得分组名称
	//TiXmlElement* eleGroupName = group->FirstChild()->ToElement();
	TiXmlNode* file = group->FirstChild("Files")->FirstChild("File");

	//文件路径
	TiXmlElement* eleFilePath = file->FirstChild("FilePath")->ToElement();
	//cout << eleFilePath->GetText() << endl;

	//工程文件路径
	getdir(srcf);
	strcat(srcf, "\\");
	const char* src = eleFilePath->GetText();
	string rdir = getRelativedir(src);//获取相对路径中的..\\..\\				
	strcat(srcf, rdir.c_str());
	//setdir(srcf);

	//getdir(dstf);
	strcat(srcf, "\\Visual Studio 2013 Project\\");
	//const char *jstr = rmredir(src);//获取相对路径中的文件名 source\\main.c
	//strcat(srcf, jstr);//得到目标绝对路径
	string VSpath = srcf;
	return VSpath;
}

bool get_uv_info(const char* uvprojfullfilename, vector<string>& groups, string& define, string& includepath)
{
	char srcf[256];
	char dstf[256];
	const char* ret_str = NULL;
	bool bXmlUtf8 = true;
	const char* src;
	TiXmlDocument doc(uvprojfullfilename);
	if (!doc.LoadFile()){
		cout << "错误:TiXmlDocument.LoadFile()" << endl;
		return false;
	}
	//doc.Print();
	//TiXmlElement* rootElement = doc.RootElement();
	//TiXmlElement* classElement = rootElement->FirstChildElement();  // Class元素
	//TiXmlElement* studentElement = classElement->FirstChildElement();  //Students 
	bXmlUtf8 = stricmp(doc.FirstChild()->ToDeclaration()->Encoding(), "UTF-8") == 0;


	TiXmlNode* nodeTarget = doc.FirstChild("Project")->FirstChild("Targets")->FirstChild("Target");
	TiXmlNode* nodeVariousControls = nodeTarget->FirstChild("TargetOption")->FirstChild("TargetArmAds")->FirstChild("Cads")->FirstChild("VariousControls");
	//取得宏定义 Define
	TiXmlElement* cDefine = nodeVariousControls->FirstChild("Define")->ToElement();
	ret_str = cDefine->GetText();
	if (ret_str){
		if (bXmlUtf8){
			define = AStr(ret_str, true).toAnsi();
		}
		else{
			define = ret_str;
		}
		cout << cDefine->GetText() << endl;
	}
	//取得包含路径 IncludePath
	TiXmlElement* cIncludePath = nodeVariousControls->FirstChild("IncludePath")->ToElement();
	ret_str = cIncludePath->GetText();
	if (ret_str){
		if (bXmlUtf8){
			includepath = AStr(ret_str, true).toAnsi();
		}
		else{
			includepath = ret_str;
		}
		//一定注意 Cout 不为NULL时才能 输出 否则报异常
		cout << cIncludePath->GetText() << endl;
	}


	

	TiXmlNode* nodeGroups = nodeTarget->FirstChild("Groups");
	//Goups包含源代码分组信息,遍历
	for (TiXmlNode* group = nodeGroups->FirstChild(); group != NULL; group = group->NextSibling()){
		stringstream strGroup("");
		//取得分组名称
		TiXmlElement* eleGroupName = group->FirstChild()->ToElement();

		strGroup << "# Begin Group \"" << AStr(eleGroupName->GetText(), bXmlUtf8).toAnsi() << "\"\r\n\r\n";
		strGroup << "# PROP Default_Filter \"\"\r\n";
		//cout<<eleGroupName->GetText()<<endl;

		//Files包含所有的文件列表
		TiXmlNode* nodeFiles = eleGroupName->NextSibling();
		if (nodeFiles){
			for (TiXmlNode* file = nodeFiles->FirstChild(); file != NULL; file = file->NextSibling()){
				//文件名
				TiXmlElement* eleuvprojx_file = file->FirstChild()->ToElement();
				//文件路径
				TiXmlElement* eleFilePath = file->FirstChild()->NextSibling()->NextSibling()->ToElement();
				//cout << eleuvprojx_file->GetText() << "," << eleFilePath->GetText() << endl;


				//工程文件路径
				getdir(srcf);
				strcat(srcf, "\\");
				src = eleFilePath->GetText();
				string rdir =getRelativedir(src);//获取相对路径中的..\\..\\
				strcat(srcf, rdir.c_str());
				//setdir(srcf);

				//getdir(dstf);
				strcat(srcf, "\\Visual Studio 2013 Project\\");
				const char *jstr = rmredir(src);//获取相对路径中的文件名 source\\main.c
				strcat(srcf, jstr);//得到目标绝对路径

				copyfile(src, srcf);

				strGroup << "# Begin Source File\r\n\r\nSOURCE=\"" << AStr(eleFilePath->GetText(), bXmlUtf8).toAnsi() << "\"\r\n" << "# End Source File\r\n";
			}
		}
		strGroup << "# End Group\r\n";
		groups.push_back(string(strGroup.str()));
	}
	return true;
}
//DEQUOTE
//去除字符串中的配对引号，且该字符串是以该引号开始的，并且去掉与之配对的后引号之后的全部内容。
//啰嗦了，举个例子：y = dequote(x);
//
//"我是'我'的"我""    我是'我'的
//"你"是你的'你'      你
//他是他的"他"        他是他的"他"
//她们还是她们        她们还是她们
//'它是'它'的它'      它是
void dequote(char* str)
{
	char* di = str;
	char* si = str;
	if (*si != ' ' && *si != '\t' && *si != '\"') return;

	while (*si && *si != '\"'){
		si++;
	}

	if (!*si) return;
	else si++;

	while (*si && *si != '\"'){
		*di++ = *si++;
	}
	*di = '\0';
}

void dereturn(char* str)
{
	char* p = str + strlen(str) - 1;
	if (*p == '\n') *p = '\0';
}


char *strstr1(const char*s1, const char*s2)
{
	int n;
	if (*s2)
	{
		while (*s1)
		{
			for (n = 0; *(s1 + n) == *(s2 + n); n++)
			{
				if (!*(s2 + n + 1))
					return(char*)s1;
			}
			s1++;
		}
		return NULL;
	}
	else
		return (char*)s1;
}
bool is_file_present(const char* fn)
{
	DWORD dwAttributes = GetFileAttributes(fn);
	return dwAttributes != INVALID_FILE_ATTRIBUTES &&
		!(dwAttributes&FILE_ATTRIBUTE_DIRECTORY);
}

void about(void)
{

}

void make_full_path(char* s, int nLen, const char *file_name, const char*file_ext)
{
	char szPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	char cDir[100] = "";
	char cDrive[10] = "";
	char cf[20] = "";
	char cExt[10] = "";
	_splitpath_s(szPath, cDrive, cDir, cf, cExt);
	_makepath_s(s, nLen, cDrive, cDir, file_name, file_ext);
}
int main(int argc, char** argv)
{
	FILE *in_file, *out_file;
	vector<string> groups;
	string define;
	string includepath;
	WIN32_FIND_DATA FindFileData;
	char project[128] = { 0 };
	char path[MAX_PATH] = { 0 };
	char location[MAX_PATH] = { 0 };
	char compiler[260] = { 0 };
	about();
	//char tmp;
	//sort(groups.begin(), groups.end());
		//获取运行目录 szFullPath
		char szFullPath[MAX_PATH];
		//getdir(szFullPath);
		//cout << szFullPath << endl;
		GetFilePath(szFullPath);
		setdir(szFullPath);	//如果设置的目录一定要确保\\结尾 否则当作文件处理***
		getdir(szFullPath);
		cout << "优化后的当前路径: " << szFullPath << endl;
	

	//strcat(location, "\\..\\usr\\\\elp/23d");
	//	strcat(location, uvprojx_file);
	//_mkdir("..");
	//CreateDir(location);
	//strrchr(location, '.\\');

	char uvprojx_file[MAX_PATH] = { 0 };
	char uv_full_name[MAX_PATH] = { 0 };
	//获取uvprojx文件 转换为绝对路径
	for (;;){

		printf("输入uvprojx工程文件名:");
		//获取工程文件 
		fgets(uvprojx_file, sizeof(uvprojx_file), stdin);
		if (*uvprojx_file == '\n'){
			printf("错误:项目名称不合法!\n");
			continue;
		}
		else{
			dequote(uvprojx_file);
			dereturn(uvprojx_file);
			
			// 判断 
			if (NULL == strstr((const char *)uvprojx_file, ":\\"))	{
				//相对路径->绝对路径
				strcat(szFullPath, "\\");
				strcat(szFullPath, uvprojx_file);
				strcpy(uvprojx_file, szFullPath);
			}
			if (GetFileAttributes(uvprojx_file) != -1)
			{
				printf("找到uv工程文件: %s \n", uvprojx_file);

				break;
			}
			printf("未找到uv工程文件 : %s \n", uvprojx_file);
		}
	}
	setdir(uvprojx_file);
	getdir(szFullPath);
	cout << "重新获取工程文件路径: " << szFullPath << endl;
	
	//取得工程名
	char *uvfilename;
	char project_name[128] = { 0 };
	uvfilename =strrchr(uvprojx_file, '\\');
	uvfilename++;
	strcpy(project_name, uvfilename);
	strrchr(project_name, '.')[0] = 0; 
	cout << "工程名称: " << project_name << endl;
	
	//获取VS2013工程创建路径
	string vsproj = get_vs_proj(uvprojx_file);
	vsproj += project_name;
	//vsproj.at(1) = 0;
	//const char *str11 = vsproj.c_str();
	//vsproj += str11;
	//string::iterator it = vsproj.begin();
	//string::iterator it2 = vsproj.end();
	//int ss0 = vsproj.size();
	try{
		if (make_dsw_file(vsproj.c_str())){
			if (get_uv_info(uvprojx_file, groups, define, includepath)){
				includepath += ";";
				//includepath += compiler;
				if (make_dsp_file(vsproj.c_str(), groups, define, includepath)){

				}
			}
		}
	}
	catch (...){

	}
	system("pause");

	return 0;
}
