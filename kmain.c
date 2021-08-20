    #include "drivers/frame_buffer.h"
    #include "drivers/serial_port.h"
    #include "drivers/io.h"
    #include "segmentation/segments.h"
    #include "interrupts/interrupts.h"
    #include "interrupts/keyboard.h"
    #include "interrupts/pic.h"
    

    void kmain()
    {

       //char arr[] = "Welcome to fifthOS";
       //fb_move_cursor(6*80);
       //fb_write(arr, 20);
       //serial_write(arr, 20);
       segments_install_gdt();
       interrupts_install_idt();

    }

