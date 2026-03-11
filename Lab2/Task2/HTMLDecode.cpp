#include "HTMLDecode.h"
#include <iostream>
#include <string>

std::string GetString(std::istream& input)
{
	std::string val;
	std::string str;
	while (input >> val)
	{
		str += val;
	}

	return str;
}

std::string HTMLDecode(std::string const& str)
{
	std::string resultStr = str;

	std::string alphabet[5][2] =
	{
		{"&quot;", "\""},
		{"&apos;", "'"},
		{"&lt;", "<"},
		{"&gt;", ">"},
		{"&amp;", "&"}
	};

	for (auto& tmp : alphabet)
	{
		std::string oldStr = tmp[0];
		std::string newStr = tmp[1];
		size_t start = resultStr.find(oldStr);
		while (start != std::string::npos)
		{
			resultStr.replace(start, oldStr.length(), newStr);
			start = resultStr.find(oldStr, start + newStr.length());
		}
	}
	return resultStr;
}