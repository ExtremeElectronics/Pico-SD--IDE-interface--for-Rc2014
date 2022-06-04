#include <stdio.h>
#include  "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "hardware/structs/bus_ctrl.h"
#include "pico/multicore.h"
#include "z80io.pio.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "ide.h"

//sd card reader
#include "f_util.h"
#include "ff.h"
#include "hw_config.h"

//iniparcer
#include "dictionary.h"
#include "iniparser.h"


//pico pio globals
uint8_t registers[16];
PIO pio = pio0;  
uint sm_z80io; 

//IDE globals
static int ide =1; //set to 1 to init IDE
struct ide_controller *ide0;
const char * idepath ="CPMdisk.img";
const char * idepathi ="CPMIDE.id";
int iscf=0;

#define trace_interface 1
#define trace_ide 2

int trace=0;

//Drive LED
const uint LEDPIN = PICO_DEFAULT_LED_PIN;


//isr currently not used (I cant get it to work)
void isr()
{
    printf(" here ");
    pio_sm_put(pio,sm_z80io,0xaa);

//    pio_interrupt_clear(pio0, 0);
    irq_clear(PIO0_IRQ_0);
      }


int SDFileExists(char * filename){
    FRESULT fr;
    FILINFO fno;

    fr = f_stat(filename, &fno);
    return fr==FR_OK;
}

static uint8_t my_ide_read(uint16_t addr)
{
        uint8_t r =  ide_read8(ide0, addr);
        if (trace & trace_ide) printf( "ide read %d = %02X\n", addr, r);
        return r;
}

static void my_ide_write(uint16_t addr, uint8_t val)
{
        if (trace & trace_ide) printf( "ide write %d = %02X\n", addr, val);
        ide_write8(ide0, addr, val);
}

//second core handles z80
void z80io_core_entry() {
        float freq = 60000000.0; 
	float div = (float)clock_get_hz(clk_sys) / freq;
	uint offset_z80io = pio_add_program(pio, &z80io_program);

	sm_z80io= pio_claim_unused_sm(pio, true);	
	z80io_init(pio, sm_z80io, offset_z80io, div);
	pio_sm_set_enabled(pio, sm_z80io, true);

	uint32_t io = 0;
	uint32_t c=0;
	uint16_t addr=0;
	uint8_t data=0;
/*	
        pio_set_irq0_source_enabled(pio, pis_interrupt0, true);
        irq_set_exclusive_handler(PIO0_IRQ_0, isr);
        irq_set_enabled(PIO0_IRQ_0, true);
*/
    
//	pio_sm_put(pio,sm_z80io,c & 0);
	while (1) {
            
//z80read Pico  SENDS data
	    if(pio_interrupt_get(pio, 5)){
              //io = pio_sm_get(pio, sm_z80io);
	      while(pio_sm_get_rx_fifo_level(pio,sm_z80io)){ //remove if not needed
	        io = pio_sm_get(pio, sm_z80io);
	      }
	      addr=(io >>10) & 0x07;
	      
	      data=my_ide_read(addr & 7); //call to ide disk routines
	      
	      pio_sm_clear_fifos(pio, sm_z80io); //remove if not needed

	      pio_sm_put(pio,sm_z80io,data);
	      if (trace & trace_interface ) {printf("io:%04X Wr reg[%02X]->%02X %03i\n\r",io,addr,data,data);}
	      pio_interrupt_clear(pio, 5);
	    }
	    
//z80write Pico RECEIVES data
	    if(pio_interrupt_get(pio, 6)){
//	      io = pio_sm_get(pio, sm_z80io);
              while(pio_sm_get_rx_fifo_level(pio,sm_z80io)){ //remove if not needed
                io = pio_sm_get(pio, sm_z80io);
              }

	      pio_sm_clear_fifos(pio, sm_z80io); //remove if not needed
	      addr=(io >>10) & 0x07;
	      data=(io & 0x00ff);

	      my_ide_write(addr & 7, data); //call to ide disk routines

              if (trace & trace_interface ){printf("io:%04X Rd reg[%02X]<-%02X \n\r",io,addr,data);}
              //registers[addr]=data;
              pio_interrupt_clear(pio, 6);
	    }
		
		
	}
	
}

