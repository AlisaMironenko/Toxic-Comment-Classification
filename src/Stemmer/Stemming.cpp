#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "Stemmer.h"

//�������� ������� ��������, ����������� not � ���� ����� � �������� ���������
//�� ���� - ��, ��� ����� ���������
//�� ������ - ������ �������� (�������) ��� ��� ����� ��������� � ��������� �����
std::vector<std::vector<tcc::word_t>> tcc::PorterStemming::stem(std::vector<json> text) {

	std::vector<std::vector<tcc::word_t>> res(text.size(), std::vector<tcc::word_t>());
	std::string buf_comment;
	tcc::word_t buf_word;
	std::string tag = "comment_text";
	std::string neg = "not";
	bool for_not = false;

	size_t i = 0;
	for (json el : text) {

		buf_comment = el[tag];

		std::istringstream iss(buf_comment, std::istringstream::in);
		while (iss >> buf_word.str) {
			if (buf_word.str == "not") {
				for_not = true;
				continue;
			}				

			if (for_not)
				buf_word.str = neg + buf_word.str;
			res[i].push_back(buf_word);
			for_not = false;
		}

		i++;

	}
	for (std::vector<std::vector<tcc::word_t> >::iterator it = res.begin(); it != res.end(); ++it) {
		for (std::vector<tcc::word_t>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
			trim(*it2);
			stem_word((*it2).str);
		}
	}

	return res;
}

void tcc::PorterStemming::stem_word(std::string& word) {
	// special case short words or sentence tags
	if (word.size() <= 2 || word == "<s>" || word == "</s>")
		return;

	// max word length is 35 for English
	if (word.size() > 35)
		word = word.substr(0, 35);

	if (word[0] == '\'')
		word = word.substr(1, word.size() - 1);

	if (special(word))
		return;

	changeY(word);
	size_t startR1 = getStartR1(word);
	size_t startR2 = getStartR2(word, startR1);

	step0(word);

	if (step1A(word))
	{
		std::replace(word.begin(), word.end(), 'Y', 'y');
		return;
	}

	step1B(word, startR1);
	step1C(word);
	step2(word, startR1);
	step3(word, startR1, startR2);
	step4(word, startR2);
	step5(word, startR1, startR2);

	std::replace(word.begin(), word.end(), 'Y', 'y');
	return;
}

void tcc::PorterStemming::trim(tcc::word_t& word)
{
	if (word.str == "<s>" || word.str == "</s>")
		return;

	if (word.str.size() == std::count_if(word.str.begin(), word.str.end(), [](const char& a) { return ::isupper(a); }))
		word.info = UPPERCASE;
	else
		word.info = LOWERCASE;

	std::transform(word.str.begin(), word.str.end(), word.str.begin(), ::tolower);
	for (std::string::iterator i = word.str.begin(); i != word.str.end();) {
		if (!isalpha(*i)) {
			word.str.erase(word.str.find(*i), 1);
			continue;
		}
		i++;
		if (i == word.str.end())
			break;
	}
}

size_t tcc::PorterStemming::getStartR1(const std::string& word)
{
	// special cases
	if (word.size() >= 5 && word[0] == 'g' && word[1] == 'e' && word[2] == 'n'
		&& word[3] == 'e' && word[4] == 'r')
		return 5;
	if (word.size() >= 6 && word[0] == 'c' && word[1] == 'o' && word[2] == 'm'
		&& word[3] == 'm' && word[4] == 'u' && word[5] == 'n')
		return 6;
	if (word.size() >= 5 && word[0] == 'a' && word[1] == 'r' && word[2] == 's'
		&& word[3] == 'e' && word[4] == 'n')
		return 5;

	// general case
	return firstNonVowelAfterVowel(word, 1);
}

size_t tcc::PorterStemming::getStartR2(const std::string& word,
	size_t startR1)
{
	if (startR1 == word.size())
		return startR1;

	return firstNonVowelAfterVowel(word, startR1 + 1);
}

size_t tcc::PorterStemming::firstNonVowelAfterVowel(const std::string& word,
	size_t start)
{
	for (size_t i = start; i != 0 && i < word.size(); ++i)
	{
		if (!isVowelY(word[i]) && isVowelY(word[i - 1]))
			return i + 1;
	}

	return word.size();
}

void tcc::PorterStemming::changeY(std::string& word)
{
	if (word[0] == 'y')
		word[0] = 'Y';

	for (size_t i = 1; i < word.size(); ++i)
	{
		if (word[i] == 'y' && isVowel(word[i - 1]))
			word[i++] = 'Y'; // skip next iteration
	}
}

