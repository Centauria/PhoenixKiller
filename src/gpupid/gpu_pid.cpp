//
// Created by Centauria V. CHEN on 2021/5/8.
//

#include "gpu_pid.h"

#include <iostream>
#include <cstdlib>

extern "C" {
#include "extract_gpu_info.h"
}

using namespace std;

map <size_t, string> get_pid_name() {
    if (not init_gpu_info_extraction()) {
        throw runtime_error("GPU Initialization Failed");
    }

    size_t num_devices;
    struct device_info *dev_infos = nullptr;
    map <size_t, string> result;
    num_devices = initialize_device_info(&dev_infos, UINT_MAX);
    if (num_devices == 0) {
        cout << "No GPU left to monitor." << endl;
        if (dev_infos != nullptr) {
            free(dev_infos);
        }
        return result;
    }
    update_device_infos(num_devices, dev_infos);

    for (unsigned int i = 0; i < num_devices; i++) {
        auto n_compute = dev_infos[i].num_compute_procs;
        auto n_graphic = dev_infos[i].num_graphical_procs;
        for (unsigned int j = 0; j < n_compute; j++) {
            auto pid = dev_infos[i].compute_procs[j].pid;
            auto pname = dev_infos[i].compute_procs[j].process_name;
            if (result.find(pid) == result.end()) {
                result.insert_or_assign(pid, pname);
            }
        }
        for (unsigned int j = 0; j < n_graphic; j++) {
            auto pid = dev_infos[i].graphic_procs[j].pid;
            auto pname = dev_infos[i].graphic_procs[j].process_name;
            if (result.find(pid) == result.end()) {
                result.insert_or_assign(pid, pname);
            }
        }
    }

    clean_device_info(num_devices, dev_infos);
    shutdown_gpu_info_extraction();
    return result;
}
