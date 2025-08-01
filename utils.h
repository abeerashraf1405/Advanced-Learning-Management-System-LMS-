#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

namespace Utils 
{
    vector<string> splitString(const string& s, char delimiter)
    {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) 
        {
            // Trim whitespace from each token
            token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
            token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
            if (!token.empty()) 
            {
                tokens.push_back(token);
            }
        }
        return tokens;
    }

    string joinString(const vector<string>& vec, const string& delimiter)
    {
        if (vec.empty()) 
            return "";

        string result;
        for (size_t i = 0; i < vec.size() - 1; ++i) 
        {
            result += vec[i] + delimiter;
        }
        result += vec.back();
        return result;
    }

    string getCurrentDate()
    {
        time_t now = time(nullptr);
        tm ltm;
        localtime_s(&ltm, &now);

        stringstream ss;
        ss << 1900 + ltm.tm_year << "-" << setw(2) << setfill('0') << 1 + ltm.tm_mon << "-" << setw(2) << setfill('0') << ltm.tm_mday;
        return ss.str();
    }

    bool validateDate(const string& date)
    {
        if (date.length() != 10)
            return false;
        if (date[4] != '-' || date[7] != '-') 
            return false;

        try {
            int year = stoi(date.substr(0, 4));
            int month = stoi(date.substr(5, 2));
            int day = stoi(date.substr(8, 2));

            if (year < 1900 || year > 2100) 
                return false;
            if (month < 1 || month > 12)
                return false;
            if (day < 1 || day > 31)
                return false;

            // Additional validation for specific months
            if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) 
                return false;
            if (month == 2) 
            {
                bool isLeap = (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
                if (day > (isLeap ? 29 : 28)) 
                    return false;
            }

            return true;
        }
        catch (...) {
            return false;
        }
    }

    bool validateGrade(int grade) 
    {
        return grade >= 0 && grade <= 100;
    }

    void clearInputBuffer() 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    vector<string> parseRecord(const string& record) 
    {
        vector<string> parts;
        size_t start = 0;
        size_t end = record.find('|');

        while (end != string::npos)
        {
            parts.push_back(record.substr(start, end - start));
            start = end + 1;
            end = record.find('|', start);
        }
        parts.push_back(record.substr(start));

        // Trim whitespace from each part
        for (auto& part : parts) 
        {
            part.erase(part.find_last_not_of(" \t\n\r\f\v") + 1);
            part.erase(0, part.find_first_not_of(" \t\n\r\f\v"));
        }

        return parts;
    }

    string toLower(const string& s) 
    {
        string result = s;
        transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
            {
            return tolower(c);
            });
        return result;
    }

    string toUpper(const string& s) 
    {
        string result = s;
        transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
            {
            return toupper(c);
            });
        return result;
    }
}

#endif 
