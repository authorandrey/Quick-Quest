#define uint unsigned int

#include "Includes.h"
#include "Processing.h"
#include "STObjectClass.h"
#include "GlobalVariables.h"

using namespace sf;

int main() {
    setlocale(LC_ALL, "Russian");

    unsigned short amount_of_stations = 1;
    unsigned short amount_of_tasks = 1;
    unsigned short current_station = 1;
    unsigned short lives_per_task = 3;
    unsigned short window_high = 600;
    unsigned short window_width = 800;
    std::wstring window_title = L"Competition creator";
    unsigned short delta_tasks = 0;
    Color elements_color = Color(158, 158, 158);

    //Player info
    unsigned short player_wins = 0;
    bool is_changing_station_enabled = true;
    bool is_app_leave_advisor_enabled = false;
    bool is_focus_lost = false;
    bool is_focus_lost_when_leave_advisor_enabled = false;
    bool is_changing_settings_mode_enabled = false;
    bool is_starting_menu_open = true;
    std::wstring team_name = L"";
    std::vector <std::wstring> escape_app_times;

    ScanSettings(amount_of_stations, amount_of_tasks, lives_per_task, window_high, window_width, window_title);

    std::vector<std::vector<std::wstring>> answers(amount_of_stations, std::vector<std::wstring>(amount_of_tasks, L"%!%"));
    std::vector<std::vector<unsigned short>> skipped_tasks_iterators(amount_of_stations);

    ScanAnswers(answers, skipped_tasks_iterators, amount_of_stations, amount_of_tasks);

    std::wstring player_answer;
    std::wstring formalised_player_answer;
    //0 - answer is false, 1 - answer is true, 2 - answer is undefined
    std::vector<unsigned short> is_answer_true(amount_of_tasks, 2);
    std::vector<unsigned short> task_lives_container(amount_of_tasks, lives_per_task);

    ContextSettings textures_settings;
    textures_settings.antialiasingLevel = 4;
    Image icon;
    if (!icon.loadFromFile("./Files/Textures/icon.png"))
        return -1;

    RenderWindow window(VideoMode(window_width, window_high), L"", Style::Close, textures_settings);
    window.setFramerateLimit(17);
    window.setTitle(window_title);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    Clock clock, start_menu_clock;

    Font font;
    if (!font.loadFromFile("./Files/Other/arial.ttf"))
        return -1;

    STObject restart_button;
    if (!restart_button.texture.loadFromFile("./Files/Textures/restart_button.png"))
        return -1;
    restart_button.sprite.setTexture(restart_button.texture);
    restart_button.sprite.setPosition(window_width - restart_button.texture.getSize().x, 0);
    IntRect restart_button_position_rectangle(
        restart_button.sprite.getPosition().x, restart_button.sprite.getPosition().y,
        restart_button.texture.getSize().x, restart_button.texture.getSize().y);

    STObject screen_lock_button;
    if (!screen_lock_button.texture.loadFromFile("./Files/Textures/screen_lock_texture.png"))
        return -1;
    screen_lock_button.sprite.setTexture(screen_lock_button.texture);
    screen_lock_button.sprite.setPosition(restart_button.sprite.getPosition().x - screen_lock_button.texture.getSize().x, 0);
    IntRect screen_lock_button_position_rectangle(
        screen_lock_button.sprite.getPosition().x, screen_lock_button.sprite.getPosition().y,
        screen_lock_button.texture.getSize().x, screen_lock_button.texture.getSize().y);

    STObject app_leave_advisor_button;
    if (!app_leave_advisor_button.texture.loadFromFile("./Files/Textures/leaving_app_area_advisor.png"))
        return -1;
    app_leave_advisor_button.sprite.setTexture(app_leave_advisor_button.texture);
    app_leave_advisor_button.sprite.setPosition(screen_lock_button.sprite.getPosition().x - app_leave_advisor_button.texture.getSize().x, 0);
    IntRect app_leave_advisor_position_rectangle(
        app_leave_advisor_button.sprite.getPosition().x, app_leave_advisor_button.sprite.getPosition().y,
        app_leave_advisor_button.texture.getSize().x, app_leave_advisor_button.texture.getSize().y);

    STObject upload_station_results_button;
    if (!upload_station_results_button.texture.loadFromFile("./Files/Textures/upload_station_results_button.png"))
        return -1;
    upload_station_results_button.sprite.setTexture(upload_station_results_button.texture);
    upload_station_results_button.sprite.setPosition(app_leave_advisor_button.sprite.getPosition().x - upload_station_results_button.texture.getSize().x, 0);
    IntRect upload_station_results_position_rectangle(
        upload_station_results_button.sprite.getPosition().x, upload_station_results_button.sprite.getPosition().y,
        upload_station_results_button.texture.getSize().x, upload_station_results_button.texture.getSize().y);

    STObject settings_button;
    if (!settings_button.texture.loadFromFile("./Files/Textures/settings_button.png"))
        return -1;
    settings_button.sprite.setTexture(settings_button.texture);
    settings_button.sprite.setPosition(upload_station_results_button.sprite.getPosition().x - settings_button.texture.getSize().x, 0);
    IntRect settings_button_position_rectangle(
        settings_button.sprite.getPosition().x, settings_button.sprite.getPosition().y,
        settings_button.texture.getSize().x, settings_button.texture.getSize().y);

    STObject load_settngs_button;
    if (!load_settngs_button.texture.loadFromFile("./Files/Textures/load_settings.png"))
        return -1;
    load_settngs_button.sprite.setTexture(load_settngs_button.texture);
    load_settngs_button.sprite.setPosition(settings_button.sprite.getPosition().x - load_settngs_button.texture.getSize().x, 0);
    IntRect load_settngs_button_position_rectangle(
        load_settngs_button.sprite.getPosition().x, load_settngs_button.sprite.getPosition().y,
        load_settngs_button.texture.getSize().x, load_settngs_button.texture.getSize().y);

    STObject upload_settings_button;
    if (!upload_settings_button.texture.loadFromFile("./Files/Textures/upload_settings.png"))
        return -1;
    upload_settings_button.sprite.setTexture(upload_settings_button.texture);
    upload_settings_button.sprite.setPosition(load_settngs_button.sprite.getPosition().x - upload_settings_button.texture.getSize().x, 0);
    IntRect upload_settings_button_position_rectangle(
        upload_settings_button.sprite.getPosition().x, upload_settings_button.sprite.getPosition().y,
        upload_settings_button.texture.getSize().x, upload_settings_button.texture.getSize().y);

    STObject background;
    if (!background.texture.loadFromFile("./Files/Textures/background.png"))
        return -1;
    background.sprite.setTexture(background.texture);
    background.sprite.setPosition(0, 0);
    background.sprite.setScale((float)window.getSize().x / background.texture.getSize().x,
        (float)window.getSize().y / background.texture.getSize().y);

    STObject station_changing_arrow;
    if (!station_changing_arrow.texture.loadFromFile("./Files/Textures/station_changing_arrow.png"))
        return -1;
    station_changing_arrow.sprite.setTexture(station_changing_arrow.texture);

    STObject element_increment;
    if (!element_increment.texture.loadFromFile("./Files/Textures/plus_element_button.png"))
        return -1;
    element_increment.sprite.setTexture(element_increment.texture);

    STObject element_decrement;
    if (!element_decrement.texture.loadFromFile("./Files/Textures/delete_element_button.png"))
        return -1;
    element_decrement.sprite.setTexture(element_decrement.texture);

    STObject heart;
    if (!heart.texture.loadFromFile("./Files/Textures/heart.png"))
        return -1;
    heart.sprite.setTexture(heart.texture);
    heart.sprite.setOrigin(heart.texture.getSize().x / 2, heart.texture.getSize().y / 2);

    STObject rewrite_answer;
    if (!rewrite_answer.texture.loadFromFile("./Files/Textures/rewrite_answer.png"))
        return -1;
    rewrite_answer.sprite.setTexture(rewrite_answer.texture);
    rewrite_answer.sprite.setOrigin(rewrite_answer.texture.getSize().x / 2, rewrite_answer.texture.getSize().y / 2);

    CircleShape room_choose((window.getSize().x + window.getSize().y) / 40);
    room_choose.setFillColor(elements_color);
    room_choose.setPosition(45, 15);

    CircleShape answer_confirm_button(55);
    answer_confirm_button.setFillColor(elements_color);

    CircleShape player_answer_input_1((window.getSize().x + window.getSize().y) / 14);
    player_answer_input_1.setFillColor(elements_color);
    player_answer_input_1.setPosition(window.getSize().x / 70, window.getSize().y / 4);
    CircleShape player_answer_input_2((window.getSize().x + window.getSize().y) / 14);
    player_answer_input_2.setFillColor(elements_color);
    player_answer_input_2.setPosition(window.getSize().x - player_answer_input_1.getPosition().x - player_answer_input_2.getRadius() * 2,
        window.getSize().y / 4);
    RectangleShape player_answer_input_3(
        Vector2f(player_answer_input_2.getPosition().x - player_answer_input_1.getPosition().x,
            player_answer_input_2.getRadius() * 2));
    player_answer_input_3.setFillColor(elements_color);
    player_answer_input_3.setPosition(player_answer_input_1.getPosition().x + player_answer_input_1.getRadius(),
        player_answer_input_1.getPosition().y);

    CircleShape team_name_input_1((window.getSize().x + window.getSize().y) / 40);
    team_name_input_1.setFillColor(elements_color);
    team_name_input_1.setPosition(window.getSize().x / 5, window.getSize().y / 3.0);
    CircleShape team_name_input_2(team_name_input_1.getRadius());
    team_name_input_2.setFillColor(elements_color);
    team_name_input_2.setPosition(window.getSize().x - team_name_input_1.getPosition().x - team_name_input_2.getRadius() * 2,
        team_name_input_1.getPosition().y);
    RectangleShape team_name_input_3(
        Vector2f(team_name_input_2.getPosition().x - team_name_input_1.getPosition().x,
            team_name_input_2.getRadius() * 2));
    team_name_input_3.setFillColor(elements_color);
    team_name_input_3.setPosition(team_name_input_1.getPosition().x + team_name_input_1.getRadius(),
        team_name_input_1.getPosition().y);

    CircleShape start_button_1((window.getSize().x + window.getSize().y) / 20);
    start_button_1.setFillColor(elements_color);
    start_button_1.setPosition(window.getSize().x / 3, window.getSize().y / 1.8);
    CircleShape start_button_2(start_button_1.getRadius());
    start_button_2.setFillColor(elements_color);
    start_button_2.setPosition(window.getSize().x - start_button_1.getPosition().x - start_button_2.getRadius() * 2,
        start_button_1.getPosition().y);
    RectangleShape start_button_3(
        Vector2f(start_button_2.getPosition().x - start_button_1.getPosition().x,
            start_button_2.getRadius() * 2));
    start_button_3.setFillColor(elements_color);
    start_button_3.setPosition(start_button_1.getPosition().x + start_button_1.getRadius(),
        start_button_1.getPosition().y);
    Text start_button_text;
    start_button_text.setFont(font);
    start_button_text.setCharacterSize(75);
    start_button_text.setFillColor(Color::Black);
    start_button_text.setPosition(start_button_1.getPosition().x + 15, start_button_1.getPosition().y + 15);
    start_button_text.setString(L"Начать");
    IntRect start_button_position_rectangle(int(start_button_1.getPosition().x),
        int(start_button_1.getPosition().y),
        start_button_1.getRadius() * 2 + start_button_3.getSize().x,
        start_button_3.getSize().y);

    Text team_name_text;
    team_name_text.setFont(font);
    team_name_text.setCharacterSize(50);
    team_name_text.setFillColor(Color::Black);
    team_name_text.setPosition(3, window.getSize().y - 53);
    team_name_text.setString(team_name);

    RectangleShape team_name_show_zone(Vector2f(300, 45));
    team_name_show_zone.setFillColor(elements_color);
    team_name_show_zone.setPosition(0, window.getSize().y - 45);
    CircleShape team_name_show_zone_edge(team_name_show_zone.getSize().y / 2.0);
    team_name_show_zone_edge.setFillColor(elements_color);
    team_name_show_zone_edge.setPosition(team_name_show_zone.getSize().x - team_name_show_zone_edge.getRadius(),
        window.getSize().y - 2 * team_name_show_zone_edge.getRadius());

    Text input_text;
    input_text.setFont(font);
    input_text.setCharacterSize(50);
    input_text.setFillColor(Color::Black);
    input_text.setPosition(player_answer_input_3.getPosition().x, player_answer_input_3.getPosition().y + 25);

    Text station_text;
    station_text.setFont(font);
    station_text.setCharacterSize(room_choose.getRadius() * 2);
    station_text.setFillColor(Color::Black);
    Vector2f station_text_position_under_10 = Vector2f(room_choose.getPosition().x + room_choose.getRadius() / 2.3,
        room_choose.getPosition().y - room_choose.getRadius() / 3.0);
    Vector2f station_text_position_grater_10 = Vector2f(room_choose.getPosition().x + room_choose.getRadius() / 7.3,
        room_choose.getPosition().y - room_choose.getRadius() / 15.0);

    Text task_number_text;
    task_number_text.setFont(font);
    task_number_text.setCharacterSize(50);
    task_number_text.setFillColor(Color(158, 158, 158, 230));

    Text remained_lives_text;
    remained_lives_text.setFont(font);
    remained_lives_text.setFillColor(Color(158, 158, 158, 230));

    SoundBuffer lose_try_sound_buffer;
    if (!lose_try_sound_buffer.loadFromFile("./Files/Sounds/lose_try_sound.wav"))
        return -1;
    Sound lose_try_sound;
    lose_try_sound.setBuffer(lose_try_sound_buffer);

    SoundBuffer win_try_sound_buffer;
    if (!win_try_sound_buffer.loadFromFile("./Files/Sounds/win_try_sound.wav"))
        return -1;
    Sound win_try_sound;
    win_try_sound.setBuffer(win_try_sound_buffer);

    SoundBuffer total_sound_buffer;
    if (!total_sound_buffer.loadFromFile("./Files/Sounds/total_lose_sound.wav"))
        return -1;
    Sound total_lose_sound;
    total_lose_sound.setBuffer(total_sound_buffer);
    total_lose_sound.setPitch(1.6);

    //Positions and rectangles that describes positions and sizes
    Vector2f station_changing_arrow_right_position(
        room_choose.getPosition().x + (float)13 / 6 * room_choose.getRadius(),
        room_choose.getPosition().y + room_choose.getRadius() - station_changing_arrow.texture.getSize().y / 2);

    Vector2f station_changing_arrow_left_position(
        room_choose.getPosition().x - (float)1 / 6 * room_choose.getRadius(),
        room_choose.getPosition().y + room_choose.getRadius() + station_changing_arrow.texture.getSize().y / 2);

    Vector2f taskline_changing_arrow_right_position(
        window.getSize().x - 100,
        (window.getSize().y + player_answer_input_1.getRadius() * 2.0 + player_answer_input_1.getPosition().y) /
        2.0 - station_changing_arrow.texture.getSize().y / 2.0);

    Vector2f taskline_changing_arrow_left_position(
        100,
        (window.getSize().y + player_answer_input_1.getRadius() * 2.0 + player_answer_input_1.getPosition().y) /
        2.0 + station_changing_arrow.texture.getSize().y / 2.0);

    Vector2f station_addiction_position = station_changing_arrow_right_position;
    Vector2f tasks_addiction_position = taskline_changing_arrow_right_position;
    Vector2f station_delete_position(station_changing_arrow_right_position.x + element_increment.texture.getSize().x + 5,
        station_changing_arrow_right_position.y);
    Vector2f tasks_delete_position(taskline_changing_arrow_right_position.x + element_increment.texture.getSize().x + 5,
        taskline_changing_arrow_right_position.y);
    IntRect station_delete_position_rectangle(station_delete_position.x, station_delete_position.y,
        element_decrement.texture.getSize().x, element_decrement.texture.getSize().y);
    IntRect tasks_delete_position_rectangle(tasks_delete_position.x, tasks_delete_position.y,
        element_decrement.texture.getSize().x, element_decrement.texture.getSize().y);

    IntRect station_changing_arrow_right_position_rectangle(
        station_changing_arrow_right_position.x, station_changing_arrow_right_position.y,
        station_changing_arrow.texture.getSize().x, station_changing_arrow.texture.getSize().y);

    IntRect station_changing_arrow_left_position_rectangle(
        station_changing_arrow_left_position.x - station_changing_arrow.texture.getSize().x,
        station_changing_arrow_left_position.y - station_changing_arrow.texture.getSize().y,
        station_changing_arrow.texture.getSize().x, station_changing_arrow.texture.getSize().y);

    IntRect taskline_changing_arrow_right_position_rectangle(
        taskline_changing_arrow_right_position.x, taskline_changing_arrow_right_position.y,
        station_changing_arrow.texture.getSize().x, station_changing_arrow.texture.getSize().y);

    IntRect taskline_changing_arrow_left_position_rectangle(
        taskline_changing_arrow_left_position.x - station_changing_arrow.texture.getSize().x,
        taskline_changing_arrow_left_position.y - station_changing_arrow.texture.getSize().y,
        station_changing_arrow.texture.getSize().x, station_changing_arrow.texture.getSize().y);

    Vector2i current_mouse_position;

    heart.sprite.setScale((float)answer_confirm_button.getRadius() * 1.6 / heart.texture.getSize().x,
        (float)answer_confirm_button.getRadius() * 1.6 / heart.texture.getSize().y);
    rewrite_answer.sprite.setScale((float)answer_confirm_button.getRadius() * 2.6 / heart.texture.getSize().x,
        (float)answer_confirm_button.getRadius() * 2.6 / heart.texture.getSize().y);
    remained_lives_text.setCharacterSize(110 / (float(heart.sprite.getScale().x) + heart.sprite.getScale().y));
    remained_lives_text.setOrigin(remained_lives_text.getCharacterSize() / 3.0,
        remained_lives_text.getCharacterSize() / 1.6);

    //Main loop
    while (window.isOpen()) {

        //Event catcher
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (is_focus_lost == false) {
                if (event.type == Event::MouseButtonPressed) {
                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        current_mouse_position = Mouse::getPosition(window);
                        if (is_starting_menu_open && start_button_position_rectangle.contains(current_mouse_position) && team_name.size() > 0) {
                            is_starting_menu_open = !is_starting_menu_open;
                            team_name_text.setString(team_name);
                        }
                        //Restart button click
                        else if (restart_button_position_rectangle.contains(current_mouse_position)) {
                            Restart(player_answer,
                                formalised_player_answer,
                                is_answer_true,
                                is_changing_station_enabled,
                                is_app_leave_advisor_enabled,
                                task_lives_container,
                                lives_per_task,
                                is_starting_menu_open);
                        }
                        //Station increasing by arrow click
                        else if (is_changing_station_enabled && station_changing_arrow_right_position_rectangle.contains(current_mouse_position)) {
                            if (is_changing_settings_mode_enabled && current_station == amount_of_stations) {
                                amount_of_stations++;
                                current_station++;
                                skipped_tasks_iterators.resize(amount_of_stations);
                                answers.resize(amount_of_stations);
                                for (auto& a : answers[amount_of_stations - 1])
                                    a = L"%!%";
                                for (int i = 0; i < amount_of_tasks; i++)
                                    skipped_tasks_iterators[amount_of_stations - 1].push_back(i);
                            }
                            else {
                                if (current_station == amount_of_stations)
                                    current_station = 1;
                                else current_station++;
                            }
                        }
                        //Station decreasing by arrow click
                        else if (is_changing_station_enabled && station_changing_arrow_left_position_rectangle.contains(current_mouse_position)) {
                            if (current_station == 1)
                                current_station = amount_of_stations;
                            else current_station--;
                        }
                        //Delete last station
                        else if (is_changing_settings_mode_enabled && station_delete_position_rectangle.contains(current_mouse_position)) {
                            if (current_station == amount_of_stations) {
                                if (amount_of_stations > 1) {
                                    amount_of_stations--;
                                    current_station--;
                                    skipped_tasks_iterators.resize(amount_of_stations);
                                    answers.resize(amount_of_stations);
                                }
                            }
                        }
                        //Taskline increasing by arrow click
                        else if (taskline_changing_arrow_right_position_rectangle.contains(current_mouse_position)) {
                            if (is_changing_settings_mode_enabled && delta_tasks + 5 >= amount_of_tasks) {
                                amount_of_tasks++;
                                if (amount_of_tasks > 5)
                                    delta_tasks++;
                                task_lives_container.resize(amount_of_tasks);
                                task_lives_container[amount_of_tasks - 1] = lives_per_task;
                                is_answer_true.resize(amount_of_tasks);
                                is_answer_true[amount_of_tasks - 1] = 2;
                                for (int i = 0; i < amount_of_stations; i++) {
                                    answers[i].resize(amount_of_tasks);
                                    answers[i][amount_of_tasks - 1] = L"%!%";
                                    skipped_tasks_iterators[i].push_back(amount_of_tasks - 1);
                                }
                            }
                            else {
                                if (delta_tasks >= amount_of_tasks - 5)
                                    delta_tasks = 0;
                                else delta_tasks++;
                            }
                        }
                        //Taskline decreasing by arrow click
                        else if (amount_of_tasks > 5 && taskline_changing_arrow_left_position_rectangle.contains(current_mouse_position)) {
                            if (delta_tasks == 0)
                                delta_tasks = amount_of_tasks - 5;
                            else delta_tasks--;
                        }
                        //Taskline deleting last task
                        else if (is_changing_settings_mode_enabled && delta_tasks + 5 >= amount_of_tasks && amount_of_tasks > 1
                            && tasks_delete_position_rectangle.contains(current_mouse_position)) {
                            amount_of_tasks--;
                            if (amount_of_tasks >= 5)
                                delta_tasks--;
                            for (int i = 0; i < answers.size(); i++) {
                                answers[i].resize(amount_of_tasks);
                            }
                            is_answer_true.resize(amount_of_tasks);
                            task_lives_container.resize(amount_of_tasks);
                            skipped_tasks_iterators.resize(amount_of_stations);
                        }
                        //Screen lock button click
                        else if (screen_lock_button_position_rectangle.contains(current_mouse_position)) {
                            is_changing_station_enabled = false;
                        }
                        //App leave advisor
                        else if (app_leave_advisor_position_rectangle.contains(current_mouse_position)) {
                            is_app_leave_advisor_enabled = true;
                        }
                        //Upload station results to file
                        else if (upload_station_results_position_rectangle.contains(current_mouse_position)) {
                            UploadStationResults(is_focus_lost_when_leave_advisor_enabled,
                                current_station,
                                is_answer_true,
                                skipped_tasks_iterators,
                                task_lives_container,
                                team_name,
                                escape_app_times);
                        }
                        //Settings button
                        else if (settings_button_position_rectangle.contains(current_mouse_position)) {
                            if (team_name.compare(L"%root%") == 0)
                                is_changing_settings_mode_enabled = !is_changing_settings_mode_enabled;
                        }
                        //Load settings and answer from files
                        else if (is_changing_settings_mode_enabled && load_settngs_button_position_rectangle.contains(current_mouse_position)) {
                            current_station = 1;
                            delta_tasks = 0;
                            ScanSettings(amount_of_stations, amount_of_tasks, lives_per_task, window_high, window_width, window_title);
                            window.setTitle(window_title);
                            skipped_tasks_iterators.clear();
                            skipped_tasks_iterators.resize(amount_of_stations);
                            answers.clear();
                            answers.resize(amount_of_stations);
                            for (auto& part : answers)
                                part.resize(amount_of_tasks, L"%!%");
                            ScanAnswers(answers, skipped_tasks_iterators, amount_of_stations, amount_of_tasks);
                            is_answer_true.clear();
                            is_answer_true.resize(amount_of_tasks, 2);
                            task_lives_container.clear();
                            task_lives_container.resize(amount_of_tasks, lives_per_task);
                        }
                        else if (is_changing_settings_mode_enabled && upload_settings_button_position_rectangle.contains(current_mouse_position)) {
                            UploadSettings(amount_of_stations,
                                amount_of_tasks,
                                lives_per_task,
                                window_title);
                            UploadAnswers(answers);
                        }
                        //Answer buttons click
                        else
                            for (int i = 1; amount_of_tasks >= 5 && i <= 5 || amount_of_tasks < 5 && i <= amount_of_tasks; i++) {
                                if (IntRect(120 * i - 15,
                                    (window.getSize().y + player_answer_input_1.getRadius() * 2.0 + player_answer_input_1.getPosition().y) / 2.0 - answer_confirm_button.getRadius(),
                                    answer_confirm_button.getRadius() * 2,
                                    answer_confirm_button.getRadius() * 2).contains(current_mouse_position)) {
                                    if (is_changing_settings_mode_enabled) {
                                        RewriteAnswers(answers, current_station, i + delta_tasks, formalised_player_answer);
                                        player_answer.clear();
                                        formalised_player_answer.clear();
                                    }
                                    else {
                                        if (player_answer.size() > 0 && is_answer_true[i + delta_tasks - 1] == 2) {

                                            bool is_task_skipped = false;
                                            for (int j = 0; j < skipped_tasks_iterators[current_station - 1].size(); j++) {
                                                if (skipped_tasks_iterators[current_station - 1][j] + 1 == i + delta_tasks)
                                                    is_task_skipped = true;
                                            }

                                            //Forbiding to change station
                                            is_changing_station_enabled = false;

                                            if (is_task_skipped);
                                            //Player answer and answer data comparison
                                            else {
                                                if (!answers[current_station - 1][i + delta_tasks - 1].std::wstring::compare(formalised_player_answer)) {
                                                    WinTry(is_answer_true[i + delta_tasks - 1], win_try_sound, player_answer, formalised_player_answer);
                                                }
                                                else LoseTry(is_answer_true[i + delta_tasks - 1], lose_try_sound,
                                                    total_lose_sound, task_lives_container[i + delta_tasks - 1] == 1,
                                                    task_lives_container[i + delta_tasks - 1]);
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                    }
                }

                if (event.type == Event::TextEntered && is_starting_menu_open == false) {
                    int event_text_code = event.text.unicode;
                    if (event_text_code >= ' ' && event_text_code <= '~' || event_text_code >= '¡') {
                        if (event_text_code != Keyboard::BackSpace && player_answer.size() <= 32) {
                            player_answer.push_back(static_cast<wchar_t>(event_text_code));
                            if (event_text_code != ' ')
                                formalised_player_answer.push_back(ToUpper(event_text_code));
                        }
                        if (player_answer.size() == 16)
                            player_answer.push_back('\n');
                    }
                }
                else if (event.type == Event::TextEntered && event.text.unicode >= ' ' && team_name.size() < 13)
                    team_name += static_cast<wchar_t>(event.text.unicode);

                if (event.type == Event::KeyPressed && is_starting_menu_open == false) {
                    switch (event.key.code) {
                    case Keyboard::BackSpace:
                        if (player_answer.size() > 0) {
                            if (player_answer[player_answer.size() - 1] == L'\n' ||
                                player_answer[player_answer.size() - 1] == L' ')
                                player_answer.pop_back();
                            else {
                                player_answer.pop_back();
                                formalised_player_answer.pop_back();
                            }
                        }
                        break;
                    case Keyboard::Left:
                        if (is_changing_station_enabled) {
                            if (current_station == 1)
                                current_station = amount_of_stations;
                            else current_station--;
                        }
                        break;
                    case Keyboard::Right:
                        if (is_changing_station_enabled) {
                            if (current_station == amount_of_stations)
                                current_station = 1;
                            else current_station++;
                        }
                        break;
                    default:
                        break;
                    }
                }
                else if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::BackSpace) {
                        if (team_name.size() > 0)
                            team_name.pop_back();
                    }
                    else if (event.text.unicode == Keyboard::Enter && team_name.size() > 0) {
                        if (team_name.size() > 8)
                            team_name_text.setString(team_name.substr(0, 8) + L"...");
                        else team_name_text.setString(team_name);
                        is_starting_menu_open = false;
                    }
                }
            }

            if (event.type == Event::LostFocus) {
                is_focus_lost = true;
                is_focus_lost_when_leave_advisor_enabled = false;
            }
            if (event.type == Event::GainedFocus)
                is_focus_lost = false;

            if (is_focus_lost_when_leave_advisor_enabled == false && is_app_leave_advisor_enabled && is_focus_lost) {
                escape_app_times.push_back(CurrentTimeToString().time);
                is_focus_lost_when_leave_advisor_enabled = true;
            }
        }

        if (is_focus_lost == false && is_starting_menu_open == false) {
            window.clear();
            window.draw(background.sprite);

            window.draw(restart_button.sprite);
            window.draw(screen_lock_button.sprite);
            window.draw(app_leave_advisor_button.sprite);
            window.draw(upload_station_results_button.sprite);
            window.draw(settings_button.sprite);
            if (is_changing_settings_mode_enabled) {
                window.draw(load_settngs_button.sprite);
                window.draw(upload_settings_button.sprite);
            }

            window.draw(player_answer_input_1);
            window.draw(player_answer_input_2);
            window.draw(player_answer_input_3);

            if (clock.getElapsedTime().asSeconds() <= 1)
                input_text.setString(player_answer);
            else if (clock.getElapsedTime().asSeconds() >= 2)
                clock.restart();
            else input_text.setString(player_answer + L"|");
            window.draw(input_text);

            window.draw(room_choose);

            if (is_changing_settings_mode_enabled && current_station == amount_of_stations) {
                element_increment.sprite.setPosition(station_addiction_position);
                element_decrement.sprite.setPosition(station_delete_position);
                window.draw(element_increment.sprite);
                window.draw(element_decrement.sprite);
            }
            else {
                station_changing_arrow.sprite.setRotation(0.f);
                station_changing_arrow.sprite.setPosition(
                    station_changing_arrow_right_position);
                window.draw(station_changing_arrow.sprite);
            }
            if (is_changing_settings_mode_enabled && delta_tasks + 5 >= amount_of_tasks) {
                element_increment.sprite.setPosition(tasks_addiction_position);
                element_decrement.sprite.setPosition(tasks_delete_position);
                window.draw(element_increment.sprite);
                window.draw(element_decrement.sprite);
            }
            if (amount_of_tasks > 5 && (is_changing_settings_mode_enabled == false || delta_tasks + 5 < amount_of_tasks)) {
                station_changing_arrow.sprite.setRotation(0.f);
                station_changing_arrow.sprite.setPosition(
                    taskline_changing_arrow_right_position);
                window.draw(station_changing_arrow.sprite);
            }
            station_changing_arrow.sprite.setRotation(180.f);
            station_changing_arrow.sprite.setPosition(
                station_changing_arrow_left_position);
            window.draw(station_changing_arrow.sprite);
            if (amount_of_tasks > 5) {
                station_changing_arrow.sprite.setPosition(
                    taskline_changing_arrow_left_position);
                window.draw(station_changing_arrow.sprite);
            }

            if (current_station < 10) {
                station_text.setScale(Vector2f(1, 1));
                station_text.setPosition(station_text_position_under_10);
            }
            else {
                station_text.setScale(Vector2f(0.8, 0.8));
                station_text.setPosition(station_text_position_grater_10);
            }
            station_text.setString(std::to_string(current_station));
            window.draw(station_text);

            for (int index = 1; amount_of_tasks >= 5 && index <= 5 || amount_of_tasks < 5 && index <= amount_of_tasks; index++) {
                //Show buttom to confirm answer
                answer_confirm_button.setPosition(120 * index - 15,
                    (window.getSize().y + player_answer_input_1.getRadius() * 2.0 + player_answer_input_1.getPosition().y) / 2.0 - answer_confirm_button.getRadius());
                int is_task_skipped = false;
                for (int i = 0; i < skipped_tasks_iterators[current_station - 1].size(); i++) {
                    if (skipped_tasks_iterators[current_station - 1][i] + 1 == index + delta_tasks)
                        is_task_skipped = true;
                }
                if (is_task_skipped)
                    answer_confirm_button.setFillColor(Color(158, 158, 158, 100));
                else {
                    if (is_answer_true[index + delta_tasks - 1] == 2) {
                        answer_confirm_button.setFillColor(Color(158, 158, 158));
                        if (is_changing_settings_mode_enabled == false) {
                            //Show heart
                            heart.sprite.setPosition(answer_confirm_button.getPosition().x + answer_confirm_button.getRadius(),
                                answer_confirm_button.getPosition().y + answer_confirm_button.getRadius());
                            //Show remained lives
                            remained_lives_text.setString(std::to_string(task_lives_container[index + delta_tasks - 1]));
                            if (task_lives_container[index + delta_tasks - 1] > 9)
                                remained_lives_text.setPosition(heart.sprite.getPosition().x - task_number_text.getCharacterSize() / 8.0,
                                    heart.sprite.getPosition().y);
                            else remained_lives_text.setPosition(heart.sprite.getPosition());
                        }
                    }
                    else if (is_answer_true[index + delta_tasks - 1] == 1)
                        answer_confirm_button.setFillColor(Color(102, 255, 51));
                    else answer_confirm_button.setFillColor(Color(255, 51, 51));
                }
                window.draw(answer_confirm_button);
                if (is_task_skipped == false && is_answer_true[index + delta_tasks - 1] == 2 && is_changing_settings_mode_enabled == false) {
                    window.draw(heart.sprite);
                    window.draw(remained_lives_text);
                }
                else if (is_changing_settings_mode_enabled) {
                    rewrite_answer.sprite.setPosition(answer_confirm_button.getPosition().x + answer_confirm_button.getRadius(),
                        answer_confirm_button.getPosition().y + answer_confirm_button.getRadius());
                    window.draw(rewrite_answer.sprite);
                }

                //Show task number
                if (index + delta_tasks > 99)
                    task_number_text.setPosition(answer_confirm_button.getPosition().x + task_number_text.getCharacterSize() / 1.3 - task_number_text.getCharacterSize() / 1.9,
                        answer_confirm_button.getPosition().y - task_number_text.getCharacterSize() - 10);
                else if (index + delta_tasks > 9)
                    task_number_text.setPosition(answer_confirm_button.getPosition().x + task_number_text.getCharacterSize() / 1.3 - task_number_text.getCharacterSize() / 3.5,
                        answer_confirm_button.getPosition().y - task_number_text.getCharacterSize() - 10);
                else
                    task_number_text.setPosition(answer_confirm_button.getPosition().x + task_number_text.getCharacterSize() / 1.3,
                        answer_confirm_button.getPosition().y - task_number_text.getCharacterSize() - 10);
                task_number_text.setString(std::to_string(index + delta_tasks));
                window.draw(task_number_text);
            }

            window.draw(team_name_show_zone_edge);
            window.draw(team_name_show_zone);
            team_name_text.setPosition(3, window.getSize().y - 53);
            window.draw(team_name_text);
        }

        else if (is_starting_menu_open) {
            window.clear();
            window.draw(background.sprite);

            window.draw(team_name_input_1);
            window.draw(team_name_input_2);
            window.draw(team_name_input_3);
            window.draw(start_button_1);
            window.draw(start_button_2);
            window.draw(start_button_3);
            window.draw(start_button_text);

            team_name_text.setPosition(team_name_input_3.getPosition());
            if (start_menu_clock.getElapsedTime().asSeconds() <= 1)
                team_name_text.setString(team_name);
            else if (start_menu_clock.getElapsedTime().asSeconds() >= 2)
                start_menu_clock.restart();
            else team_name_text.setString(team_name + L"|");
            window.draw(team_name_text);
        }
        window.display();
    }

    return 0;
}