-# AI Safety Monitor with FPGA + Zephyr RTOS
This repository presents the solution for the [Best of 2025 Competition(FPGA Project)](https://www.hackster.io/contests/best-of-2025-competition).

## Introduction
The main project details, tutorials, and architecture are comprehensively summarized on Hackster.io. Please refer to the project page below:

[AI Safety Monitor with FPGA + Zephyr RTOS](https://www.hackster.io/iotengineer22/ai-safety-monitor-with-fpga-zephyr-rtos-bf21d8)

## Demo Videos
- [Hello World! with Zephyr(RTOS) and KV260_R5](https://youtu.be/9fN434vk-Kk)
- [Memory Access FPGA(PL) with Zephyr(RTOS) and KV260_R5](https://youtu.be/D5QgywujpGk)
- [FPGA(PL) GPIO Control with Zephyr(RTOS) and KV260_R5](https://youtu.be/x0Sz4feHSCw)
- [DPU(Deep Learning Processor Unit) Monitor with Zephyr(RTOS) and KV260_R5](https://youtu.be/owYmlJc4roc)

## Structure
    .
    ├── pcb                         # PCB design files and documentation
    ├── src
    │   ├── fpga                    # FPGA PL models and testing scripts
    │   │   ├── zephyr_kv260_dpu        # DPU configuration and PyTorch tests
    │   │   └── zephyr_kv260_gpio       # FPGA PL GPIO hardware overlay
    │   └── zephyr                  # Zephyr RTOS workspaces for KV260 Cortex-R5
    │       ├── kv260_r5_blinky         # FPGA(PL) GPIO Control test
    │       ├── kv260_r5_dpu            # DPU/SFM/INTC Hardware Monitor test
    │       ├── kv260_r5_hello_world    # Standard Zephyr Hello World
    │       └── kv260_r5_pl             # FPGA PL Memory Access test
    ├── LICENSE
    └── README.md


## License
This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

**Disclaimer regarding FPGA Binaries:**
The pre-compiled FPGA binaries (such as bitstreams, `.xclbin`, etc.) included in this repository contain proprietary IP cores provided by AMD/Xilinx (e.g., DPU, processing system IPs). While the project source code is open-sourced under the Apache 2.0 License, the use of these pre-compiled binaries is subject to the AMD/Xilinx End User License Agreement (EULA) and they are intended solely for use on compatible AMD/Xilinx devices (e.g., Kria KV260).