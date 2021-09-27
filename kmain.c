#include "drivers/serial_port/serial_port.h"
#include "memory/segmentation/segments.h"
#include "drivers/interrupts/interrupts.h"
#include "multiboot.h"
#include "memory/paging/paging.h"
#include "user_mode/user_mode.h"
#include "drivers/interrupts/interrupt_enabler.h"

void init(u32int kernelPhysicalStart, u32int kernelPhysicalEnd){
   segments_install_gdt();
   interrupts_install_idt();
   init_paging(kernelPhysicalStart, kernelPhysicalEnd);
}

int kmain(unsigned int ebx, u32int kernel_physical_start, u32int kernel_physical_end){
   
   init(kernel_physical_start, kernel_physical_end);

   disable_hardware_interrupts();
  	
  	switch_to_user_mode();

   return 0;
}

