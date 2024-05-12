openocd -f $1 -c "program ../build/HelloWorld.elf verify reset exit"
