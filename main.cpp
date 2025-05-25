#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <sstream>   // Para std::istringstream (en wordWrap)
#include <stdexcept>
#include <cstdlib>   // Para std::getenv, system
#include <cstdio>    // Para FILE, popen, pclose, fgets (en exec)
#include <array>     // Para std::array (en exec)
#include <memory>    // Para std::unique_ptr (en exec y getAiGeneratedQuote)
// #include <functional> // No estrictamente necesaria con el enfoque actual de httplib::Result

// Define CPPHTTPLIB_OPENSSL_SUPPORT ANTES de incluir httplib.h
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

// --- COLECCIÓN DE DISEÑOS ASCII ---
const std::vector<std::string> asciiArts = {
    R"(
        ⣯⠕⢮⠛⠉⠉⠉⠙⢫⠖⢭⣿
        ⡇⠐⠁⠀⡰⠀⠰⡀⠀⠑⠀⣿
        ⡇⠀⡇⢨⠉⣦⡎⢩⠀⡇⠀⣿
        ⡇⢀⣿⣌⠻⠯⠽⢋⣼⣇⠀⣿
        ⠁⣼⣿⢊⢫⣪⣪⢏⢺⣿⡄⢹
        ⡆⢹⣧⡋⣦⣮⣶⡌⢎⣿⠁⣾
        ⣵⣿⣿⣷⢩⢩⢩⢱⣿⣿⣷⣽
        ⣿⣿⣿⣿⣶⣾⣶⣾⣿⣿⣿⣿
    ---------------------------------
    __QUOTE_PLACEHOLDER__
    ---------------------------------
    )",
    R"(
                   _ |\_
                   \` ..\
              __,.-" =__Y=
            ."        )
      _    /   ,    \/\_
     ((____|    )_-\ \_-`
     `-----'`-----` `--`
_ __QUOTE_PLACEHOLDER__ _
    )",
    R"(
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠋⠉⠁⠀⠀⠀⠀⠉⠉⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢳⣤⡈⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠀⠈⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠒⠢⡀⠀⠀⠀⠀⠀⠀⠐⠒⢄⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⡔⠉⠉⢢⠈⠀⠀⠀⢠⠚⠉⠉⠓⡄⠑⠀⠀⠀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⢸⢠⣾⢿⣄⡇⠀⠀⠀⡇⣰⡾⢿⣷⡘⡄⠀⠀⠀⠀⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⣿⡿⠁⠀⢻⡃⢀⡀⢠⣷⡏⠀⠀⢹⣷⡇⠀⠀⠀⠀⢼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⢸⡇⠀⠀⣼⠓⢉⣈⡚⠻⢇⠀⠀⢨⣿⠀⠀⠀⠀⠀⢺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⢿⡤⢈⣴⣞⣽⣷⣬⣿⣤⣅⡐⢾⠋⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⢠⠊⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡦⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠐⠰⣯⣿⣿⣿⣿⣿⣿⣿⠟⣡⣿⣿⠅⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢰⡀⢠⣉⣉⣉⣉⣭⣤⣾⡿⣋⣥⣦⠀⠀⠀⠘⣦⡀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⣸⣿⣦⡉⠟⠻⠛⠻⢉⣴⣾⣿⣿⣿⣧⠀⠀⠀⠘⠃⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠘⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠋⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⡿⢱⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⢀⡀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀⣰⠀⠀⢿⠿⠟⢛⣉⣩⣭⡥⢁⣴⣶⣶⣶⣶⣤⣤⣤⣬⣉⣙⠛⠀⠀⠈⠛⣆⠀⠀⠀⠀⠙⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁⠀⡼⠁⠀⣠⣴⣾⣿⣿⣿⣿⣿⢁⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠈⢆⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⡼⠁⠀⣼⣿⣿⣿⣿⣿⣿⣿⠇⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠘⣆⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⡴⠃⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⠀⠀⠸⡄⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁⣸⠁⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⡇⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⡇⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀⡇⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⢡⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⣿⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀⣰⡇⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⡿⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⢀⡿⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠈⢱⡀⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠀⡼⠃⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⡆⢀⣀⣀⠀⢧⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡗⣀⡼⠛⠛⠻⠶⣄⡀⠀⢰⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⡟⣰⣿⣿⣿⣷⡄⠉⠻⢿⣿⣿⣿⣿⣿⣿⣇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⣴⣦⣅⠋⠀⠀⠀⠀⠀⢠⡇⢀⠾⢿⣿⣿⣿⣿⣿⣿
    ⣿⡿⢟⣀⣩⣩⢉⡰⣵⣿⣿⣿⣿⣿⣆⠀⠀⠙⠿⣿⣿⣿⣿⣿⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⢽⣿⡿⠀⠀⠀⠀⠀⠀⠈⢠⣾⣿⠆⣿⣿⣿⣿⣿
    ⣿⠱⣾⣿⣯⣿⣾⣷⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⢘⣿⣷⣂⡀⠀⠀⠀⣀⣴⣾⣿⣿⡆⣿⣿⣿⣿⣿
    ⣿⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠀⠀⠀⠀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⢸⣿⣿⣳⡽⣎⡷⣯⢿⣿⣿⣿⣿⣷⣌⠻⣿⣿⣿
    ⣿⣌⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⣿⣿⣿⣿⣿⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣬⡛⢿
    ⣿⣇⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡤⢤⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠠⣛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⡨
    ⡟⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡌⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠁⠀⠀⠀⢐⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⡿⣟⠿⡩⠆⣱
    ⢰⡹⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡽⣀⠀⠉⠛⠛⠛⠻⠟⠛⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⢌⡿⣿⣿⣿⣿⣿⣿⣿⡿⢿⡹⢎⠱⠃⣌⣠⣴⣿⣿
    ⣤⡉⠧⢛⡜⢯⣛⢿⡻⢿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡱⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⠞⡿⣿⢿⡟⡿⣏⠷⡩⢃⠐⣠⣶⣾⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣶⣤⣬⣆⣉⠣⠙⠲⢩⠛⣽⢻⡟⡿⡝⠶⡉⠄⢠⣶⣾⣶⣿⣿⣿⣿⣷⣾⣶⣶⣶⣤⣤⣤⣀⡀⠀⠨⠑⡍⢎⡙⠲⢉⠂⢁⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣶⣬⣀⡡⠘⠁⠉⠐⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⣀⠀⠀⠀⠀⠁⣠⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿

    < __QUOTE_PLACEHOLDER__ >
    )",
    R"(
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠿⠛⠛⠛⠛⠻⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⣉⠄⣢⣴⣟⣫⣤⣾⢿⣿⣷⡶⢦⣬⣉⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣡⡴⠋⢑⣬⣴⣿⣿⡻⣿⣿⣶⣝⠻⣿⣷⣾⣿⢿⣦⡉⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣡⡾⠫⣠⣾⣿⣿⣿⣿⣿⣷⢹⣿⣿⣿⣷⡙⢿⣿⣿⣧⡐⡝⣦⡙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢃⣼⣿⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣇⣿⣿⣿⣿⣿⡌⠙⢿⣿⣿⡐⣿⣷⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⣹⢻⡟⡘⣿⠇⣿⣿⣿⣿⣿⣿⣿⡏⣿⠻⣿⣿⣿⣿⡌⢷⡉⢙⠀⠈⠀⡶⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠐⠀⠾⢠⣷⣜⢸⣿⣿⡇⣿⣿⣿⣿⡇⢻⣧⢻⣿⣿⣿⣷⡀⡁⠀⠁⡁⣦⣄⠁⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⢀⠑⣸⡦⣈⡻⠇⢨⢹⣿⡧⣿⣿⣿⣿⡇⣘⣿⡜⣿⣿⣿⢿⢇⠀⠀⣧⢱⡹⣷⡌⠂⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⡟⢠⡄⡈⠻⣿⣿⣿⣿⣿⣿⠁⠌⢀⢇⡿⠀⣿⣿⡇⣦⣾⣿⣃⣿⣿⣿⣿⡇⠸⣟⢃⢛⣋⡴⠂⠎⡀⠘⣿⡌⣷⡘⣿⡄⠀⠘⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⡿⢀⠟⢜⣼⠀⣼⣿⣿⣿⣿⠇⠌⠀⣸⠸⡇⠀⡇⠟⣃⣿⣿⣿⢸⣿⢿⣶⣭⠁⡁⠹⡠⠌⢉⣬⣉⣀⡃⠀⠸⣷⢸⣧⡹⣷⡀⢧⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣷⡈⠲⠛⢉⠀⢻⣿⣿⣿⡟⠀⣼⠀⣿⢰⡇⠀⠀⣿⡇⣿⣿⡟⢨⡿⣸⣿⡟⢠⣿⣄⠁⠀⣿⣿⣿⣿⢰⠀⠀⢿⡆⢿⣷⡙⣷⡈⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡾⠛⠻⣾⣿⣿⣿⠃⢰⡏⠠⣿⠸⣧⠀⠀⠁⠁⠹⡿⠁⣼⠃⡟⠉⠀⠒⠈⠉⠁⠀⠛⣿⣿⣿⢸⢐⠲⠘⣿⠈⣻⣷⡌⢿⡄⢾⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣅⠀⢀⣿⣿⣿⣿⢀⡿⠁⠘⣿⢨⢻⡀⠀⢦⠂⠀⠠⣤⣥⣤⣦⣶⡆⠀⠀⠙⣿⡇⢀⣿⣿⣿⡏⢐⠳⡄⢿⡇⡟⡏⢻⣆⢈⠀⠙⠛⣛⠉⢸⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⠸⠁⠀⡇⡟⠘⡘⣧⠀⢸⣇⠀⡀⢹⣿⣿⣿⣿⣧⠐⣸⠀⣽⠇⡏⣿⣿⡿⠇⢘⠵⠃⢸⡇⠃⠇⠈⡜⡄⠻⣶⣦⣤⣶⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠌⠛⠋⠀⠰⠀⠁⡇⠀⢧⠘⠧⡀⠿⢦⠡⣾⣿⣿⣿⣿⡿⢓⡿⠶⡟⠘⢰⣿⣿⢱⠀⠀⠀⠀⣼⠇⢰⠀⠀⢱⡀⢧⢹⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⠟⠃⠀⠀⠀⠃⠀⠘⡀⠀⠀⠐⢄⣠⣿⣿⣿⣿⣿⣯⡐⣜⢂⡠⠂⣿⣿⡧⢸⠀⠀⠀⢠⠋⠀⠈⠀⠀⠘⠀⠈⢂⣿⣿⣿⣿⣿⣿⣿
⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠃⠆⠀⠸⠇⠀⠀⠀⠄⠥⠀⢀⠀⠀⠙⠛⠿⢶⣼⣿⡿⠿⠛⢉⣤⢰⣿⡿⠃⡇⠀⠀⠀⠀⠀⠀⠀⡴⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿
⠀⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣭⣭⣭⡒⡄⠂⠀⠀⠀⠀⠀⠀⡀⣀⠀⢰⣿⢃⣾⠿⠁⠀⠀⠀⠀⢀⠀⠀⠠⢄⠀⠀⢀⣜⣠⣿⣿⣿⣿⣿⣿⣿⣿
⣇⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡘⡀⠐⢂⣀⣤⣴⠊⡠⠴⠀⠉⠡⠛⠁⠀⠀⢠⡶⢀⡴⣄⡐⢶⡄⢠⡀⢺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⡄⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⢡⠀⠹⢿⣿⢃⢰⣶⣥⣒⡶⠟⣓⣤⣤⡾⢋⠔⣩⣾⣿⣿⠖⣠⣾⠇⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣷⡈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠂⠀⠀⠁⣌⢦⠙⢛⣣⣵⣾⣿⠿⢋⣐⣁⣨⣭⣭⣤⣤⣤⣤⣤⣬⣀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣧⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡈⠄⠀⠀⣫⡕⣬⣓⡲⣶⠖⠂⡄⠛⠉⠙⣿⣿⣿⡿⠿⠛⠋⠁⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣆⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⠁⢰⡙⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠐⠀⠀⣿⣿⠸⣿⠟⣱⣾⡆⢱⢠⢰⠈⠉⣀⣤⢠⣤⣤⠔⣠⣶⡀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⡄⢻⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠈⣷⡈⢛⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠁⠈⢿⠿⠿⠈⢺⣿⣿⣷⠈⡌⢸⠀⣿⣿⠇⣾⡟⢁⠠⠤⠄⠠⢀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⡈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡐⡄⠰⢊⣀⣀⡛⠻⣿⡿⠀⡇⠆⠀⠿⠋⠀⠋⠀⠤⣴⣶⣶⢶⣤⠀⠘⢿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣧⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢱⠀⠚⠉⣩⡶⠀⢀⠀⠀⢀⠀⠠⣀⣀⠀⠀⠀⢐⣄⠀⠀⠀⠈⢂⠀⢨⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣇⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⢧⠀⠖⣉⠤⠴⢂⣀⠈⡓⠦⠄⠀⠀⠀⠐⠤⣌⠛⠓⢄⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿
⣿⠀⠤⠄⠀⢀⣀⣠⣨⣉⣉⣉⣉⣛⣛⡛⣛⢛⡛⠛⠛⠛⠛⠻⠿⠿⠿⠿⠿⠿⠿⠿⠜⡆⠈⠉⠀⠀⠙⠋⠠⠤⠐⠛⠀⠀⠀⠀⠀⠈⠳⠀⠀⠀⠀⠀⠀⠀⠛⠛⠛⠛⠛⠛⠻
⣿⣶⣦⣤⣤⣤⣤⣤⣄⣀⣀⣀⣈⣉⣉⡉⠉⠉⠉⠉⠛⠛⠛⠛⠛⠚⠓⠒⠒⠶⠖⠲⠦⠰⠶⠰⠂⠉⠉⠉⠛⠛⠓⠛⠛⠁⠀⣉⣁⣀⣀⣀⣀⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣼
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣶⣶⣶⣶⣤⣤⣤⣤⣤⣤⣴⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    --------------------------------------
    __QUOTE_PLACEHOLDER__
    --------------------------------------
    )"
};

//aqui van los prompts

// --- DEFINICIÓN GLOBAL Y CONSTANTE DE CATEGORÍAS DE PROMPTS ---
const std::vector<std::string> PROMPT_CATEGORIES = {
    // === MOTIVACIONALES Y GENERALES ===
    "Dame una frase motivacional muy corta, inspiradora y original para empezar el día. Idealmente, no más de 15 palabras.",
    "Un pensamiento breve y positivo para afrontar cualquier desafío. Máximo 20 palabras.",
    "Una reflexión concisa sobre la importancia del aprendizaje continuo. Menos de 20 palabras.",
    "Genera una pepita de sabiduría en una sola frase corta sobre la perseverancia.",
    "Una frase corta que inspire creatividad y pensamiento original.",
    "Un consejo de vida breve, profundo y fácil de recordar.",
    "Una frase corta sobre la importancia de dar el primer paso.",

    // === LINUX, SISTEMAS OPERATIVOS Y SOFTWARE LIBRE ===
    "Escribe una cita corta, ingeniosa o un hecho curioso breve sobre el kernel de Linux. No más de 20 palabras.",
    "Una frase memorable sobre la filosofía del software libre o el movimiento open source. Límite 15 palabras.",
    "Un dato curioso sobre la historia de Unix o sus derivados (como BSD o macOS). Breve y conciso.",
    "Describe en una frase corta y poética la belleza de un sistema operativo bien diseñado, estable o eficiente.",
    "Un pensamiento breve sobre la línea de comandos (CLI) y su poder, elegancia o eficiencia.",
    "Una frase corta que capture la esencia de la comunidad Linux o el espíritu colaborativo.",
    "¿Qué significa 'GNU' en GNU/Linux? Responde de forma curiosa, breve y memorable.",
    "Un dato curioso sobre la historia de una distribución de Linux menos conocida (ej. Slackware, Arch, Gentoo). Muy breve.",
    "Frase breve sobre qué hace especial a Arch Linux.",
    "Cita breve sobre la filosofía de Gentoo Linux.",
    "Comparación humorística breve entre distribuciones Linux como si fueran personajes de una saga.",

    // === DEBIAN ESPECÍFICO ===
    "Una cita inspiradora sobre Debian GNU/Linux, enfatizando su estabilidad o comunidad. Corta y al grano.",
    "¿Por qué Debian es llamado el 'sistema operativo universal'? Explica brevemente en una frase.",
    "Frase corta sobre la importancia de las versiones estables de Debian (Stable, Testing, Unstable).",
    "Anécdota breve (real o ficticia) sobre el proceso de lanzamiento de una versión de Debian.",
    "Un dato curioso sobre el origen del nombre 'Debian' o su mascota. Menos de 20 palabras.",
    "Cita inspiradora de un desarrollador (real o ficticio) de Debian.",

    // === PROGRAMACIÓN Y DESARROLLO DE SOFTWARE ===
    "Proporciona un pensamiento breve y profundo sobre el arte de la programación. Máximo 20 palabras.",
    "Un haiku sobre código limpio y elegante (formato 5-7-5 sílabas).",
    "Un chiste corto y original de programadores (apto para todos los públicos).",
    "Una frase corta y filosófica sobre la naturaleza de los 'bugs' en el software.",
    "Un consejo breve pero útil para depurar código de manera efectiva. ¡Sorpréndeme!",
    "Una reflexión sobre la importancia de la elección de un buen algoritmo o estructura de datos. Corta e impactante.",
    "Una cita célebre (o inventada) de un programador ficticio sobre su lenguaje de programación favorito. Corta.",
    "¿Cuál es la diferencia entre un buen programador y uno excelente? Responde en una frase corta, aguda y original.",
    "Una metáfora breve y clara para explicar la recursividad en programación.",
    "Un consejo sobre cómo nombrar variables o funciones de forma efectiva y legible. Muy corto.",
    "Una frase corta sobre la alegría (o frustración) de resolver un problema de programación complejo.",
    "Un pensamiento sobre la 'deuda técnica' en el desarrollo de software. Breve.",
    "Describe la 'navaja de Ockham' aplicada al diseño de software en una frase.",
    "Haiku (5-7-5 sílabas) sobre el proceso de depurar código.",

    // === INGENIERÍA INFORMÁTICA Y CONCEPTOS TÉCNICOS ===
    "Explica un concepto complejo de redes de computadoras (como TCP/IP, DNS, o el modelo OSI) en una frase muy simple y memorable.",
    "Una frase corta que destaque la importancia de la arquitectura de computadoras o el diseño de sistemas robustos.",
    "Un pensamiento breve y original sobre el futuro de la inteligencia artificial y su integración en la vida diaria. Máximo 20 palabras.",
    "Una reflexión concisa sobre la computación cuántica y su potencial disruptivo o un desafío clave. Corta.",
    "Una frase corta sobre la ley de Moore y su relevancia (o irrelevancia) actual.",
    "Define 'virtualización' o 'contenedores' (Docker, Kubernetes) en una frase simple y clara.",

    // === CIBERSEGURIDAD Y PRIVACIDAD ===
    "Genera una frase corta, impactante y memorable sobre la ciberseguridad. Límite 15 palabras.",
    "Un consejo breve y práctico para mejorar la seguridad digital personal en el día a día. Original y útil.",
    "Una reflexión corta sobre el balance entre conveniencia y seguridad en la tecnología moderna.",
    "Una frase corta sobre la importancia del cifrado en la era digital.",

    // === HISTORIA DE LA INFORMÁTICA, INTERNET Y VIDEOJUEGOS ===
    "Un dato curioso y breve sobre la historia de la informática o un pionero/pionera (Grace Hopper, Alan Turing, etc.). Máximo 25 palabras.",
    "Una anécdota ficticia (pero plausible y entretenida) muy corta sobre los primeros días de la programación o el hardware. Con humor si es posible.",
    "Nombra una figura histórica de la informática (ej. Ada Lovelace, Charles Babbage) y una frase corta que podría haber dicho sobre el futuro de la computación.",
    "Un hecho poco conocido sobre el desarrollo de ARPANET, los inicios de Internet, o el primer email. Breve y sorprendente.",
    "¿Cuál fue el primer 'bug' informático documentado? Pista: fue literal. Responde brevemente y con ingenio.",
    "Anécdota curiosa sobre el desarrollo de un clásico de los videojuegos (ej. Pac-Man, Tetris, Doom). Máximo 25 palabras.",
    "Frase breve sobre la evolución de los gráficos en videojuegos desde sus inicios hasta hoy.",
    "Cita inspiradora de un desarrollador famoso de videojuegos (ej. Miyamoto, Carmack, Kojima). Corta.",
    "Describe un problema técnico curioso o un 'glitch' famoso durante el desarrollo de un juego conocido. Brevemente.",
    "Dato curioso sobre los primeros días de ARPANET en menos de 25 palabras.",
    "Anécdota breve sobre el desarrollo del primer navegador web (ej. Mosaic, Netscape).",
    "Frase corta sobre cómo era internet en los años 90 comparado con hoy.",
    "Hecho poco conocido sobre el origen del protocolo TCP/IP.",
    "Anécdota sobre el desarrollo inicial de Ubuntu (ej. el nombre en clave, Warty Warthog).",


    // === CIENCIA FICCIÓN, STAR WARS Y CULTURA GEEK/HACKER ===
    "Una cita corta de ciencia ficción (de un libro, película o serie) que inspire innovación tecnológica. Máximo 20 palabras.",
    "¿Qué consejo daría Yoda (Star Wars) sobre la depuración de código o la resolución de problemas informáticos? Frase corta y sabia.",
    "Una frase corta que combine Star Trek (TOS, TNG, etc.) y desarrollo de software, con un toque geek o filosófico.",
    "Cita breve del universo de 'Dune' aplicada metafóricamente a la gestión de grandes sistemas informáticos o datos.",
    "Analogía corta y original entre el viaje en el tiempo (ciencia ficción) y el control de versiones (Git).",
    "Genera una frase corta y humorística que podría ser un meme de programadores o sysadmins. Apto para la terminal.",
    "Una referencia sutil a un meme popular de la cultura de internet, adaptada como una frase corta y sabia para un informático.",
    "Una frase al estilo de 'Keep Calm and Carry On' pero para informáticos (ej. 'Keep Calm and Compile On'). Original.",
    "Si un Dalek (Doctor Who) intentara explicar un error de segmentación, ¿qué diría en una frase corta y exterminadora?",
    "Un chiste corto sobre programación con una referencia sutil a Star Wars o El Señor de los Anillos.",
    "Metáfora breve comparando la recursividad en programación con un concepto de una película de ciencia ficción (ej. Inception).",
    "Frase corta que capture el espíritu hacker original (curiosidad, exploración, superación de límites).",
    "Cita breve de Richard Stallman sobre el software libre con un toque filosófico o provocador.",
    "Analogía entre la ética hacker (exploración y conocimiento) y el código Jedi o la filosofía de alguna orden ficticia.",

    // === FUTURISMO TECNOLÓGICO ===
    "Predicción breve, creativa y un poco inquietante sobre el futuro de la IA, al estilo de una novela de ciencia ficción.",
    "¿Cómo se imaginaban los años 2020 en las películas de ciencia ficción de los 80s versus la realidad actual? Una breve comparación humorística o irónica.",
    "Frase corta sobre la computación cuántica usando una analogía de un concepto de ciencia ficción para explicar su rareza o poder.",

    // === PRODUCTIVIDAD Y TRABAJO CON ORDENADORES ===
    "Dame un consejo conciso y original sobre productividad para alguien que trabaja intensamente con ordenadores. Menos de 20 palabras.",
    "Una frase corta para recordar la importancia de los descansos (como la técnica Pomodoro) o la ergonomía al trabajar frente a la pantalla.",
    "Un tip breve y efectivo para organizar archivos digitales o gestionar proyectos personales de forma eficiente.",
    "Una frase sobre cómo mantener el enfoque en un mundo lleno de distracciones digitales. Corta.",

    // === FILOSOFÍA Y TECNOLOGÍA ===
    "Una frase corta y reflexiva sobre el impacto de la tecnología en la sociedad y las relaciones humanas.",
    "Un pensamiento breve sobre la ética en la inteligencia artificial y la responsabilidad de los desarrolladores. Original.",
    "Una pregunta filosófica corta relacionada con la tecnología o el futuro digital para reflexionar al inicio del día.",
    "Una frase sobre la relación entre la creatividad humana y las herramientas tecnológicas que la potencian o la limitan."
};


// --- FUNCIÓN WORD WRAP ---
std::vector<std::string> wordWrap(const std::string& text, int maxWidth) {
    std::vector<std::string> lines;
    if (maxWidth <= 0) {
        lines.push_back(text);
        return lines;
    }
    std::istringstream textStream(text);
    std::string currentLine;
    std::string word;
    while (textStream >> word) {
        if (currentLine.empty()) {
            currentLine = word;
        } else if (currentLine.length() + 1 + word.length() <= static_cast<size_t>(maxWidth)) {
            currentLine += " " + word;
        } else {
            lines.push_back(currentLine);
            currentLine = word;
        }
    }
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    return lines;
}

// --- FUNCIÓN PARA OBTENER VARIABLE DE ENTORNO ---
std::string getEnvVar(const std::string& key, const std::string& defaultValue = "") {
    char* val = std::getenv(key.c_str());
    return (val == nullptr) ? defaultValue : std::string(val);
}

// --- FUNCIÓN PARA EJECUTAR COMANDO DEL SISTEMA Y CAPTURAR SALIDA ---
std::string exec(const char* cmd) {
    std::array<char, 256> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        std::cerr << "Error: popen() falló al ejecutar: " << cmd << std::endl;
        return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// --- FUNCIÓN PARA LIMPIAR RESPUESTA DE LA IA ---
std::string cleanAiResponse(std::string text) {
    size_t firstNonSpace = text.find_first_not_of(" \t\n\r\f\v");
    if (firstNonSpace == std::string::npos) return "";

    // Bucle para quitar múltiples etiquetas <tag>...</tag> al principio
    bool tagFoundAndRemoved;
    do {
        tagFoundAndRemoved = false;
        firstNonSpace = text.find_first_not_of(" \t\n\r\f\v");
        if (firstNonSpace != std::string::npos && text[firstNonSpace] == '<') {
            size_t tagClosePos = text.find('>', firstNonSpace);
            if (tagClosePos != std::string::npos) {
                std::string tagNameContent = text.substr(firstNonSpace + 1, tagClosePos - firstNonSpace - 1);
                size_t spaceInTag = tagNameContent.find(' ');
                std::string tagName = (spaceInTag != std::string::npos) ? tagNameContent.substr(0, spaceInTag) : tagNameContent;

                if (!tagName.empty() && tagName.find('/') == std::string::npos) {
                    std::string closingTag = "</" + tagName + ">";
                    size_t closingTagPos = text.find(closingTag, tagClosePos);
                    if (closingTagPos != std::string::npos) {
                        text.erase(firstNonSpace, closingTagPos + closingTag.length() - firstNonSpace);
                        tagFoundAndRemoved = true;
                    } else {
                        // Si no hay etiqueta de cierre clara, y es una etiqueta simple sin contenido útil aparente
                        // o una etiqueta de sistema como <|im_start|>, la borramos.
                        // Esta parte es heurística. Si la etiqueta tiene contenido real y no cierre,
                        // podríamos querer conservarlo. Por ahora, borramos la etiqueta simple.
                        text.erase(firstNonSpace, tagClosePos - firstNonSpace + 1);
                        tagFoundAndRemoved = true;
                    }
                }
            }
        }
    } while (tagFoundAndRemoved);

    firstNonSpace = text.find_first_not_of(" \t\n\r\f\v");
    if (firstNonSpace == std::string::npos) return "";
    text.erase(0, firstNonSpace);

    size_t lastNonSpace = text.find_last_not_of(" \t\n\r\f\v");
    if (lastNonSpace == std::string::npos) return "";
    text.erase(lastNonSpace + 1);

    return text;
}

// --- FUNCIÓN PARA OBTENER CITA DE LA IA (CON GENERADORES ALEATORIOS ESTÁTICOS PARA PROMPTS) ---
std::string getAiGeneratedQuote(bool isLocalOllama = false, const std::string& ollamaModelFromCaller = "qwen3-4b-abliterated-nothink:latest") {
    // Generadores aleatorios estáticos para la selección de prompts
    static std::random_device rd_prompt_device;
    static std::mt19937 gen_prompt(rd_prompt_device());
    static std::uniform_int_distribution<size_t> distrib_prompt(
        0, PROMPT_CATEGORIES.empty() ? 0 : PROMPT_CATEGORIES.size() - 1);

    std::string user_prompt;
    if (!PROMPT_CATEGORIES.empty()) {
        user_prompt = PROMPT_CATEGORIES[distrib_prompt(gen_prompt)];
    } else {
        user_prompt = "Dame una frase corta interesante.";
    }

    std::string endpointHost;
    std::string endpointPath;
    httplib::Headers headers;
    json payload;

    payload["messages"] = json::array({
        {{"role", "user"}, {"content", user_prompt}}
    });

    if (isLocalOllama) {
        endpointHost = "localhost";
        endpointPath = "/api/chat";
        payload["model"] = ollamaModelFromCaller;
        headers.emplace("Content-Type", "application/json");
        payload["stream"] = false;
    } else {
        std::string apiKey = getEnvVar("OPENROUTER_API_KEY");
        if (apiKey.empty()) {
            return "Error_API_KEY";
        }
        endpointHost = "openrouter.ai";
        endpointPath = "/api/v1/chat/completions";
        headers = {
            {"Authorization", "Bearer " + apiKey},
            {"Content-Type", "application/json"}
        };
        payload["model"] = "qwen/qwq-32b:free"; // Modelo para OpenRouter
    }

    httplib::Result response_result;
    if (isLocalOllama) {
        httplib::Client client(endpointHost, 11434);
        client.set_connection_timeout(10, 0);
        client.set_read_timeout(60, 0);
        client.set_write_timeout(10, 0);
        std::string payloadStr = payload.dump();
        response_result = client.Post(endpointPath.c_str(), headers, payloadStr, "application/json");
    } else {
        httplib::SSLClient client(endpointHost.c_str());
        client.set_connection_timeout(10, 0);
        client.set_read_timeout(30, 0);
        client.set_write_timeout(10, 0);
        std::string payloadStr = payload.dump();
        response_result = client.Post(endpointPath.c_str(), headers, payloadStr, "application/json");
    }

    if (!response_result) {
        auto err_code = response_result.error();
        // No imprimimos a std::cerr aquí, solo devolvemos el código de error
        return isLocalOllama ? "Error_OLLAMA_CONNECTION" : "Error_OPENROUTER_CONNECTION";
    }

    if (response_result->status == 200) {
        try {
            json jsonResponse = json::parse(response_result->body);
            std::string content;
            if (jsonResponse.contains("choices") && jsonResponse["choices"].is_array() && !jsonResponse["choices"].empty()) {
                const auto& firstChoice = jsonResponse["choices"][0];
                if (firstChoice.contains("message") && firstChoice["message"].is_object()) {
                    const auto& messageObj = firstChoice["message"];
                    if (messageObj.contains("content") && messageObj["content"].is_string()) {
                        content = messageObj["content"].get<std::string>();
                    }
                }
            } else if (jsonResponse.contains("message") && jsonResponse["message"].is_object()) {
                 const auto& messageObj = jsonResponse["message"];
                 if (messageObj.contains("content") && messageObj["content"].is_string()){
                    content = messageObj["content"].get<std::string>();
                 }
            } else if (jsonResponse.contains("response") && jsonResponse["response"].is_string()){
                content = jsonResponse["response"].get<std::string>();
            }

            if (!content.empty()) {
                return content;
            }
            // No imprimimos el JSON aquí para mantener los errores concisos en main
            return "Error_FORMAT";
        } catch (const json::parse_error& e) {
            // No imprimimos el body aquí
            return "Error_PARSE";
        } catch (const std::exception& e) {
            return "Error_UNEXPECTED_PROCESSING";
        }
    } else {
        if (response_result->status == 429) {
            return isLocalOllama ? "Error_OLLAMA_API_Status_429" : "Error_OPENROUTER_API_Status_429";
        }
        // No incluimos el cuerpo del error aquí para ser concisos
        return isLocalOllama ? "Error_OLLAMA_API_Status_Other" : "Error_OPENROUTER_API_Status_Other";
    }
    return "Error_UNHANDLED";
}

// --- FUNCIÓN MAIN (CON LÓGICA DE FALLBACK INVERTIDA Y MENSAJES CONCISOS) ---
int main() {
    std::string outputMessage;
    std::string aiQuote;
    bool quoteFetchedSuccessfully = false;
    std::string dataSource = "Ninguna"; // Para saber de dónde vino la cita o si es fallback

    // 1. INTENTAR CON OLLAMA LOCAL PRIMERO
    aiQuote = getAiGeneratedQuote(true, "qwen3-4b-abliterated-nothink:latest");

    if (aiQuote.rfind("Error_", 0) != 0) {
        quoteFetchedSuccessfully = true;
        dataSource = "Ollama Local";
    } else {
        if (aiQuote == "Error_OLLAMA_CONNECTION") {
            std::cerr << "[Info] Ollama local: No disponible o error de conexión." << std::endl;
        } else {
            std::cerr << "[Info] Ollama local: Falló (" << aiQuote << ")." << std::endl;
        }

        // 2. INTENTAR CON OPENROUTER COMO FALLBACK SI OLLAMA FALLÓ
        std::string openRouterApiKey = getEnvVar("OPENROUTER_API_KEY");
        if (openRouterApiKey.empty()) {
            std::cerr << "[Info] OpenRouter: API Key no configurada. Omitiendo." << std::endl;
        } else {
            aiQuote = getAiGeneratedQuote(false);
            if (aiQuote.rfind("Error_", 0) != 0) {
                quoteFetchedSuccessfully = true;
                dataSource = "OpenRouter";
            } else {
                if (aiQuote == "Error_OPENROUTER_CONNECTION") {
                    std::cerr << "[Info] OpenRouter: Error de conexión." << std::endl;
                } else if (aiQuote == "Error_OPENROUTER_API_Status_429") {
                    std::cerr << "[Info] OpenRouter: Límite de tasa alcanzado." << std::endl;
                } else if (aiQuote == "Error_API_KEY"){ // Ya se verificó arriba, pero por si acaso
                     std::cerr << "[Info] OpenRouter: API Key no configurada o inválida." << std::endl;
                } else {
                    std::cerr << "[Info] OpenRouter: Falló (" << aiQuote << ")." << std::endl;
                }
            }
        }
    }

    if(quoteFetchedSuccessfully){
        // Opcional: mensaje para saber la fuente, normalmente comentado.
        // std::cerr << "[Debug] Cita obtenida de: " << dataSource << std::endl;
    }

    if (quoteFetchedSuccessfully) {
        aiQuote = cleanAiResponse(aiQuote);
        if (aiQuote.empty()) {
            // std::cerr << "[Info] Respuesta de IA vacía tras limpieza. Usando fallback." << std::endl;
            quoteFetchedSuccessfully = false; // Activa el fallback a cowsay
        }
    }

    if (quoteFetchedSuccessfully) {
        if (asciiArts.empty()) {
            outputMessage = "Advertencia: No hay diseños ASCII definidos.\n" + aiQuote;
        } else {
            std::random_device rdArt;
            std::mt19937 genArt(rdArt());
            std::uniform_int_distribution<> distribArt(0, static_cast<int>(asciiArts.size()) - 1);
            std::string selectedArt = asciiArts[distribArt(genArt)];
            int quoteMaxWidth = 40;
            std::vector<std::string> wrappedLines = wordWrap(aiQuote, quoteMaxWidth);
            std::string formattedQuoteBlock;
            for (const auto& line : wrappedLines) { formattedQuoteBlock += line + "\n"; }
            if (!formattedQuoteBlock.empty() && formattedQuoteBlock.back() == '\n') { formattedQuoteBlock.pop_back(); }
            std::string placeholder = "__QUOTE_PLACEHOLDER__";
            size_t placeholderPos = selectedArt.find(placeholder);
            if (placeholderPos != std::string::npos) {
                selectedArt.replace(placeholderPos, placeholder.length(), formattedQuoteBlock);
                outputMessage = selectedArt;
            } else {
                outputMessage = "Advertencia: Placeholder no encontrado en el ASCII art.\n" + selectedArt + "\n--------------------\n" + aiQuote + "\n--------------------";
            }
        }
    } else {
        // --- FALLBACK FINAL A COWSAY O MENSAJE SIMPLE ---
        // std::cerr << "[Info] Usando fallback a cowsay/mensaje simple." << std::endl;
        std::string cowsayOutput;
        std::vector<std::string> fallbackQuotes = {
            "La paciencia es una virtud, especialmente al compilar.",
            "Recuerda guardar tu trabajo frecuentemente.",
            "Que tus bugs sean pocos y tus commits muchos.",
            "¡A programar se ha dicho!",
            "Linux es libertad.",
            "Un 'kernel panic' al día mantiene al aburrimiento en la lejanía... o no.",
            "La mejor documentación es la que se escribe."
        };
        std::random_device rdFallback;
        std::mt19937 genFallback(rdFallback());
        std::uniform_int_distribution<> distribFallback(0, static_cast<int>(fallbackQuotes.size()) - 1);
        std::string selectedFallbackQuote = fallbackQuotes[distribFallback(genFallback)];

        std::string escapedQuote = selectedFallbackQuote;

        std::string cowsayCmd = "cowsay -f tux \"" + escapedQuote + "\"";
        if (system("command -v cowsay > /dev/null 2>&1") == 0) {
             cowsayOutput = exec(cowsayCmd.c_str());
        }
        if (!cowsayOutput.empty()) {
            outputMessage = cowsayOutput;
        } else {
            outputMessage = "------------------------------------\n";
            outputMessage += selectedFallbackQuote + "\n";
            outputMessage += "------------------------------------\n";
            outputMessage += "(Fallback final: IA no disponible y cowsay no encontrado)";
        }
    }

    std::cout << outputMessage << std::endl;
    return 0;
}