int main(){
	stdio_init_all();
	printf("\r\nz80disk\n\r\n");

//LED
        gpio_init(LEDPIN);
        gpio_set_dir(LEDPIN, GPIO_OUT);
        gpio_put(LEDPIN, 1);
        sleep_ms(200);
        gpio_put(LEDPIN, 0);
        sleep_ms(200);
        
//banner

        printf( "\n\n\r     ________________________________");
        printf(   "\n\r    /                                |");
        printf(   "\n\r   /           RC2014                |");
        printf(   "\n\r  /       SD Pico Disk Drive         |");
        printf(   "\n\r |  O                                |");
        printf(   "\n\r |          Derek Woodroffe          |");
        printf(   "\n\r |              2022                 |");
        //printf(   "\n\r |                                   |");
        printf(   "\n\r |___________________________________|");
        printf(   "\n\r   | | | | | | | | | | | | | | | | |  \n\n\r");
	
// mount SD Card
        sd_card_t *pSD = sd_get_by_num(0);
        FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
        if (FR_OK != fr){
          panic("SD card f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
          while(1); //halt
        }

        printf("\n\n\rSD INIT OK \n\r",1);

// INI Parser 
        dictionary * ini ;
        char       * ini_name ;
        const char  *   s ;
        const char * inidesc;

        ini_name = "rc2014.ini";
        if (! SDFileExists(ini_name)){
          ini_name = "rc2040.ini";
          printf("rc2014.ini not found, Trying rc2040.ini\n\r");
        }
        
        if (SDFileExists(ini_name)){
          printf("Ini file %s Exists Loading ... \n\r",ini_name);

          ini = iniparser_load(fr, ini_name);

          // IDE cf file
          iscf=iniparser_getint(ini, "IDE:iscf", iscf);

          idepathi =iniparser_getstring(ini, "IDE:idefilei", "");
          idepath =iniparser_getstring(ini, "IDE:idefile", idepath);

          // Trace enable from inifile
          trace=iniparser_getint(ini, "DEBUG:trace",trace );

          printf("Loaded INI\n\r");
        }else{
          printf("Cant open ini file \n\r stopped \n\n");
          while(1);
        }
// End of INI Parser 



//IO setup
	gpio_init(14);
	gpio_init(15);
	gpio_set_dir(14,GPIO_IN);
	gpio_set_dir(15,GPIO_IN);

//IDE setup
        FIL fili;
        FIL fild;
        ide0 = ide_allocate("cf");
        if (ide0) {
          if (iscf==0){
            FRESULT ide_fri=f_open(&fili, idepathi, FA_READ | FA_WRITE);
              if (ide_fri != FR_OK) {
                printf("\n\r Error IDE ident file Open Fail %s \n\r",idepathi);
                ide = 0;
              }
          }
          FRESULT ide_frd=f_open(&fild, idepath, FA_READ | FA_WRITE);
          if ( ide_frd != FR_OK) {
             //perror(idepath);
             printf("\n\r Error IDE Data Open Fail %s \n\r",idepath);
             ide = 0;
          }
          if (ide_attach(ide0, 0, fili,fild,iscf) == 0) {
//             ide = 1;
             ide_reset_begin(ide0);
             printf( "IDE0 Open OK\n\r");
          }else{
            ide=0;
          }  
        }
        if(ide==0){
           printf("\n\rWe have a failure to allocate ide\n\r Stopping\n\n");
        }else{     
          printf("\n\r%s loaded",idepath);
          printf("\n\rTrace:%i",trace);
          printf("\n\r\n\r OK lets start the z80 stuff\n\r");
	  multicore_launch_core1(z80io_core_entry);
	  printf("\n\r Listning for IDE activity... \n\r");
	  if (trace==0){ 
	   printf("\n\r Trace zero, so no more info shown, \n\r Keep the speed up and the noise down\n\r SSShhh!!!!!....\n\r");
	  }
        }


//twiddle thumbs
	while (1) {

// really need to think of something to do here.. Such a waste... 
// Maybe emulate an RC2014 :) 

	}
	
}

