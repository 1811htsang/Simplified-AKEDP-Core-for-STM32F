# AK firmware custom porting - STM32F103C8T6

This repository ports AK firmware from the STM32L151 base project to STM32F103C8T6.
The current workstream focuses on a lean EDP core profile that keeps essential runtime functions for bring-up, debugging, and system validation.

## Current Status

As of the latest refactor, the STM32F103 port is organized around `Drivers/EDP` and core runtime stability.

Included in the active profile:

- AK kernel core (`ak/inc`, `ak/src`): task scheduler, message system, timer, FSM.
- Core app tasks: `task_system`, `task_life`, `task_if`, `task_uart_if`, `task_dbg`.
- System abstraction and platform glue in `Drivers/EDP/sys`.
- Common utilities in `Drivers/EDP/common`.

Removed or excluded from the active profile:

- Shell and command-line update flow.
- RF24/Zigbee integration tasks.
- Modbus-related application path.
- UI/screens modules.

This keeps the firmware smaller and easier to validate during low-level porting.

## Repository Structure

- `stm32f_porting/`: Active STM32F103C8T6 port.
  - `AK Firmware F103 Custom Porting/`: STM32CubeIDE project.
  - `Drivers/EDP/`: AK core + application/system adaptation layer used by this port.
- `stm32l_base/`: Original STM32L151 codebase for reference.
- `user-manual.md`: Notes and usage guidance for local development.
- `README.md`: Project overview and current status.

## Build And Development Notes

- Main project type: STM32CubeIDE managed project (GCC + G++).
- Recommended environment: Linux, or Windows with STM32CubeIDE / WSL toolchain.
- When changing generated build files, keep C and C++ compile paths separated.
- If source paths are renamed (for example, legacy `AK_Imported_Driver` -> `EDP`), recheck `.cproject` and generated Debug metadata.

## Quick Start

1. Open `stm32f_porting/AK Firmware F103 Custom Porting` in STM32CubeIDE.
2. Select `Debug` configuration.
3. Build the project and resolve any generated-file drift first (`.cproject`, `Debug/*`).
4. Flash and validate basic runtime via UART/debug tasks.

## Compatibility Notes

- This port targets STM32F103C8T6 specifically.
- It is not a drop-in package for other STM32 families.
- Use `stm32l_base` as the baseline reference when rebasing or comparing behavior.

## Contributing

Contributions are welcome.
For each change, prefer small focused commits with clear scope, and include build/test evidence when possible.
