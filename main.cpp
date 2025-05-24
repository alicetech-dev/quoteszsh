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
        std::cerr << "Error: popen() falló al ejecutar: " << cmd << std::endl; // Mensaje de error más corto
        return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// --- FUNCIÓN PARA LIMPIAR RESPUESTA DE LA IA ---
std::string cleanAiResponse(std::string text) {
    // Quitar etiquetas <tag>...</tag> al principio del texto
    // Esto es una heurística y podría necesitar ajustes para casos más complejos.
    size_t firstNonSpace = text.find_first_not_of(" \t\n\r\f\v");
    if (firstNonSpace == std::string::npos) return ""; // String está vacío o solo espacios

    if (text[firstNonSpace] == '<') {
        size_t tagClosePos = text.find('>', firstNonSpace);
        if (tagClosePos != std::string::npos) {
            // Extraer el nombre de la etiqueta (ej. "think" de "<think>")
            std::string tagName = text.substr(firstNonSpace + 1, tagClosePos - firstNonSpace - 1);
            // Quitar espacios y atributos de la etiqueta, si los tuviera (ej. <think attr="val"> -> think)
            size_t spaceInTag = tagName.find(' ');
            if (spaceInTag != std::string::npos) {
                tagName = tagName.substr(0, spaceInTag);
            }

            if (!tagName.empty() && tagName.find('/') == std::string::npos) { // Asegurarse que no sea una etiqueta de cierre
                std::string closingTag = "</" + tagName + ">";
                size_t closingTagPos = text.find(closingTag, tagClosePos);
                if (closingTagPos != std::string::npos) {
                    // Borrar desde el inicio de la etiqueta de apertura hasta el final de la de cierre
                    text.erase(firstNonSpace, closingTagPos + closingTag.length() - firstNonSpace);
                } else {
                    // Si no hay etiqueta de cierre y es una etiqueta simple al principio, la borramos
                    // Esto ayuda con cosas como <|im_start|> que algunos modelos usan
                    text.erase(firstNonSpace, tagClosePos - firstNonSpace + 1);
                }
            }
        }
    }

    // Trim de espacios al principio y al final
    firstNonSpace = text.find_first_not_of(" \t\n\r\f\v");
    if (firstNonSpace == std::string::npos) return "";
    text.erase(0, firstNonSpace);

    size_t lastNonSpace = text.find_last_not_of(" \t\n\r\f\v");
    if (lastNonSpace == std::string::npos) return ""; // Debería ser imposible si el trim de inicio funcionó y no era vacía
    text.erase(lastNonSpace + 1);

    return text;
}


// --- FUNCIÓN PARA OBTENER CITA DE LA IA (MODIFICADA PARA FALLBACK Y SIN ClientBase) ---
std::string getAiGeneratedQuote(bool isLocalOllama = false, const std::string& ollamaModelFromCaller = "qwen3-4b-abliterated-nothink:latest") {
    std::string endpointHost;
    std::string endpointPath;
    httplib::Headers headers;
    json payload;

    std::string sourceApiName = isLocalOllama ? "Ollama (" + ollamaModelFromCaller + ")" : "OpenRouter";
    // std::cerr << "[Debug] Intentando API: " << sourceApiName << std::endl; // Mensaje de depuración opcional

    // En getAiGeneratedQuote()

    std::vector<std::string> promptCategories = {
        // Motivacionales y Generales
        "Dame una frase motivacional muy corta, inspiradora y original para empezar el día. Idealmente, no más de 15 palabras.",
        "Un pensamiento breve y positivo para afrontar cualquier desafío. Máximo 20 palabras.",
        "Una reflexión concisa sobre la importancia del aprendizaje continuo. Menos de 20 palabras.",

        // Linux y Sistemas Operativos
        "Escribe una cita corta, ingeniosa o un hecho curioso breve sobre el kernel de Linux. No más de 20 palabras.",
        "Una frase memorable sobre la filosofía del software libre o el movimiento open source. Límite 15 palabras.",
        "Un dato curioso sobre la historia de Unix o sus derivados. Breve y conciso.",
        "Una cita inspiradora sobre Debian GNU/Linux. Corta y al grano.",
        "Describe en una frase corta y poética la belleza de un sistema operativo bien diseñado.",
        "Un pensamiento breve sobre la línea de comandos (CLI) y su poder.",

        // Programación y Desarrollo de Software
        "Proporciona un pensamiento breve y profundo sobre la programación o el desarrollo de software. Máximo 20 palabras.",
        "Un haiku sobre código limpio y elegante.",
        "Un chiste corto de programadores (apto para todos los públicos).",
        "Una frase corta sobre la naturaleza de los 'bugs' en el software.",
        "Un consejo breve pero útil para depurar código eficientemente.",
        "Una reflexión sobre la importancia de la elección de un buen algoritmo.",
        "Una cita célebre (o inventada que suene célebre) de un programador ficticio sobre su lenguaje favorito. Corta.",
        "¿Cuál es la diferencia entre un buen programador y uno excelente? Responde en una frase corta.",

        // Ingeniería Informática y Conceptos Técnicos
        "Explica un concepto complejo de redes de computadoras en una frase muy simple y memorable.",
        "Una frase corta sobre la importancia de la arquitectura de computadoras.",
        "Un pensamiento breve sobre el futuro de la inteligencia artificial. Máximo 20 palabras.",
        "Una reflexión concisa sobre la computación cuántica y su potencial.",

        // Ciberseguridad y Privacidad
        "Genera una frase corta y memorable sobre la ciberseguridad o la importancia de la privacidad en línea. Límite 15 palabras.",
        "Un consejo breve y práctico para mejorar la seguridad digital personal.",

        // Historia de la Informática y Anécdotas
        "Un dato curioso y breve sobre la historia de la informática o un pionero/pionera importante. Máximo 25 palabras.",
        "Una anécdota ficticia (pero plausible) muy corta sobre los primeros días de la programación. Con un toque de humor si es posible.",
        "Nombra una figura histórica de la informática y una frase corta que podría haber dicho. (Ej: Ada Lovelace)",
        "Un hecho poco conocido sobre el desarrollo de ARPANET o los inicios de Internet. Breve.",

        // Memes y Cultura Geek (con cuidado, la IA puede no entenderlos bien o ser muy literal)
        "Genera una frase corta y humorística que podría ser un meme de programadores. Apto para la terminal.",
        "Una referencia sutil a un meme popular de la cultura de internet, adaptada como una frase corta y sabia.",
        "Imagina que un personaje de Star Trek da un consejo sobre sistemas. Frase corta.",

        // Productividad y Trabajo con Ordenadores
        "Dame un consejo conciso sobre productividad para alguien que trabaja con ordenadores. Menos de 20 palabras.",
        "Una frase corta para recordar la importancia de los descansos al trabajar frente a la pantalla.",

        // Filosofía y Tecnología
        "Una frase corta y reflexiva sobre el impacto de la tecnología en la sociedad.",
        "Un pensamiento breve sobre la ética en la inteligencia artificial."
    };
    std::random_device rdPrompt;
    std::mt19937 genPrompt(rdPrompt());
    std::uniform_int_distribution<> distribPrompt(0, static_cast<int>(promptCategories.size()) - 1);
    std::string user_prompt = promptCategories[distribPrompt(genPrompt)];

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
            return "Error_API_KEY"; // Solo el código de error
        }
        endpointHost = "openrouter.ai";
        endpointPath = "/api/v1/chat/completions";
        headers = {
            {"Authorization", "Bearer " + apiKey},
            {"Content-Type", "application/json"}
        };
        payload["model"] = "qwen/qwq-32b:free";
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
                return content; // Devolver el contenido crudo, se limpiará en main
            }
            // std::cerr << "Respuesta API (formato inesperado) de " << sourceApiName << ":\n" << jsonResponse.dump(2) << std::endl;
            return "Error_FORMAT";
        } catch (const json::parse_error& e) {
            // std::cerr << "Respuesta API (error de parseo) de " << sourceApiName << ":\n" << response_result->body << std::endl;
            return "Error_PARSE";
        } catch (const std::exception& e) {
            return "Error_UNEXPECTED_PROCESSING";
        }
    } else {
        if (response_result->status == 429) {
            return isLocalOllama ? "Error_OLLAMA_API_Status_429" : "Error_OPENROUTER_API_Status_429";
        }
        return isLocalOllama ? "Error_OLLAMA_API_Status_Other" : "Error_OPENROUTER_API_Status_Other";
    }
    return "Error_UNHANDLED";
}

