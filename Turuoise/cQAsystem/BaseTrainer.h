#ifndef _BASE_TRAINER_H_
#define _BASE_TRAINER_H_

#include <forward_list>
#include "StdRedef.h"
#include "KoreanMorphologicalAnalyzer.h"
#include "SqliteConnector.h"
#include "RapidXmlParser.h"

#ifdef _WIN32
#include "winDirent.h"
#else
#include <dirent.h>
#endif


class BaseTrainer : virtual public KoreanMorphologicalAnalyzer<String, Integer>
{
protected:
	String				mTrainingPath;
	SqliteConnector*	mSqlConnector;
	RapidXmlParser*		mXmlParser;

protected:
	void readDirectory(const char* srcDir, std::forward_list<String> *ngtPathList)
	{
		DIR *dir = opendir(srcDir);
		if(!dir)
		{
			std::cerr << "Can not open directory : [" << dir << "]" << std::endl;
			return;
		}

		// traverse files in directory
		dirent *entry = nullptr;
		while((entry = readdir(dir)) != NULL)
		{
			auto curFileName = entry->d_name;
			if(!strcmp(curFileName, ".") || !strcmp(curFileName, ".."))
				continue;

			char *path = (char*)malloc(strlen(srcDir)+strlen(curFileName)+2);
			sprintf(path, "%s/%s", srcDir, curFileName);

			ngtPathList->insert_after(ngtPathList->before_begin(), String(path));

			free(path);
		}
		closedir(dir);
	}

public:
	BaseTrainer() { };
	BaseTrainer(SqliteConnector* SqlConnector)
	{
		this->mSqlConnector = SqlConnector;
	}
	virtual ~BaseTrainer() { };

	virtual bool beginTraning(String& trainPath) = 0;
};


#endif