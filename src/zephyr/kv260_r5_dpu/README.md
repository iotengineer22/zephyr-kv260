# KV260 DPU Hardware Monitor Test

## Overview

This project includes a hardware monitoring test program for the Kria KV260 Vision AI Starter Kit. It directly reads the registers of the DPU, SFM (Softmax), and INTC (Interrupt Controller) IPs to evaluate their operational statuses. The states of these hardware components are indicated using the onboard LEDs and logged to the console for verification.

## Building and Running

To build this application, use the standard Zephyr `west` command with your target board:

`west build -p -b kv260_r5`