/**
Step 0
*/
void tcc::PorterStemming::step0(std::string& word)
{
	// short circuit the longest suffix
	replaceIfExists(word, "'s'", "", 0) || replaceIfExists(word, "'s", "", 0)
		|| replaceIfExists(word, "'", "", 0);
}


bool tcc::PorterStemming::step1A(std::string& word)
{
	if (!replaceIfExists(word, "sses", "ss", 0))
	{
		if (endsWith(word, "ied") || endsWith(word, "ies"))
		{
			// if preceded by only one letter
			if (word.size() <= 4)
				word.pop_back();
			else
			{
				word.pop_back();
				word.pop_back();
			}
		}
		else if (endsWith(word, "s") && !endsWith(word, "us")
			&& !endsWith(word, "ss"))
		{
			if (word.size() > 2 && containsVowel(word, 0, word.size() - 2))
				word.pop_back();
		}
	}

	// special case after step 1a
	return word == "inning" || word == "outing" || word == "canning"
		|| word == "herring" || word == "earring" || word == "proceed"
		|| word == "exceed" || word == "succeed";
}


void tcc::PorterStemming::step1B(std::string& word, size_t startR1)
{
	bool exists = endsWith(word, "eedly") || endsWith(word, "eed");

	if (exists) // look only in startR1 now
		replaceIfExists(word, "eedly", "ee", startR1)
		|| replaceIfExists(word, "eed", "ee", startR1);
	else
	{
		size_t size = word.size();
		bool deleted = (containsVowel(word, 0, size - 2)
			&& replaceIfExists(word, "ed", "", 0))
			|| (containsVowel(word, 0, size - 4)
				&& replaceIfExists(word, "edly", "", 0))
			|| (containsVowel(word, 0, size - 3)
				&& replaceIfExists(word, "ing", "", 0))
			|| (containsVowel(word, 0, size - 5)
				&& replaceIfExists(word, "ingly", "", 0));

		if (deleted && (endsWith(word, "at") || endsWith(word, "bl")
			|| endsWith(word, "iz")))
			word.push_back('e');
		else if (deleted && endsInDouble(word))
			word.pop_back();
		else if (deleted && startR1 == word.size() && isShort(word))
			word.push_back('e');
	}
}

void tcc::PorterStemming::step1C(std::string& word)
{
	size_t size = word.size();
	if (size > 2 && (word[size - 1] == 'y' || word[size - 1] == 'Y'))
		if (!isVowel(word[size - 2]))
			word[size - 1] = 'i';
}


void tcc::PorterStemming::step2(std::string& word, size_t startR1)
{
	static const std::vector<std::pair<std::string, std::string>> subs
		= { { "ational", "ate" },
		{ "tional", "tion" },
		{ "enci", "ence" },
		{ "anci", "ance" },
		{ "abli", "able" },
		{ "entli", "ent" },
		{ "izer", "ize" },
		{ "ization", "ize" },
		{ "ation", "ate" },
		{ "ator", "ate" },
		{ "alism", "al" },
		{ "aliti", "al" },
		{ "alli", "al" },
		{ "fulness", "ful" },
		{ "ousli", "ous" },
		{ "ousness", "ous" },
		{ "iveness", "ive" },
		{ "iviti", "ive" },
		{ "biliti", "ble" },
		{ "bli", "ble" },
		{ "fulli", "ful" },
		{ "lessli", "less" } };

	for (auto& sub : subs)
		if (replaceIfExists(word, sub.first, sub.second, startR1))
			return;

	if (!replaceIfExists(word, "logi", "log", startR1 - 1))
	{
		// make sure we choose the longest suffix
		if (endsWith(word, "li") && !endsWith(word, "abli")
			&& !endsWith(word, "entli") && !endsWith(word, "aliti")
			&& !endsWith(word, "alli") && !endsWith(word, "ousli")
			&& !endsWith(word, "bli") && !endsWith(word, "fulli")
			&& !endsWith(word, "lessli"))
			if (word.size() > 3 && word.size() - 2 >= startR1
				&& isValidLIEnding(word[word.size() - 3]))
			{
				word.pop_back();
				word.pop_back();
			}
	}
}


void tcc::PorterStemming::step3(std::string& word, size_t startR1,
	size_t startR2)
{
	static const std::vector<std::pair<std::string, std::string>> subs
		= { { "ational", "ate" },
		{ "tional", "tion" },
		{ "alize", "al" },
		{ "icate", "ic" },
		{ "iciti", "ic" },
		{ "ical", "ic" },
		{ "ful", "" },
		{ "ness", "" } };

	for (auto& sub : subs)
		if (replaceIfExists(word, sub.first, sub.second, startR1))
			return;

	replaceIfExists(word, "ative", "", startR2);
}

