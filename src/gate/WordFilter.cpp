#include "WordFilter.h"

#include "Fir.h"
#include "zCSVParser.h"

WordFilter::WordFilter()
{
}

bool WordFilter::init()
{
	forbidWords.clear();

	zCSVParser csvParser;
	std::string csvPath = Fir::global["configdir"] + "csv/chat_illegal_word.csv";

	bool result = csvParser.Load(csvPath);
	if (!result)
	{
		Fir::logger->error("Words forbidden load error! filePath='%s' err='%s'", csvPath.c_str(), csvParser.GetErrorInfo().c_str());
		return false;
	}

	std::string errFromat = "Words forbidden parse field '%s' error! err='%s'";
	while (csvParser.MoveToNextLine())
	{
		std::string forbidWord;
		result = csvParser.GetFieldValue("illegal_word", forbidWord);
		if (!result)
		{
			Fir::logger->error(errFromat.c_str(), "illegal_word", csvParser.GetErrorInfo().c_str());
			return false;
		}

		forbidWords.insert(forbidWord);
	}
	Fir::logger->debug("Words forbidden load success. total count: %d. ", (int)forbidWords.size());

	return true;
}

bool WordFilter::doFilter(const std::string text) const
{
	std::string::size_type pos = 0;

	for (WordsSet::const_iterator it = forbidWords.begin(); it != forbidWords.end(); ++it)
	{
		pos = text.find(*it, 0);
		if (pos != std::string::npos)
		{
			return false;
		}
	}

	return true;
}