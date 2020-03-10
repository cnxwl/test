#include <iostream>
#include <fstream>
#include "hiPacker.h"
#include "include/modbus.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

using namespace std;
//slave id
#define SLAVE_ID 1
int main(void)
{
    logger_name = "/home/sab/HiPacker/log/HiPacker_logging_" + get_current_date_time(time(0)) + ".log";
    logger_file.open(logger_name, ios_base::app);
    /*cout << init_open_label_printer() << endl;
    //logger_file << logging_generation("INFO", "test");
    logger_file.close();*/
    
    int s = -1;
    modbus_t *ctx = NULL;
    modbus_mapping_t *mb_mapping ;
    int use_backend;
    ctx = modbus_new_rtu("/dev/ttyUSB0",9600,'N',8,1);

    //set slave
    modbus_set_slave(ctx, SLAVE_ID);
    logger_file << logging_generation("INFO","set slave address Ok!");

    //modbus connect
    if(modbus_connect(ctx) == -1)
    {
       fprintf(stderr,"Connection failed: %s\n",modbus_strerror(errno));
       logger_file << logging_generation("ERR","Connection failed:")<<modbus_strerror(errno)<<endl;
       modbus_free(ctx);
       logger_file.close();
       return -1;
    }
    logger_file << logging_generation("INFO","modbus connect Ok!");

    //register map Initialization
    mb_mapping = modbus_mapping_new(MODBUS_MAX_READ_BITS,0,MODBUS_MAX_READ_REGISTERS,0);
    if(mb_mapping == NULL)
    {
        fprintf(stderr,"Failed to allocate the mapping:%s\n",

        modbus_strerror(errno));
        modbus_free(ctx);
        logger_file << logging_generation("ERR","Failed to allocate the mapping:");
        logger_file.close();
        return -1;      
    }

    mb_mapping->tab_registers[0] = 1;
    mb_mapping->tab_registers[1] = 2;
    mb_mapping->tab_registers[2] = 3;
    mb_mapping->tab_registers[3] = 4;
    mb_mapping->tab_registers[4] = 5;

    int tabRegisters[5];
    for(int i=0;i<sizeof(tabRegisters);i++)
    {
      tabRegisters[i] = mb_mapping->tab_registers[i];
    }

    while(true)
    {
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        int rc = modbus_receive(ctx, query);
        
        if(rc > 0)
        {
            modbus_reply(ctx, query, rc, mb_mapping);
            if(tabRegisters[0] != mb_mapping->tab_registers[0])
            {
                 cout <<"Number1:"<< mb_mapping->tab_registers[0]<<endl;
                 tabRegisters[0] = -1;
            }
            if(tabRegisters[1] != mb_mapping->tab_registers[1])
            {
                 cout <<"Number2:"<< mb_mapping->tab_registers[1]<<endl;
            }
           if(tabRegisters[2] != mb_mapping->tab_registers[2])
            {
                 cout <<"Number3:"<< mb_mapping->tab_registers[2]<<endl;
            }
            if(tabRegisters[3] != mb_mapping->tab_registers[3])
            {
                 cout <<"Number4:"<< mb_mapping->tab_registers[3]<<endl;
            }if(tabRegisters[4] != mb_mapping->tab_registers[4])
            {
                 cout <<"Number5:"<< mb_mapping->tab_registers[4]<<endl;
            }
        }
        else if(rc == -1)
        {
            break;
        }
    }
    if(s != -1)
    {
        close(s);
    }

    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);

    logger_file.close();
    return 0;
}
