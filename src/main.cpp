#include "menu_scene.h"
#include "save_file_hacking.h"

void hackTheGame()  {
    std::ifstream in("save_file_hacking/Template.bin", std::ios::binary);
    if (!in) return;

    savefile profile;
    IdentifyMenu IMenu;

    in.read(reinterpret_cast<char *> (&profile.mask), 1);
    in.read(reinterpret_cast<char *> (&profile.name), sizeof(profile.name));
    in.read(reinterpret_cast<char *> (&profile.password), sizeof(profile.password));

    in.seekg(500, std::ios::cur);
    std::string username;
    std::string tmp = std::bitset<7>(profile.mask).to_string();
    for (int i = 0; i < strlen(profile.name); ++i) {
        std::string ch = std::bitset<7>(profile.name[i]).to_string();
        int result = stoi(tmp, 0, 2) ^ stoi(ch, 0, 2);
        username += (char)(result);
    }
    IMenu.user.username = username;

    std::string password;
    for (int i = 0; i < strlen(profile.password); ++i) {
        std::string ch = std::bitset<7>(profile.password[i]).to_string();
        int result = stoi(tmp, 0, 2) ^ stoi(ch, 0, 2);
        password += (char)(result);
    }
    IMenu.user.password = password;

    auto run = [](int pos) {
        int step = 0;
        for (int i = pos; i % 4 != 0; ++i) {
            ++step;
        }
        return step;
    };

    for (int i = 0; i < 5; ++i) {
        in.seekg(run(in.tellg()), std::ios::cur);
        in.read(reinterpret_cast<char *> (&profile.record[i].date.dd), 4);

        in.seekg(run(in.tellg()), std::ios::cur);
        in.read(reinterpret_cast<char *> (&profile.record[i].date.mm), 4);

        in.seekg(run(in.tellg()), std::ios::cur);
        in.read(reinterpret_cast<char *> (&profile.record[i].date.yy), 4);

        in.seekg(run(in.tellg()), std::ios::cur);
        in.read(reinterpret_cast<char *> (&profile.record[i].points), 4);
        in.seekg(500, std::ios::cur);
    }


    for (int i = 0; i < 5; ++i) {
        in.seekg(run(in.tellg()), std::ios::cur);
        in.read(reinterpret_cast<char *> (&profile.state[i].p), 4);

        in.seekg(run(in.tellg()), std::ios::cur);
        in.read(reinterpret_cast<char *> (&profile.state[i].q), 4);

        in.seekg(run(in.tellg()), std::ios::cur);
        in.read(reinterpret_cast<char *> (&profile.state[i].p_), 4);

        in.seekg(run(in.tellg()), std::ios::cur);
        in.read(reinterpret_cast<char *> (&profile.state[i].q_), 4);

        in.read(reinterpret_cast<char *> (&profile.state[i].board), sizeof(profile.state[i].board));
        in.read(reinterpret_cast<char *> (&profile.state[i].file_background), sizeof(profile.state[i].file_background));
        in.seekg(500, std::ios::cur);
    }

    in.close();

    auto isExisted = [](const std::string &username, const std::string &password) {
        std::fstream in("user_data/database.txt", std::ios::in);

        std::string input_stream;
        Player dummy;
        while (getline(in, input_stream, '/')) {
            dummy.username = input_stream;
            getline(in, input_stream, '/');
            dummy.password = input_stream;
            getline(in, input_stream, '/');
            dummy.gamemode = input_stream;
            getline(in, input_stream, '/');
            dummy.point = stoi(input_stream);
            getline(in, input_stream);
            dummy.lvl = stoi(input_stream);
            if (dummy.username == username && dummy.password == password) return true;
        }

        in.close();
        return false;
    };

    std::fstream out("user_data/database.txt", std::ios::app);

    if (isExisted(IMenu.user.username, IMenu.user.password) == true) return;

    for (int i = 0; i < 5; ++i) {
        out<<"\n";
        out<<IMenu.user.username<<"/"<<IMenu.user.password<<"/"<<"challenge/"<<profile.record[i].points<<"/1";
    }

    out.close();
}

int main() {
    Screen::setUpConsole(18);
    Menu menu;
    hackTheGame();
    menu.menuController(false);
    while (menu.menuController(true) == true);
    Screen::clearConsole();
}
