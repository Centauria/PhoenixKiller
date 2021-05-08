#include "argparse/argparse.hpp"
#include "termcolor/termcolor.hpp"
#include "util.h"
#include "gpu_pid.h"

#include <cstdlib>
#include <filesystem>
#include <regex>
#include <csignal>

using namespace std;

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("PhoenixKiller", "1.0");
    program.add_description("Find out hidden Phoenix Miner 5.5c on the computer.\n"
                            "WARNING: This program only detects the location by default!\n"
                            "Use --cleanup option for automatic cleanup. (require sudo)\n"
                            "And DO NOT FORGET to cleanup your /var/spool/cron/crontabs/root");
    program.add_argument("--with-md5-check")
            .default_value(false)
            .implicit_value(true)
            .help("Use md5 check when searching.\n"
                  "More accuracy, but slower");
    program.add_argument("--cleanup")
            .default_value(false)
            .implicit_value(true)
            .help("Cleanup mining programs automatically.");
    try {
        program.parse_args(argc, argv);
    }
    catch (const runtime_error &err) {
        cout << termcolor::red << err.what() << endl;
        cout << termcolor::reset << program;
        exit(EXIT_SUCCESS);
    }

    // Find running mining program
    auto gpu_pid_name = get_pid_name();
    for (const auto &[pid, name] : gpu_pid_name) {
        if (name == "N/A") {
            cout << termcolor::green << "Found running mining program, PID=" << pid;
            if (program.get<bool>("--cleanup")) {
                kill(pid, SIGKILL);
                cout << termcolor::red << ", Killed." << termcolor::reset;
            }
            cout << endl;
        }
    }

    vector<string> del_file_list;

    // Find booter script
    auto booter_bins = vector<string>{
            "h32", "h64", "run32", "run64", "run646"
    };
    auto booter_bins_map = map<string, string>{
            {"h32",    "0d01bd11d1d3e7676613aacb109de55f"},
            {"h64",    "c644c04bce21dacdeb1e6c14c081e359"},
            {"run32",  "b1459b6b52c11097d818607bc8eef42a"},
            {"run64",  "348b243a40a02931eb48b68d68b7e596"},
            {"run646", "a136c168a2d108c1baa58852350c492c"}
    };
    auto result = locate(".cpan");
    pprint_vector("Possible \".cpan\" path:", result);
    for (const auto &r : result) {
        vector<string> file_list;
        // ls: .cache .empty .pip .version
        auto cache_dir = filesystem::path(r) / ".cache";
        if (not filesystem::is_directory(cache_dir)) {
            continue;
        }
        for (const auto &entry : filesystem::directory_iterator(cache_dir)) {
            file_list.push_back(entry.path().filename());
        }
        if (contains(file_list, booter_bins)) {
            if (program.get<bool>("--with-md5-check")) {
                if (all_of(
                        booter_bins.begin(),
                        booter_bins.end(),
                        [&cache_dir, &booter_bins_map](const string &entry) {
                            return md5_file(cache_dir / entry) == booter_bins_map[entry];
                        })) {
                    del_file_list.push_back(r);
                    prompt(termcolor::green, "Found booter script in: ", r);
                }
            } else {
                del_file_list.push_back(r);
                prompt(termcolor::green, "Found booter script in: ", r);
            }
        }
    }

    // Find mining program
    result = locate("epools.txt");
    vector<string> parent;
    parent.reserve(result.size());
    for (const auto &r : result) {
        parent.push_back(filesystem::path(r).parent_path().u8string());
    }
    pprint_vector("Possible mining program directory:", parent);
    for (const auto &r : parent) {
        for (const auto &entry : filesystem::directory_iterator(r)) {
            auto filename = entry.path().filename();
            if (filename == "python") {
                bool is_mining_program = false;
                if (filesystem::file_size(entry.path()) == 9373180) {
                    if (program.get<bool>("--with-md5-check")) {
                        if (md5_file(entry.path()) == "637818c725a395bae86175d5bacae72d") {
                            is_mining_program = true;
                            prompt(termcolor::green, "Found mining program: ", entry.path());
                        }
                    } else {
                        is_mining_program = true;
                        prompt(termcolor::green, "Found mining program: ", entry.path());
                    }
                }
                if (is_mining_program) {
                    del_file_list.push_back(entry.path());
                    break;
                }
            }
        }
        auto log_regex = regex(R"(log(\d{8})_\d{6}\.txt)");
        for (const auto &entry : filesystem::directory_iterator(r)) {
            auto filename = entry.path().filename();
            if (regex_match(filename.u8string(), log_regex)) {
                prompt(termcolor::green, "Found log file: ", entry.path());
                del_file_list.push_back(entry.path());
            } else if (filename == "config.txt") {
                prompt(termcolor::green, "Found config.txt: ", entry.path());
                del_file_list.push_back(entry.path());
            } else if (filename == "epools.txt") {
                prompt(termcolor::green, "Found epools.txt: ", entry.path());
                del_file_list.push_back(entry.path());
            }
        }
    }
    if (program.get<bool>("--cleanup")) {
        error_code ec;
        for (const auto &f : del_file_list) {
            cout << termcolor::red << "Delete " << termcolor::reset << f;
            if (filesystem::is_regular_file(f) or filesystem::is_symlink(f)) {
                filesystem::remove(f, ec);
            } else if (filesystem::is_directory(f)) {
                filesystem::remove_all(f, ec);
            }
            cout << ", return code=" << ec.value() << endl;
        }
        for (const auto &r:parent) {
            if (filesystem::is_empty(r)) {
                cout << termcolor::red << "Delete " << termcolor::reset << r;
                filesystem::remove_all(r, ec);
                cout << ", return code=" << ec.value() << endl;
            }
        }
    } else {
        pprint_vector("Files to be deleted:", del_file_list);
    }
    return 0;
}
