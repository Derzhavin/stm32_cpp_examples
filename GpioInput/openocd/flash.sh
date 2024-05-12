openocd -f $1 -c "program ../build/GpioInput.elf verify reset exit"
