openocd -f $1 -c "program ../build/usart_hal verify reset exit"