// --- FUNCIÓN MAIN ---
int main() {
    std::string outputMessage;
    std::string aiQuote;
    bool quoteFetchedSuccessfully = false;
    std::string dataSource = "Desconocida"; // Para saber de dónde vino la cita

    // 1. Intentar con OpenRouter
    // std::cerr << "[Debug] Intentando OpenRouter..." << std::endl;
    aiQuote = getAiGeneratedQuote(false);

    if (aiQuote.rfind("Error_", 0) != 0) {
        quoteFetchedSuccessfully = true;
        dataSource = "OpenRouter";
    } else {
        // Mensajes de error concisos para OpenRouter
        if (aiQuote == "Error_API_KEY") {
            std::cerr << "[Info] OpenRouter: API Key no configurada o inválida." << std::endl;
        } else if (aiQuote == "Error_OPENROUTER_CONNECTION") {
            std::cerr << "[Info] OpenRouter: Error de conexión." << std::endl;
        } else if (aiQuote == "Error_OPENROUTER_API_Status_429") {
            std::cerr << "[Info] OpenRouter: Límite de tasa alcanzado." << std::endl;
        } else {
            std::cerr << "[Info] OpenRouter: Falló (" << aiQuote << ")." << std::endl;
        }

        if (aiQuote != "Error_API_KEY") { // No intentar Ollama si el problema es la API Key de OpenRouter
            // 2. Intentar con Ollama Local como fallback
            // std::cerr << "[Info] Intentando fallback a Ollama local..." << std::endl;
            aiQuote = getAiGeneratedQuote(true, "qwen3-4b-abliterated-nothink:latest");

            if (aiQuote.rfind("Error_", 0) != 0) {
                quoteFetchedSuccessfully = true;
                dataSource = "Ollama Local";
            } else {
                // Mensajes de error concisos para Ollama
                if (aiQuote == "Error_OLLAMA_CONNECTION") {
                     std::cerr << "[Info] Ollama local: Error de conexión. (¿Está corriendo Ollama?)" << std::endl;
                } else {
                     std::cerr << "[Info] Ollama local: Falló (" << aiQuote << ")." << std::endl;
                }
            }
        }
    }

    // Mensaje opcional sobre la fuente, solo si la IA tuvo éxito
    if(quoteFetchedSuccessfully){
        // std::cerr << "[Info] Cita obtenida de: " << dataSource << std::endl;
    }


    if (quoteFetchedSuccessfully) {
        aiQuote = cleanAiResponse(aiQuote); // Limpiar la respuesta de la IA
        if (aiQuote.empty()) { // Si después de limpiar quedó vacía
            // std::cerr << "[Info] La respuesta de la IA quedó vacía después de la limpieza. Usando fallback final." << std::endl;
            quoteFetchedSuccessfully = false;
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