# Magnetic Sensor Based Control of a Non-Invasive Forearm Prosthesis

## Overview
This project investigates the use of magnetic field sensors for non-invasive control of a forearm prosthesis.

Instead of traditional EMG electrodes, muscle contractions mechanically move a small permanent magnet placed under or near the skin. The magnetic field variations are measured by a 3-axis magnetometer and translated into control signals for motors driving the prosthetic hand.

The goal is to provide a low-cost, simple, and comfortable alternative to EMG-based control systems.

---

## Research Objective
The main objective of this work is:

> To evaluate whether magnetic field sensing can be used as a reliable and non-invasive method for real-time prosthetic control.

Key assumptions:
- muscle contraction → magnet displacement
- magnet displacement → magnetic field change
- magnetic field change → motor command

---

## Motivation
Conventional prosthetic control methods (especially EMG) suffer from:
- high cost
- complex signal processing
- sensitivity to electrode placement
- skin irritation and discomfort
- noise and instability

Magnetic sensing offers:
- simpler hardware
- low cost sensors
- stable signals
- no skin contact issues
- easier calibration

---

## System Architecture
System pipeline:

Muscle movement  
→ magnet displacement  
→ magnetometer measurement  
→ filtering & calibration  
→ control algorithm  
→ motor driver  
→ prosthetic motion

---

## Hardware
- STM32 microcontroller
- 3-axis magnetometer (MAG3110 or similar)
- DC/servo motors
- Motor drivers
- 3D printed prosthetic hand
- Permanent magnet
- Power supply

---

## Software Features
- I2C magnetometer driver
- real-time magnetic field acquisition
- offset calibration
- moving average filtering
- heading/orientation estimation
- UART logging
- motor control algorithms


---

## Calibration
The system performs:
- hard-iron offset compensation
- moving average filtering
- noise reduction

Future work may include:
- soft-iron calibration
- Kalman filtering
- sensor fusion with IMU

---

## Thesis Information
Master’s Thesis:

**"Investigation of Magnetic Field Sensors for Non-Invasive Control of a Forearm Prosthesis"**

This repository contains both experimental firmware and research code used during development.

---

## Author
Tomasz Fuszara

Electronic and Telecommunication: Embedded Systems

---

## License
MIT / Academic use