void tcc::PorterStemming::step4(std::string& word, size_t startR2)
{
	static const std::vector<std::pair<std::string, std::string>> subs
		= { { "al", "" },
		{ "ance", "" },
		{ "ence", "" },
		{ "er", "" },
		{ "ic", "" },
		{ "able", "" },
		{ "ible", "" },
		{ "ant", "" },
		{ "ement", "" },
		{ "ment", "" },
		{ "ism", "" },
		{ "ate", "" },
		{ "iti", "" },
		{ "ous", "" },
		{ "ive", "" },
		{ "ize", "" } };

	for (auto& sub : subs)
		if (replaceIfExists(word, sub.first, sub.second, startR2))
			return;

	// make sure we only choose the longest suffix
	if (!endsWith(word, "ement") && !endsWith(word, "ment"))
		if (replaceIfExists(word, "ent", "", startR2))
			return;

	// short circuit
	replaceIfExists(word, "sion", "s", startR2 - 1)
		|| replaceIfExists(word, "tion", "t", startR2 - 1);
}


void tcc::PorterStemming::step5(std::string& word, size_t startR1,
	size_t startR2)
{
	size_t size = word.size();
	if (word[size - 1] == 'e')
	{
		if (size - 1 >= startR2)
			word.pop_back();
		else if (size - 1 >= startR1 && !isShort(word.substr(0, size - 1)))
			word.pop_back();
	}
	else if (word[word.size() - 1] == 'l')
	{
		if (word.size() - 1 >= startR2 && word[word.size() - 2] == 'l')
			word.pop_back();
	}
}


bool tcc::PorterStemming::isShort(const std::string& word)
{
	size_t size = word.size();

	if (size >= 3)
	{
		if (!isVowelY(word[size - 3]) && isVowelY(word[size - 2])
			&& !isVowelY(word[size - 1]) && word[size - 1] != 'w'
			&& word[size - 1] != 'x' && word[size - 1] != 'Y')
			return true;
	}
	return size == 2 && isVowelY(word[0]) && !isVowelY(word[1]);
}

bool tcc::PorterStemming::special(std::string& word)
{
	static const std::unordered_map<std::string, std::string> exceptions
		= { { "skis", "ski" },
		{ "skies", "sky" },
		{ "dying", "die" },
		{ "lying", "lie" },
		{ "tying", "tie" },
		{ "idly", "idl" },
		{ "gently", "gentl" },
		{ "ugly", "ugli" },
		{ "early", "earli" },
		{ "only", "onli" },
		{ "singly", "singl" } };

	// special cases
	auto ex = exceptions.find(word);
	if (ex != exceptions.end())
	{
		word = ex->second;
		return true;
	}

	// invariants
	return word == "sky" || word == "news" || word == "howe" || word == "atlas"
		|| word == "cosmos" || word == "bias" || word == "andes";
}

bool tcc::PorterStemming::isVowelY(char ch)
{
	return ch == 'e' || ch == 'a' || ch == 'i' || ch == 'o' || ch == 'u'
		|| ch == 'y';
}

bool tcc::PorterStemming::isVowel(char ch)
{
	return ch == 'e' || ch == 'a' || ch == 'i' || ch == 'o' || ch == 'u';
}

bool tcc::PorterStemming::endsWith(const std::string& word,
	const std::string& str)
{
	return word.size() >= str.size()
		&& std::equal(word.begin() + (word.size() - str.size()), word.end(),
			str.begin());
}

bool tcc::PorterStemming::endsInDouble(const std::string& word)
{
	if (word.size() >= 2)
	{
		char a = word[word.size() - 1];
		char b = word[word.size() - 2];

		if (a == b)
			return a == 'b' || a == 'd' || a == 'f' || a == 'g' || a == 'm'
			|| a == 'n' || a == 'p' || a == 'r' || a == 't';
	}

	return false;
}


bool tcc::PorterStemming::replaceIfExists(std::string& word, const std::string& suffix, const std::string& replacement, size_t start)
{
	int idx = (int)word.size() - (int)suffix.size();
	if (idx < start || idx < 0)
		return false;

	if (std::equal(word.begin() + idx, word.end(), suffix.begin()))
	{
		word = word.substr(0, word.size() - suffix.size()) + replacement;
		return true;
	}
	return false;
}

bool tcc::PorterStemming::isValidLIEnding(char ch)
{
	return ch == 'c' || ch == 'd' || ch == 'e' || ch == 'g' || ch == 'h'
		|| ch == 'k' || ch == 'm' || ch == 'n' || ch == 'r' || ch == 't';
}

bool tcc::PorterStemming::containsVowel(const std::string& word,
	size_t start, size_t end)
{
	if (end <= word.size())
	{
		for (size_t i = start; i < end; ++i)
			if (isVowelY(word[i]))
				return true;
	}
	return false;
}