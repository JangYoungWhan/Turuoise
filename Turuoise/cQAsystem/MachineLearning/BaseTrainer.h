#ifndef _BASE_TRAINER_H_
#define _BASE_TRAINER_H_

#include <forward_list>
#include "../Utils/StdRedef.h"
#include "../Utils/KoreanMorphologicalAnalyzer.h"
#include "../Utils/SqliteConnector.h"
#include "../Utils/RapidXmlParser.h"
#include "../Utils/ProgressBar.h"

#ifdef _WIN32
#include "../Utils/winDirent.h"
#else
#include <dirent.h>
#endif


class BaseTrainer : virtual public KoreanMorphologicalAnalyzer<String, Integer>
{
protected:
	String					mTrainingPath;
	SqliteConnector*		mSqlConnector;
	RapidXmlParser*			mXmlParser;
	ProgressBar<Integer>*	mProgressBar;

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
		this->mProgressBar	= new ProgressBar<Integer>();
		this->mSqlConnector = SqlConnector;
	}
	virtual ~BaseTrainer()
	{
		delete mProgressBar;
	}

	virtual bool beginTraning(String& trainPath, std::map<Integer, String>& docID2Path) = 0;
};


#endif