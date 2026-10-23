# mono

Firmware for a simple, custom-built desk clock powered by the STM8 micro-controller.

## Hardware Specifications

- **MCU:** STM8S103F3

### Pin Configuration

| Component | Pins | Notes |
| :--- | :--- | :--- |
| **TM1637 Display** (4-digit 7-segment) | CLK: `PA1`, DATA: `PA2` | |
| **DS3231 RTC** | SCL: `PB4`, SDA: `PB5` | I2C |
| **LDR** | `PD2` | ADC1 (Auto-brightness) |
| **Buzzer** | `PC7` | |
| **Status LEDs** (Green/Red/Green) | `PD4`, `PD3`, `PA3` | PWM/TIM2 controlled |
| **Buttons** (1 Touch + 3 Push) | `PC3`, `PC4`, `PC5`, `PC6`| |

## Dependencies

To build and flash the firmware, you will need the following tools installed:

- `sdcc` (Small Device C Compiler)
- `make`
- `stm8dce`
- `stm8flash`

## Build & Flash

To compile the firmware, run:

```bash
make
```

*(For verbose output, use `make VERBOSE=yes`)*

To flash the compiled firmware to the MCU, run:

```bash
make flash
```

## License

This project is licensed under the GNU General Public License v2.0 - see the [LICENSE](LICENSE) file for details.
