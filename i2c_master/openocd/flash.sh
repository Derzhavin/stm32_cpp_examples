openocd -f $1 -c "program ../build/i2c_master verify reset exit"
