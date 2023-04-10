#include "Processing.h"

void EraseUnneededSymbols(std::wstring& str) {
    const wchar_t unneeded_symbols[] = L" :\r\n";
    for (const wchar_t a : unneeded_symbols)
        str.erase(std::remove(str.begin(), str.end(), a), str.end());
}

//Character to uppercase
wchar_t ToUpper(wchar_t convertable_char) {
    if ((int(convertable_char) >= L'а' && int(convertable_char) <= L'я') ||
        (int(convertable_char) >= L'a' && int(convertable_char) <= L'z'))
        return wchar_t(int(convertable_char) - 32);
    return convertable_char;
}

//String to uppercase
std::wstring ToUpper(std::wstring convertable_string) {
    for (wchar_t& a : convertable_string)
        a = ToUpper(a);
    return convertable_string;
}

std::wstring FormalizeString(std::wstring str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return ToUpper(str);
}

int WSIsInt(std::wstring str) {
    for (char c : str)
        if (c < L'0' || c > L'9')
            return false;
    return true;
}

std::string wstos(std::wstring wstr) {
    std::string str;
    for (char c : wstr)
        str.push_back(char(int(c)));
    return str;
}

//Transforms current time to stringed version of it
current_time_t CurrentTimeToString() {
    current_time_t current_time;
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm lt = *std::localtime(&t);
    current_time.time = std::to_wstring(lt.tm_hour) + L"-" + std::to_wstring(lt.tm_min) + L"-" + std::to_wstring(lt.tm_sec);
    current_time.date = std::to_wstring(lt.tm_year + 1900) + L"y-" + std::to_wstring(lt.tm_mon + 1) + L"m-" + std::to_wstring(lt.tm_mday) + L"d";
    return current_time;
}

//Restarts try
void Restart(std::wstring& player_answer,
    std::wstring& formalised_player_answer,
    std::vector<unsigned short>& is_answer_true,
    bool& is_changing_station_enabled,
    bool& is_app_leave_advisor_enabled,
    std::vector<unsigned short>& task_lives_container,
    unsigned short lives_per_task,
    bool& is_starting_menu_open) {
    player_answer.erase();
    formalised_player_answer.erase();
    std::fill(task_lives_container.begin(), task_lives_container.end(), lives_per_task);
    std::fill(is_answer_true.begin(), is_answer_true.end(), 2);
    is_changing_station_enabled = true;
    is_app_leave_advisor_enabled = false;
    is_starting_menu_open = true;
}

//Player enters right answer
void WinTry(unsigned short& is_answer_true, Sound& win_try_sound,
    std::wstring& player_answer, std::wstring& formalised_player_answer) {
    is_answer_true = 1;
    win_try_sound.play();
    player_answer.erase();
    formalised_player_answer.erase();
}

//Player enters wrong answer
void LoseTry(unsigned short& is_answer_true, Sound& lose_try_sound,
    Sound& total_lose_sound, bool is_out_of_lives, unsigned short& lives_per_answer) {
    if (is_out_of_lives) {
        is_answer_true = 0;
        lose_try_sound.stop();
        total_lose_sound.play();
    }
    else {
        lives_per_answer--;
        lose_try_sound.stop();
        lose_try_sound.play();
    }
}

void ScanSettings(unsigned short& amount_of_stations,
    unsigned short& amount_of_tasks,
    unsigned short& lives_per_task,
    unsigned short& window_high,
    unsigned short& window_width,
    std::wstring& window_title) {
    std::wifstream FILE_SETTINGS;
    FILE_SETTINGS.open("./Files/settings.txt", std::ios::binary);
    FILE_SETTINGS.imbue(std::locale(FILE_SETTINGS.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));

    wchar_t* file_input = new wchar_t[255];
    FILE_SETTINGS.ignore(1);
    std::wstring setting_type;
    std::wstring file_input_config_info;

    while (!FILE_SETTINGS.eof()) {
        FILE_SETTINGS.getline(file_input, 255, '\n');
        setting_type = file_input;
        setting_type.erase(std::find(setting_type.begin(), setting_type.end(), ':'), setting_type.end());
        file_input_config_info = file_input;
        file_input_config_info.erase(file_input_config_info.begin(), std::find(file_input_config_info.begin(), file_input_config_info.end(), ':'));

        if (!setting_type.compare(L"загаловок")) {
            window_title = file_input_config_info;
            window_title.erase(0, 1);
            window_title.erase(window_title.end() - 1, window_title.end());
        }
        else {
            EraseUnneededSymbols(file_input_config_info);

            if (!setting_type.compare(L"ширина_окна")) {
                window_width = std::stoi(file_input_config_info);
            }
            else if (!setting_type.compare(L"длина_окна")) {
                window_high = std::stoi(file_input_config_info);
            }
            else if (!setting_type.compare(L"количество_станций")) {
                if (std::stoi(file_input_config_info) > 99)
                    amount_of_stations = 99;
                else if (std::stoi(file_input_config_info) < 1)
                    amount_of_stations = 1;
                else amount_of_stations = std::stoi(file_input_config_info);
            }
            else if (!setting_type.compare(L"количество_заданий_на_станцию")) {
                if (std::stoi(file_input_config_info) > 999)
                    amount_of_tasks = 999;
                else if (std::stoi(file_input_config_info) < 1)
                    amount_of_tasks = 1;
                else amount_of_tasks = std::stoi(file_input_config_info);
            }
            else if (!setting_type.compare(L"количество_попыток_на_станцию")) {
                if (std::stoi(file_input_config_info) > 99)
                    lives_per_task = 99;
                else if (std::stoi(file_input_config_info) < 1)
                    lives_per_task = 1;
                else lives_per_task = std::stoi(file_input_config_info);
            }
        }
    }

    delete[] file_input;
    FILE_SETTINGS.close();
}

