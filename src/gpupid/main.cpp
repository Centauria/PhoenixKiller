//
// Created by Centauria V. CHEN on 2021/5/8.
//

#include <iostream>

extern "C" {
#include "extract_gpu_info.h"
}

using namespace std;

void show_gpu_processes(struct device_info *dev_infos, size_t num_devices) {
    cout << "GPU count: " << num_devices << endl;
    for (unsigned int i = 0; i < num_devices; i++) {
        cout << "[GPU " << i << "]" << endl;
        auto n_compute = dev_infos[i].num_compute_procs;
        auto n_graphic = dev_infos[i].num_graphical_procs;
        cout << "Compute process count: " << n_compute << endl;
        cout << "Graphical process count: " << n_graphic << endl;
        for (unsigned int j = 0; j < n_compute; j++) {
            cout << "PID(Compute): " << dev_infos[i].compute_procs[j].pid << endl;
            cout << "Process name(Compute): " << dev_infos[i].compute_procs[j].process_name << endl;
        }
        for (unsigned int j = 0; j < n_graphic; j++) {
            cout << "PID(Graphics): " << dev_infos[i].graphic_procs[j].pid << endl;
            cout << "Process name(Graphics): " << dev_infos[i].graphic_procs[j].process_name << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    if (not init_gpu_info_extraction()) return EXIT_FAILURE;

    size_t num_devices;
    struct device_info *dev_infos = nullptr;
    num_devices = initialize_device_info(&dev_infos, UINT_MAX);
    if (num_devices == 0) {
        cout << "No GPU left to monitor." << endl;
        if (dev_infos != nullptr) {
            free(dev_infos);
        }
        return EXIT_SUCCESS;
    }
    update_device_infos(num_devices, dev_infos);
    show_gpu_processes(dev_infos, num_devices);

    clean_device_info(num_devices, dev_infos);
    shutdown_gpu_info_extraction();
    return 0;
}