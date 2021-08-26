    #include "drivers/frame_buffer.h"
    #include "drivers/serial_port.h"
    #include "drivers/io.h"
    #include "segmentation/segments.h"
    #include "interrupts/interrupts.h"
    #include "interrupts/keyboard.h"
    #include "interrupts/pic.h"
    #include "multiboot.h"
    

    int kmain(unsigned int ebx)
    {

      //char arr[] = "Welcome to fifthOS";
      //fb_move_cursor(6*80);
      //fb_write(arr, 20);
      //serial_write(arr, 20);
      segments_install_gdt();
      interrupts_install_idt();

      multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
      multiboot_module_t* modules = (multiboot_module_t*) mbinfo->mods_addr; 
      unsigned int address_of_module = modules->mod_start;

      if((mbinfo->mods_count) == 1){
         char str[] = "Module successfully loaded";
         serial_write(str,sizeof(str));
         
         typedef void (*call_module_t)(void);
            call_module_t start_program = (call_module_t) address_of_module;
            start_program();

      }
      else{
         char str[] = "Multiple modules loaded";
         serial_write(str,sizeof(str));
      }

      return 0;
    }