void ScanAnswers(std::vector<std::vector<std::wstring>>& answers,
    std::vector<std::vector<unsigned short>>& skipped_tasks_iterators,
    unsigned short& amount_of_stations,
    unsigned short& amount_of_tasks) {
    std::wifstream FILE_ANSWERS;
    FILE_ANSWERS.open("./Files/answers.txt", std::ios::binary);
    FILE_ANSWERS.imbue(std::locale(FILE_ANSWERS.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));
    FILE_ANSWERS.ignore(1);
    wchar_t* file_input = new wchar_t[255];
    std::wstring answer_inputted;

    int counter_1 = 0, counter_2 = 0;
    while (!FILE_ANSWERS.eof()) {
        FILE_ANSWERS.getline(file_input, 255, '\n');
        answer_inputted = file_input;
        if (answer_inputted[answer_inputted.size() - 1] == L'\r')
            answer_inputted.pop_back();

        if (counter_1 >= amount_of_stations)
            break;

        if (answer_inputted.empty())
            continue;
        else if (answer_inputted[0] == L'%') {
            if (WSIsInt(answer_inputted.substr(1, answer_inputted.size() - 2)))
                counter_1 = stoi(wstos(answer_inputted.substr(1, answer_inputted.size() - 2))) - 1;
        }
        else {
            answers[counter_1][counter_2] = FormalizeString(answer_inputted);
            if (counter_2 + 1 >= amount_of_tasks) {
                counter_2 = 0;
                counter_1++;
                if (counter_1 >= amount_of_stations)
                    break;
            }
            else counter_2++;
        }
    }

    for (int i = 0; i < answers.size(); i++) {
        for (int j = 0; j < answers[i].size(); j++) {
            if (answers[i][j][1] == L'!')
                skipped_tasks_iterators[i].push_back(j);
        }
    }

    delete[] file_input;
    FILE_ANSWERS.close();
}

void UploadStationResults(bool is_focus_lost_when_leave_advisor_enabled,
    unsigned int current_station,
    std::vector<unsigned short>& is_answer_true,
    std::vector<std::vector<unsigned short>>& skipped_tasks_iterators,
    std::vector<unsigned short>& task_lives_container,
    std::wstring& team_name,
    std::vector <std::wstring>& escape_app_times) {
    std::wstring file_path = L"./Results/" + team_name + L"_" + CurrentTimeToString().date + L"_station_" + std::to_wstring(current_station) + L".txt";
    std::wofstream FILE_STATION_RESULTS;
    FILE_STATION_RESULTS.open(file_path, std::ios::binary);
    FILE_STATION_RESULTS.imbue(std::locale(FILE_STATION_RESULTS.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));

    if (is_focus_lost_when_leave_advisor_enabled) {
        for (std::wstring a : escape_app_times)
            FILE_STATION_RESULTS << L"Приложения было покинуто пользователем во время его работы в: " << a << L"\r\n";
    }
    for (int i = 0; i < is_answer_true.size(); i++) {
        bool is_task_skipped = false;
        for (int j = 0; j < skipped_tasks_iterators[current_station - 1].size(); j++) {
            if (skipped_tasks_iterators[current_station - 1][j] == i)
                is_task_skipped = true;
        }
        if (is_task_skipped)
            FILE_STATION_RESULTS << i + 1 << L"\tнет задания\r\n";
        else {
            if (is_answer_true[i] == true)
                FILE_STATION_RESULTS << i + 1 << L"\t+\r\n";
            else if (is_answer_true[i] == false)
                FILE_STATION_RESULTS << i + 1 << L"\t-\r\n";
            else FILE_STATION_RESULTS << i + 1 << L'\t' << task_lives_container[i] << L" осталось\r\n";
        }
    }

    FILE_STATION_RESULTS.close();
}

void UploadSettings(unsigned short& amount_of_stations,
    unsigned short& amount_of_tasks,
    unsigned short& lives_per_task,
    std::wstring& window_title) {
    std::wofstream FILE_SETTINGS;
    FILE_SETTINGS.open("./Files/settings.txt", std::ios::binary);
    FILE_SETTINGS.imbue(std::locale(FILE_SETTINGS.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));

    FILE_SETTINGS << L"загаловок: " << window_title << L"\r\n\n"
        << L"количество_станций: " << amount_of_stations << L"\r\n\n"
        << L"количество_заданий_на_станцию: " << amount_of_tasks << L"\r\n\n"
        << L"количество_попыток_на_станцию: " << lives_per_task << L"\r\n";

    FILE_SETTINGS.close();
}

void UploadAnswers(std::vector<std::vector<std::wstring>>& answers) {
    std::wofstream FILE_ANSWERS;
    FILE_ANSWERS.open("./Files/answers.txt", std::ios::binary);
    FILE_ANSWERS.imbue(std::locale(FILE_ANSWERS.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>));

    for (int i = 0; i < answers.size(); i++) {
        FILE_ANSWERS << L"%" << std::to_wstring(i + 1) << L"%\r\n";
        for (int j = 0; j < answers[i].size(); j++) {
            FILE_ANSWERS << answers[i][j] << L"\r\n";
        }
    }

    FILE_ANSWERS.close();
}

void RewriteAnswers(std::vector<std::vector<std::wstring>>& answers,
    unsigned int current_station,
    unsigned int task_number,
    std::wstring& new_answer) {
    if (new_answer.empty() == false)
        answers[current_station - 1][task_number - 1] = new_answer;
}