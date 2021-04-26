# Barometer

* Board STM32F401CCU6 with USB-C
* Display OLED SSD1306 32/64
* Sensor BME280/BMP280/BMP180

## MCU setup

* Enable internal HSI 16 MHz
* Setup timer TIM3 for Internal Clock, Prescaler 15999, and generate global interrupt
* Switch I2C1 and I2C2 to Fast Mode

Open Properties for project at `C/C++ Build` - `Settings` and check `Use float with printf from newlib-nano`

## OLED setup

`D:\ST\Projects\workspace\oledpressure401\Core\Inc\OLED.h`

Choose display height: 32 or 64

```c++
#define OLED_HEIGHT         32
```

## Connecting

| Device | Port  | SCL | SDA | Power |
|--------|-------|-----|-----|-------|
| OLED   | i2c_1 | B6  | B7  | +5v   |
| BMP280 | i2c_2 | B10 | B3  | +3.3v |

## Programming MCU

```
set prog=D:\ST\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe
set fw=D:\ST\Projects\workspace\oledpressure401\Release\oledpressure401.elf

%prog% -c port=SWD -d %fw% -rst -run
```
