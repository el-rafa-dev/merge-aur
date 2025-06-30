#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <filesystem>
#include <limits>
#include <vector>
#include <algorithm>

#include <curl/curl.h>

#include "json/json.hpp"

#define APP_MAJOR_VERSION 1
#define APP_MINOR_VERSION 0
#define APP_AUTHOR " Copyright (C) Rafael - el-rafa-dev (https://github.com/el-rafa-dev)"
#define APP_LIBS_USED "JSON by nlohmann (https://github.com/nlohmann/json)"

#define AUR_CLONE_BASE_URL "git clone https://aur.archlinux.org/"
#define AUR_SEARCH_API "https://aur.archlinux.org/rpc.php?v=5&type=search&search_by=name&arg="

namespace fs = std::filesystem;
using json = nlohmann::json;

class CurlRequester
{
public:
    CurlRequester()
    {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    ~CurlRequester()
    {
        curl_global_cleanup();
    }

    std::string getUrlContent(const std::string &url)
    {
        CURL *curl_handle;
        CURLcode res;
        std::string response_buffer;

        curl_handle = curl_easy_init();

        if (curl_handle)
        {
            curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_buffer);
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);

            res = curl_easy_perform(curl_handle);

            if (res != CURLE_OK)
            {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
                response_buffer.clear();
            }

            curl_easy_cleanup(curl_handle);
        }
        else
        {
            std::cerr << "Failed to initialize CURL easy handle." << std::endl;
        }
        return response_buffer;
    }

private:
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        size_t realsize = size * nmemb;
        std::string *buffer = static_cast<std::string *>(userp);
        buffer->append(static_cast<char *>(contents), realsize);
        return realsize;
    }
};

