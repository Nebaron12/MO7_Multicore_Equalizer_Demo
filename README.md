# Dual-Core Audio Processing System with 7-Band Equalizer

## Overview

This project implements a real-time audio processing system on a PYNQ-Z2 board utilizing both ARM Cortex-A9 cores in a dual-core configuration. The system provides a 7-band parametric equalizer with real-time control through a custom fader panel and OLED display.

### System Architecture

- **UI Core (Core 0)**: Handles user interface, I2C communications, OLED display, fader panel input, and audio codec initialization
- **Audio Core (Core 1)**: Performs real-time audio processing using ARM NEON SIMD instructions for high-performance 7-band equalization
- **Shared Memory**: Inter-core communication for EQ parameters, volume control, and system state synchronization

### Key Features

- **Real-time Audio Processing**: 48 kHz, 24-bit audio with minimal latency
- **7-Band Parametric Equalizer**: Bands A through G with individual gain control
- **NEON-Optimized DSP**: ARM NEON SIMD instructions for efficient filter processing
- **Live Control Interface**: Physical faders and OLED display for real-time parameter adjustment
- **Dual I2C Architecture**: Separate I2C buses to prevent conflicts between codec and UI components

## Hardware Requirements

### Main Board
- **PYNQ-Z2 Development Board** (Zynq-7000 SoC)
- SD card (8GB minimum) for boot files
- Audio input/output connections (3.5mm line in/out)
- USB cable for programming and power

### Fader Panel
- Custom fader panel with 7 analog sliders
- 128x64 OLED display (SSD1309 controller)
- TLA2528 8-channel ADC for fader readings
- Connected to PYNQ-Z2 Arduino pins:
  - **SDA**: Pin SDA (I2C Data)
  - **SCL**: Pin SCL (I2C Clock)
  - **Power**: 3.3V and GND

## System Setup

### 1. Hardware Connections

1. **Fader Panel Connection**:
   - Connect fader panel SDA to PYNQ-Z2 Arduino pin SDA
   - Connect fader panel SCL to PYNQ-Z2 Arduino pin SCL
   - Connect 3.3V power and GND

2. **Audio Connections**:
   - Connect audio source to line input (3.5mm jack)
   - Connect speakers/headphones to line output (3.5mm jack)

### 2. Software Setup

#### Option A: Using Provided Boot Files (Recommended)
1. Insert the provided SD card into the PYNQ-Z2 board
2. Power on the board - the system should boot automatically
3. **Note**: See "Known Issues" section below regarding startup reliability

#### Option B: Development Setup
1. Install AMD Vitis IDE (project origin: 2025.1, but version 2023.2 or later will probably work)
2. Import the project workspace into Vitis
3. Build both UI_Core and Audio_Core projects
4. Generate platform hardware specification
5. Upload over USB via JTAG

### 3. Project Structure

```
SID_Multicore_Demo/
├── Audio_Core/              # Audio processing core
│   ├── src/
│   │   ├── Main.c          # Audio core main loop
│   │   ├── EQ/             # Equalizer implementation
│   │   │   ├── EQ_NEON.c   # NEON-optimized filters
│   │   │   ├── Coeffs.c    # Filter coefficients
│   │   │   └── Coeffs.h    # Coefficient definitions
│   │   └── Codec/          # Audio codec drivers
├── UI_Core/                 # User interface core
│   ├── src/
│   │   ├── Main.c          # UI core main loop
│   │   ├── OLED/           # OLED display drivers
│   │   ├── FaderPanel/     # Fader panel interface
│   │   ├── I2C/            # I2C communication
│   │   ├── Codec/          # Codec configuration
│   │   └── SharedMem/      # Shared memory functions
├── Multicore_Demo_System/   # System-level configuration
│   └── SharedMem/
│       └── SharedMem.h     # Shared memory definitions
└── platform/               # Hardware platform files
```

## Operation

### Startup Sequence
1. UI Core initializes first and sets up:
   - Shared memory structures
   - I2C buses (separate for codec and UI components)
   - Audio codec configuration
   - OLED display
   - Fader panel interface

2. Audio Core waits for UI Core completion, then:
   - Initializes NEON-optimized EQ filters
   - Begins real-time audio processing loop
   - Reads shared memory for EQ parameters

### User Interface
- **7 Faders**: Control gain for each EQ band (A-G)
- **OLED Display**: Shows current EQ settings and system status
- **Real-time Updates**: Changes are applied immediately to audio processing

### EQ Bands
| Band | Frequency Range | Type |
|------|----------------|------|
| A    | Low frequencies | Parametric |
| B    | Low-mid | Parametric |
| C    | Mid-low | Parametric |
| D    | Mid | Parametric |
| E    | Mid-high | Parametric |
| F    | High-mid | Parametric |
| G    | High frequencies | Parametric |

### UART Control Interface

The system supports remote control via UART commands at **256,000 baud** (8-N-1). Connect to the UART console to send commands and receive system feedback.

