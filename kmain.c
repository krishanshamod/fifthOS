    #include "drivers/frame_buffer.h"
    #include "drivers/serial_port.h"
    #include "segmentation/segments.h"

    void kmain()
    {
       segments_install_gdt();
       
       char arr[] = "Welcome to fifthOS";
       fb_write(arr, 20);
       serial_configure(SERIAL_COM1_BASE, Baud_115200);
       serial_write(SERIAL_COM1_BASE, arr, 20);

    }

