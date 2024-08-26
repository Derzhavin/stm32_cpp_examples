openocd -f $1 -c "program ../build/spi_master verify reset exit"
