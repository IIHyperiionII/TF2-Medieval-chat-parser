/*
    TF2 present language to medieval parser
    Replaces set words with their medieval counterpart
    The set words that get replaced and their replacements are in a dict.json (can be freely added)
    In TF2 the replacing has a chance built in, this swaps always :) (not counting prepend/append)
    Can be used as a basic present->medieval 'translator'

    Usage:

    */

/*
Replacing rules/order:
1, Only replace whole words, not parts
2, Pick random replacements
3, Case-insensitive
4, Prioritize phrases before individual words ('thank you' > 'thank' 'you')
5, Recursively replace tags (&)
6. Prepend/Append with a chance, not 100%
7, Replace interpunction
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <ctime>
#include <cstdlib>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

struct Dictionary {
    map<string, vector<string>> phrases;      // "thank you" -> [...]
    map<string, vector<string>> singleWords;  // "you" -> [...]
    map<string, vector<string>> tags;         // "&god" -> [...]
    map<string, vector<string>> punctuation;  // "!" -> [...]
    vector<string> prepends;
    vector<string> appends;
};

Dictionary dict;

static void loadDictionary(const string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Can't open " << filename << endl;
        return;
    }

    json j;
    file >> j;

    // 1. Load Prepend/Append
    dict.prepends = j["prepended_words"].get<vector<string>>();
    dict.appends = j["appended_words"].get<vector<string>>();

    // 2. Load Single Replacements (and split to phrases/words)
    for (auto& it : j["single_replacements"].items()) {
        string key = it.key();
        vector<string> values = it.value();

        if (key.find(' ') != string::npos) {
            dict.phrases[key] = values;
        }
        else {
            dict.singleWords[key] = values;
        }
    }

    // 3. Load tags (&)
    for (auto& it : j["change_tags"].items()) {
        dict.tags[it.key()] = it.value().get<vector<string>>();
    }

    // 4. Load punctuation
    for (auto& it : j["punctuation"].items()) {
        dict.punctuation[it.key()] = it.value().get<vector<string>>();
    }
}

static string pickRandom(const vector<string>& options) {
    if (options.empty()) return "";
    return options[rand() % options.size()];
}

int main()
{
    // 1. Load dictionary and seed rand
    loadDictionary("./dict.json");
    srand(static_cast<unsigned int>(time(nullptr)));

    // 2. Read input   
    string og_string;
    cout << "Write your present-language sentence: " << endl;

    while (true)
    {
        getline(cin, og_string);

        // 3. Replace multi-word phrases
        for (auto const& [phrase, replacement] : dict.phrases)
        {
            regex reg("\\b" + phrase + "\\b", regex_constants::icase);
            og_string = regex_replace(og_string, reg, pickRandom(replacement));
        }

        // 4. Replace single words
        for (auto const& [word, replacement] : dict.singleWords)
        {
            regex reg("\\b" + word + "\\b", regex_constants::icase);
            og_string = regex_replace(og_string, reg, pickRandom(replacement));
        }

        // 5. Add prepend/append - 20% chance for both
        if (rand() % 10 < 2)
        {
            og_string = (pickRandom(dict.prepends) + og_string);
        }

        if (rand() % 10 < 2)
        {
            if (!og_string.empty() && og_string.back() == '.') {
                og_string.pop_back();
            }
            og_string += ", " + pickRandom(dict.appends);
        }

        // 6. Replace all tags &
        int safety = 0;
        bool foundTag = true;
        do {
            foundTag = false;
            for (auto const& [tag, replacement] : dict.tags)
            {
                regex reg(tag + "\\b", regex_constants::icase);
                smatch match;
                while (regex_search(og_string, match, reg)) {
                    foundTag = true;
                    og_string = regex_replace(og_string, reg, pickRandom(replacement), regex_constants::format_first_only);
                }
            }
            safety++;
        } while (foundTag && safety < 20);

        // 7. Replace punctuation
        for (auto const& [punct, replacement] : dict.punctuation)
        {
            regex reg("\\" + punct);
            og_string = regex_replace(og_string, reg, pickRandom(replacement));
        }

        // 8. Print sentence
        cout << og_string << endl;
    }

    return 0;
}

