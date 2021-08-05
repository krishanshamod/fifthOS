    #include "io.h" /* io.h is implement in the section "Moving the cursor" */

    /* The I/O ports */

    /* All the I/O ports are calculated relative to the data port. This is because
     * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
     * order, but they start at different values.
     */

    #ifndef INCLUDE_SERIALPORT_H
    #define INCLUDE_SERIALPORT_H

    #define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

    #define SERIAL_DATA_PORT(base)          (base)
    #define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
    #define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
    #define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
    #define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

    /* The I/O port commands */

    /* SERIAL_LINE_ENABLE_DLAB:
     * Tells the serial port to expect first the highest 8 bits on the data port,
     * then the lowest 8 bits will follow
     */

    #define SERIAL_LINE_ENABLE_DLAB         0x80

    /* Divisor for different baud rates */
    enum BaudRate { Baud_115200 = 1, Baud_57600, Baud_19200, Baud_9600 };

    /** serial_configure_baud_rate:
     *  Sets the speed of the data being sent. The default speed of a serial
     *  port is 115200 bits/s. The argument is a divisor of that number, hence
     *  the resulting speed becomes (115200 / divisor) bits/s.
     *
     *  @param com      The COM port to configure
     *  @param divisor  The divisor
     */

    void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
    {
        outb(SERIAL_LINE_COMMAND_PORT(com),
             SERIAL_LINE_ENABLE_DLAB);
        outb(SERIAL_DATA_PORT(com),
             (divisor >> 8) & 0x00FF);
        outb(SERIAL_DATA_PORT(com),
             divisor & 0x00FF);
    }

    /** serial_configure_line:
     *  Configures the line of the given serial port. The port is set to have a
     *  data length of 8 bits, no parity bits, one stop bit and break control
     *  disabled.
     *
     *  @param com  The serial port to configure
     */

    void serial_configure_line(unsigned short com)
    {
        /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
         * Content: | d | b | prty  | s | dl  |
         * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
         */
        outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
    }

    void serial_configure_fifo_buffer(unsigned short com) {
        /* Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
         * Content: | lvl | bs | r | dma | clt | clr | e |
         * Value:   | 1 1 | 0  | 0 | 0   | 1   | 1   | 1 | = 0xC7
         */
        outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
    }

    void serial_configure_modem(unsigned short com) {
        /* Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
         * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
         * Value:   | 0 | 0 | 0  | 0  | 0   | 0   | 1   | 1 | = 0x03
         */
        outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
    }

    int serial_is_transmit_fifo_empty(unsigned short com) {
        /* 0x20 = 0010 0000 */
        return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
    }

        void serial_write_byte(unsigned short port, char byteData) {
      outb(port, byteData);
    }

    void serial_configure(unsigned short port, unsigned short baudRate) {
      serial_configure_baud_rate(port, baudRate);
      serial_configure_line(port);
      serial_configure_fifo_buffer(port);
      serial_configure_modem(port);
    }

    int serial_write(unsigned short com, char *buf, unsigned int len) {
      unsigned int indexToBuffer = 0;
      while (indexToBuffer < len) {
        if (serial_is_transmit_fifo_empty(com)) {
          serial_write_byte(com, buf[indexToBuffer]);
          indexToBuffer++;
        }
      }
      return 0;
    }

#endif