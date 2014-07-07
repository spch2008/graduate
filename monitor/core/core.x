struct pm_info
{
    char  *ip;             
    float  memory_ratio;   
    float  cpu_ration;    
    char  *vm_flow_info;   
};

enum info_type
{
    Memory_Only, Cpu_Only, VmFlow_Only, Memory_Cpu, All
};

program MONITOR_PROG
{
    version MONITOR_VERS
    {
        pm_info GETPMINFO(info_type) = 1;
    } = 1;
} = 0x31230000;