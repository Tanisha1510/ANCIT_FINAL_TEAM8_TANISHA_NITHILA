# Non-Volatile Memory (NvM) Management – S32K144

## Project Overview
This project implements a Non-Volatile Memory (NvM) management module for the **NXP S32K144 :ANCIT EV2 BOARD** using S32 Design Studio. It demonstrates deferred and immediate write operations to an external EEPROM through the I²C communication interface.
The module manages memory write requests using a queue and a state machine, allowing write operations to be processed according to the selected write mode. OLED status messages provide visual feedback during testing.

## Exercise 18: Deferred vs Immediate Write
### Problem Statement

Implement `NvM_MainFunction()` as a state machine with the following states:
`IDLE → WRITE_PENDING → WRITE_BUSY → DONE`
The module must queue write requests to different memory blocks to prevent write operations from colliding.
Add a write-mode flag for each memory block to support the following modes:
- **IMMEDIATE:** Blocks the caller until the EEPROM write operation completes.
- **DEFERRED:** Queues the write request and returns immediately. The application polls `NvM_GetBlockStatus()` later to check the write status.

### Key Requirements
1. Implement the NvM state machine.
2. Support per-block IMMEDIATE and DEFERRED write modes.
3. Queue deferred write requests for sequential processing.
4. Prevent overlapping EEPROM write operations.
5. Provide block status information through `NvM_GetBlockStatus()`.
6. Account for the EEPROM's approximately 5 ms write cycle.

## Features
- **Immediate Write:** Processes a write request immediately according to the NvM implementation.
- **Deferred Write:** Queues write requests for processing by the NvM main function.
- **Request Queue:** Manages pending memory write requests.
- **State Management:** Tracks the NvM write-processing states.
- **Block Management:** Supports multiple logical memory blocks mapped to EEPROM addresses.
- **EEPROM Read-Back Verification:** Compares stored data with the original data during testing.
- **OLED Status Display:** Displays test progress and write status.

## Hardware Requirements
- NXP S32K144 microcontroller
- EEPROM
- OLED display
- I²C communication interface

## Software and Tools
- S32 Design Studio 3.5
- Embedded C
- NXP S32K144 SDK
- Git and GitHub

## Memory Configuration

| Parameter | Configuration |
|---|---|
| Microcontroller | NXP S32K144 |
| EEPROM | 24C02 |
| Communication | I²C / LPI2C |
| Logical memory blocks | 3 |
| Data size per block | 8 bytes |
| Request queue capacity | 4 requests |
| EEPROM block addresses | `0x10`, `0x20`, `0x30` |

## NvM Write Modes

### 1. Immediate Write
An immediate write request is processed synchronously by the NvM implementation. The caller waits for the operation to complete before the request returns.

### 2. Deferred Write
A deferred write request is placed in the queue and processed when `NvM_MainFunction()` is executed. This separates request submission from write processing.

## State Management
The NvM module uses a state machine to coordinate write operations:

`IDLE → WRITE_PENDING → WRITE_BUSY → DONE`

The module also maintains block status information to indicate the progress of each write request.

## Test Cases

### Test 1: Deferred Write
- Submit write requests for logical blocks 0 and 1.
- Process queued requests through `NvM_MainFunction()`.
- Read back EEPROM data.
- Compare stored data with the original test data.

### Test 2: Immediate Write
- Submit an immediate write request for logical block 2.
- Check the write completion status.
- Read back EEPROM data.
- Verify the stored data against the original test data.

The test results are available through the debugger expression window and OLED status display.

## Project Structure

```text
FINAL_PROJECT_ANCIT/
├── src/
│   ├── main.c
│   ├── Nvm.c
│   ├── Nvm.h
│   ├── Nvm_Test.c
│   ├── Nvm_Test.h
│   ├── EEPROM.c
│   ├── EEPROM.h
│   ├── OLED_Display.c
│   ├── OLED_Display.h
│   └── Oled_asci.h
├── README.md
└── [S32DS project configuration and SDK files]
```

## Building and Running

1. Import the project into S32 Design Studio 3.5.
2. Confirm that the S32K144 SDK and project configuration are available.
3. Verify the EEPROM and OLED connections.
4. Build the project.
5. Flash the generated firmware to the target microcontroller.
6. Run the firmware and observe the OLED status messages.
7. Inspect `deferredResult`, `immediateResult`, `NvM_GetState()`, and the block status values in the debugger.

A result value of `1` indicates that the corresponding test passed, provided the test function sets the result only after successful verification.


## Author

**Tanisha S**
III yr, Electrical and Electronics Engineering  
Saveetha Engineering College, Chennai


**Nithila**
III yr, Electrical and Electronics Engineering  
Saveetha Engineering College, Chennai

---
## Output
https://youtube.com/shorts/eUfDs2evcyg?feature=share


*This project was developed as part of an automotive embedded systems learning project.*