#### UART Connection Settings
- **Baud Rate**: 256,000 bps
- **Data Bits**: 8
- **Parity**: None
- **Stop Bits**: 1
- **Flow Control**: None

#### Available Commands

| Command | Syntax | Description | Example |
|---------|--------|-------------|---------|
| **Reset** | `#R` | Reset all EQ settings to default values | `#R` |
| **Echo** | `#E` | Display current EQ settings to console | `#E` |
| **Volume** | `#V` | Reset volume to default | `#V` |
| | `#V0xXX` | Set volume (0x00-0xFF = 0.00-2.55f) | `#V0x80` |
| **Gain** | `#G0xXX` | Reset filter XX to default gain | `#G0x02` |
| | `#G0xXX0xYY` | Set filter XX to gain YY (0x00-0xFF = 0.00-5.10f) | `#G0x020x40` |
| **Filter Type** | `#T0xXX` | Set filter type (0=Throughput, 1=Equalizer) | `#T0x01` |
| **Isolate** | `#I0xXX` | Isolate/solo filter XX for testing | `#I0x03` |

#### Command Format Notes
- All commands start with `#`
- No spaces between command elements
- Commands are case-sensitive
- Filter numbers: 0x00-0x06 (filters A-G)
- Comment out "#define CMD_NUMBER_FORMAT" to enable filter and gain indexing using uint8_t fomatting as opposed to 0-9
- Comment out "#define CMD_ECHO_CMD" to disable command echoing
- Note that gain or volume commands will not work with the fader panel connected

#### Example Session
```
#E                  // Echo current settings
#V0x20              // Set volume to ~0.20 (20 * 0.01)
#G0x000x80          // Set filter A (0x00) to gain ~2.56 (128 * 0.02)
#T0x01              // Enable equalizer mode
#R                  // Reset all to defaults
```

## Known Issues

### ⚠️ **Startup Reliability Problem**

**USB Boot**: The system does not start reliably when programmed via USB. It typically requires **3 or more reboots** before audio processing begins working correctly. You may hear a brief audio blip initially, but the OLED display update seems to interfere with audio codec operation, or there's a cache coherency issue I've not been able to fix.

**SD Card Boot**: When booting from the provided BOOT.bin file on SD card, the system boots but **audio processing does not work**, even with multiple SRST (system reset) button presses.

### Root Cause Analysis
The issue appears to be related to:
1. **I2C Bus Conflicts**: Despite having separate I2C buses, there may be timing conflicts between OLED updates and codec communications
2. **Initialization Race Conditions**: The dual-core startup sequence may have timing dependencies that aren't fully resolved
3. **Cache Coherency**: Shared memory synchronization between cores may have edge cases

### Workaround
- When using USB programming, be prepared to reset the system multiple times
- Monitor the serial output for initialization messages
- Audio should work reliably once the system starts properly

### For Developers
If you need to resolve this issue:
1. Examine the I2C implementations in `UI_Core/src/OLED/OLED.c` and `UI_Core/src/Codec/audio_codec.c`
2. Check the inter-core synchronization in `Audio_Core/src/Main.c` (lines 18-24)
3. Investigate cache coherency operations in shared memory access
4. Consider adding additional delays or verification steps in the codec initialization sequence

## Technical Details

### Performance Specifications
- **Audio Sample Rate**: 48 kHz
- **Bit Depth**: 24-bit
- **Processing Latency**: < 1ms (estimated)
- **EQ Bands**: 7 independent parametric filters
- **SIMD Optimization**: ARM NEON instructions for 4x parallel processing

### Memory Architecture
- **Shared Memory Base**: 0x100000
- **Cache Management**: Explicit cache flush/invalidate operations
- **Inter-core Communication**: State or sleep based synchronization

### I2C Bus Configuration
- **I2C0 (XPAR_XIICPS_0_BASEADDR)**: OLED display and fader panel
- **I2C1 (XPAR_XIICPS_1_BASEADDR)**: Audio codec (ADAU1761)
- **Clock Speed**: 400 kHz

## Development Notes

### Building the Project
1. Open AMD Vitis IDE
2. Import the workspace
3. Build the platform first
4. Build UI_Core and Audio_Core applications
5. Generate boot files

### Debugging
- Use UART console output for system messages
- Monitor I2C transactions for communication issues
- Check shared memory values for inter-core communication
- Use Vitis debugger for step-by-step analysis

### Customization
- **EQ Coefficients**: Modify `Audio_Core/src/EQ/Coeffs.c` for different filter characteristics
- **UI Layout**: Update `UI_Core/src/OLED/` for custom display layouts
- **Filter Count**: Adjust `FILTER_COUNT` in `SharedMem.h` (requires code changes throughout)

## Support

For technical questions or issues:
1. Check the serial console output for error messages
2. Verify hardware connections
3. Ensure all I2C devices are properly connected and powered
4. Review the code comments for implementation details

## License
This project is based on work licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

## Author
Dirk Jan Bakels - [GitHub Profile](https://github.com/yourusername)
                - 524334@student.fontys.nl (student email)
