#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "Includes.h"

using namespace sf;

struct current_time_t {
    std::wstring time;
    std::wstring date;
};

void EraseUnneededSymbols(std::wstring&);
//Character to uppercase
wchar_t ToUpper(wchar_t);

//String to uppercase
std::wstring ToUpper(std::wstring);

std::wstring FormalizeString(std::wstring);

int WSIsInt(std::wstring);

std::string wstos(std::wstring);

//Transforms current time to stringed version of it
current_time_t CurrentTimeToString();

//Restarts try
void Restart(std::wstring&, std::wstring&,
    std::vector<unsigned short>&, bool&, bool&,
    std::vector<unsigned short>&, unsigned short, bool&);

//Player enters right answer
void WinTry(unsigned short&, Sound&,
    std::wstring&, std::wstring&);

//Player enters wrong answer
void LoseTry(unsigned short&, Sound&,
    Sound&, bool, unsigned short&);

void ScanSettings(unsigned short&,
    unsigned short&,
    unsigned short&,
    unsigned short&,
    unsigned short&,
    std::wstring&);

void ScanAnswers(std::vector<std::vector<std::wstring>>&,
    std::vector<std::vector<unsigned short>>&,
    unsigned short&,
    unsigned short&);

void UploadStationResults(bool,
    unsigned int,
    std::vector<unsigned short>&,
    std::vector<std::vector<unsigned short>>&,
    std::vector<unsigned short>&,
    std::wstring&,
    std::vector <std::wstring>&);

void UploadSettings(unsigned short&,
    unsigned short&,
    unsigned short&,
    std::wstring&);

void UploadAnswers(std::vector<std::vector<std::wstring>>&);

void RewriteAnswers(std::vector<std::vector<std::wstring>>&,
    unsigned int,
    unsigned int,
    std::wstring&);