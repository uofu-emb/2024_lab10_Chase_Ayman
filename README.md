## Current Workflow Status

![example workflow](https://github.com/uofu-emb/2024_lab10_Chase_Ayman/actions/workflows/main.yml/badge.svg)

# Hello FreeRTOS Example Code - GPIO Blink Task Scenarios

## **Note**
Our method involves declaring all 5 scenarios as individual functions in the `hello_freertos.c` file. You can comment out whichever functions you don't want to run/flash in the `main()`.

---

## **Recorded Values from Activities**

- **Limiting Resistor:** 1k ohm  
- **LED Type:** Clear Green  

---

## **Scenario 1**
- **Applied Voltage:** 5V
- **Pulled Amperage (OFF State):** 19.9 mA
- **Pulled Amperage (ON State):** 20.5 mA

---

## **Scenario 2**
- **Applied Voltage:** 5V
- **Pulled Amperage (OFF State):** 19.98 mA
- **Pulled Amperage (ON State):** 20.45 mA

---

## **Scenario 3**
- **Applied Voltage:** 5V
- **Pulled Amperage (OFF State):** 20.13 mA
- **Pulled Amperage (ON State):** 20.33 mA

---

## **Scenario 4**
- **Applied Voltage:** 5V
- **Pulled Amperage (OFF State):** 20.1 mA
- **Pulled Amperage (ON State):** 20.63 mA

---

## **Scenario 5**
- **Applied Voltage:** 5V
- **Pulled Amperage (OFF State):** 20.34 mA
- **Pulled Amperage (ON State):** 20.41 mA

---

## Lab 10 Power
Learning Objectives
Read and apply datasheets
Read electronic schematics
Use benchtop power and measurement equipment
Describe the sources of power consumption.
Enable low power modes.
Working with open-source library code.

# Activity 1
There are several ways to measure power consumption - in this lab we will be using a benchtop power supply and multimeter. When measuring voltage, you measure the voltage across to points in the circuit in parallel. When measuring current, you measure the current through the multimeter in series. You should be familiar with the basics of using a multimeter from the prelab readings.

Throughout this lab, we'll go through the following procedure for different scenarios.

Turn off the power supply.
Disconnect the multimeter power and ground from the board.
Connect USB.
Load the code for the scenario.
Disconnect your USB
Connect the multimeter power and ground back to the board
Turn on the power supply.
Record your measurments.

# Activity 2
Measure the power consumption of the following scenarios. For each scenario, create a C file with the complete setup and main function.

To eliminate any bias in the measurements, use an external LED with a known current value limiting resistor on a GPIO pin. Don't use the built in LED, which requires the wireless module to be enabled. If you want a serial connection, you need to use the UART pins rather than USB and access it with a serial converter.

1) Blink an LED, using sleep_ms to delay iterations.
2) Blink an LED, using FreeRTOS with a thread.
3) Run a busy loop to get 100% CPU utilization
4) Place #pragma GCC optimize ("O0") at the top of the file to disable compiler optimizations.
5) Run a tight loop like the one below. Multiplication typically requires large amounts of silicon, and adders with a full carry chain causes lots of activity. I don't know if these will be the worst instructions for energy usage, but this should be a decent place to start.