bool isInteger(const std::string &str)
{
    if (str.empty()) return false;
    for (char c : str) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

int search(const std::string &name, int limit)
{
    CurlRequester requester;
    std::string api = AUR_SEARCH_API + name;

    std::cout << "Searching for '" << name << "'...\n";

    std::string json_content = requester.getUrlContent(api);

    if (json_content.empty())
    {
        std::cerr << "Could not get JSON content from URL. Exiting." << std::endl;
        return 1;
    }

    try
    {
        json parsed_json = json::parse(json_content);

        if (parsed_json.contains("resultcount") && parsed_json["resultcount"].is_number())
        {
            int count = parsed_json["resultcount"].get<int>();
            std::cout << "Total results found: " << count << std::endl;

            if (count > 0 && parsed_json.contains("results") && parsed_json["results"].is_array())
            {
                std::cout << "\n--- Package info ---\n";
                int displayed_count = 0;
                for (const auto &package : parsed_json["results"])
                {
                    if (limit > 0 && displayed_count >= limit) {
                        break;
                    }

                    if (package.contains("Name") && package.contains("Version") &&
                        package["Name"].is_string() && package["Version"].is_string())
                    {
                        std::cout << "  Name: " << package["Name"].get<std::string>()
                                  << "\n  Version: " << package["Version"].get<std::string>() << std::endl;
                    }
                    if (package.contains("Description") && package["Description"].is_string())
                    {
                        std::cout << "  Description: " << package["Description"].get<std::string>() << std::endl;
                    }
                    if (package.contains("URL") && package["URL"].is_string())
                    {
                        std::cout << "  URL: " << package["URL"].get<std::string>() << std::endl;
                    }
                    std::cout << "--------------------------------\n";
                    displayed_count++;
                }
                 if (limit > 0 && count > limit) {
                    std::cout << "Displayed " << displayed_count << " out of " << count << " total results.\n";
                }
            }
            else
            {
                std::cout << "No results found for '" << name << "'.\n";
            }
        }
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return 1;
    }
    catch (const json::exception &e)
    {
        std::cerr << "Error processing JSON (nlohmann/json exception): " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int install(const std::string &name)
{
    fs::path temp_dir_path = fs::temp_directory_path() / name;

    if (fs::exists(temp_dir_path) && fs::is_directory(temp_dir_path))
    {
        std::cout << "Temporary directory '" << temp_dir_path.string() << "' already exists. Removing it...\n";
        try {
            fs::remove_all(temp_dir_path);
            std::cout << "Temporary directory removed successfully.\n";
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error removing temporary directory: " << e.what() << ". Please remove it manually.\n";
            return 1;
        }
    }

    std::string git_clone_cmd = AUR_CLONE_BASE_URL + name + ".git " + temp_dir_path.string();
    std::cout << "Executing command: " << git_clone_cmd << "...\n";
    int result_clone = std::system(git_clone_cmd.c_str());

    if (result_clone != 0)
    {
        std::cerr << "git clone executed with errors! Exit code: " << result_clone << std::endl;
        return result_clone;
    }

    std::string makepkg_cmd = "cd " + temp_dir_path.string() + " && makepkg -si --noconfirm";
    std::cout << "Command executed successfully! Now calling makepkg...\n";
    int result_makepkg = std::system(makepkg_cmd.c_str());

    if (result_makepkg != 0)
    {
        std::cerr << "Makepkg executed with errors! Exit code: " << result_makepkg << ". Try with another package (type -search <name> for more packages).\n";
        try {
            fs::remove_all(temp_dir_path);
            std::cout << "Temporary directory cleaned after makepkg error.\n";
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Could not clean the directory '" << temp_dir_path.string() << "' after makepkg error: " << e.what() << ". Please remove it manually.\n";
        }
        return result_makepkg;
    }

    std::cout << "Makepkg executed successfully! Package installed! Now cleaning the " << name << " directory...\n";
    try {
        fs::remove_all(temp_dir_path);
        std::cout << "Package installed and temporary files cleaned successfully.\n";
        return 0;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Could not clean the directory '" << temp_dir_path.string() << "': " << e.what() << ". Please remove it manually.\n";
        return 1;
    }
}

int help()
{
    std::cout << "[Help list]: () for optional and <> for required arguments\n";
    std::cout << "\t-install <package name> : install a package from the AUR by typing the name\n";
    std::cout << "\t-search <package name> (limit of output, e.g.: 1 for 1 output) : search in AUR the package with the specified name\n";
    std::cout << "\t-version : display the version info of the merge\n";
    std::cout << "\t-help : display this help list\n";
    return 0;
}

int main(int argc, char *args[])
{
    if (argc < 2)
    {
        std::cerr << "No argument specified! Try merge -help to see a list of options!\n";
        return 1;
    }

    std::vector<std::string> arguments;
    for (int i = 1; i < argc; ++i) {
        arguments.push_back(args[i]);
    }

    std::string command = arguments[0];

    if (command == "-search")
    {
        if (arguments.size() < 2)
        {
            std::cerr << "Missing package name for -search. Try merge -help.\n";
            return 1;
        }

        std::string packageName = "";
        int limit = 0;

        for (size_t i = 1; i < arguments.size(); ++i) {
            if (isInteger(arguments[i])) {
                if (limit == 0) {
                    limit = std::stoi(arguments[i]);
                } else {
                    std::cerr << "Multiple numerical arguments found for -search. Expected one limit or none.\n";
                    return 1;
                }
            } else {
                if (packageName.empty()) {
                    packageName = arguments[i];
                } else {
                    packageName += " " + arguments[i];
                }
            }
        }

        if (packageName.empty()) {
            std::cerr << "Missing package name for -search. Try merge -help.\n";
            return 1;
        }

        return search(packageName, limit);
    }
    else if (command == "-install")
    {
        if (arguments.size() < 2)
        {
            std::cerr << "Missing package name for -install. Try merge -help.\n";
            return 1;
        }
        std::string packageName = arguments[1];
        return install(packageName);
    }
    else if (command == "-help")
    {
        return help();
    }
    else if (command == "-version")
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t current_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm *local_tm = std::localtime(&current_time_t);

        int current_year = (local_tm) ? (local_tm->tm_year + 1900) : 0;

        printf("Version: %i.%i\nAuthor:%s - %i\nLibs used: %s\n", APP_MAJOR_VERSION, APP_MINOR_VERSION, APP_AUTHOR, current_year, APP_LIBS_USED);
    }
    else
    {
        std::cerr << "Invalid argument! Try merge -help to see a list of options!\n";
        return 1;
    }

    return 0;
}