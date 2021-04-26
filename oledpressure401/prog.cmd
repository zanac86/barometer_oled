set prog=D:\ST\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe

::set fw=D:\ST\Projects\workspace\test_oled_vb\Release\test_oled_vb.elf
set fw=.\Release\oledpressure401.elf

%prog% -c port=SWD -d %fw% -rst -run
