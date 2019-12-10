#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void user_procTask(os_event_t *events);

static /*volatile*/ os_timer_t some_timer;

static void aled(int value);

//#define BIT_LED BIT2
#define BIT_LED BIT1

#define LED_GPC (GPIO_PIN0_ADDRESS + 0x4)

static bool blink = false;

void some_timerfunc(void *arg)
{
    blink = !blink;
    aled(blink);
    /*
    //Do blinky stuff
    if (!blink) //GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT_LED)
    {
        //Set GPIO2 to LOW
        gpio_output_set(0, BIT_LED, BIT_LED, 0);
    }
    else
    {
        //Set GPIO2 to HIGH
        gpio_output_set(BIT_LED, 0, BIT_LED, 0);
    }
    */
}

void aled_init() {
    // GPF_pin addr = 0x60000818 value = 0x30.
    // GPC_pin addr = 0x6000032c value = 0x00.
    // GPES    addr = 0x60000310 value = 0x02.    
    //*((char*)0x60000818) = 0x30;

    /*GPF(1)*/ PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_GPIO1);  /* GPFFS0 and GPFFS1 */
    /*GPC(1)*/ GPIO_REG_WRITE(LED_GPC, 0x00);
    /*GPES*/   GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, BIT_LED);
}

void aled(int value) {
    if (value) gpio_output_set(BIT_LED, 0, BIT_LED, 0);  // HIGH
    else       gpio_output_set(0, BIT_LED, BIT_LED, 0);  // LOW
}


//Do nothing function
static void ICACHE_FLASH_ATTR
user_procTask(os_event_t *events)
{
    os_delay_us(10);
}

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
    // Initialize the GPIO subsystem.
    gpio_init();

    //Set GPIO2 to output mode
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

    //Set GPIO2 low
    //gpio_output_set(0, BIT_LED, BIT_LED, 0);

    aled_init();
    gpio_output_set(1, 0, 1, 0);

    //Disarm timer
    os_timer_disarm(&some_timer);

    //Setup timer
    os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);

    //Arm the timer
    //&some_timer is the pointer
    //1000 is the fire time in ms
    //0 for once and 1 for repeating
    os_timer_arm(&some_timer, 1000, 1);
    
    //Start os task
    system_os_task(user_procTask, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
}
