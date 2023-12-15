/**********************第一部分:声明******************** */
.text           // 代码段
.align 2        // 2字节对齐
.global _start  // 定义全局变量_start

/**********************第二部分:开始******************** */
_start:

    bl light_led   // 调用light_led函数

/*死循环 */
loop:
    b loop         


/**********************第三部分:寄存器定义******************** */

#define gpio1_clock_enable_ccm_ccgr1 0x20c406c   // ccm_ccgr1时钟,全部使能

#define gpio1_io04_mux_ctl_register 0x20e006c   // gpio1_io04复用控制寄存器,设置GPIO1_io04的复用功能

#define gpio1_io04_pad_ctl_register 0x20e02f8   // gpio1_io04电气属性寄存器,设置为GPIO的电气属性

#define gpio1_gdir_register         0x0209C004    // gpio1_gdir寄存器,控制输入输出

#define gpio1_dr_register           0x0209C000    // gpio1_dr寄存器,控制输出高低电平


/**********************第四部分:使能时钟,io配置,gpio配置******************** */
light_led:
    /*使能时钟gpio1时钟 */
    ldr r0, =0x20c406c  // 将gpio1_clock_enable_ccm_ccgr1的地址赋值给r0
    ldr r1, =0xFFFFFFFF                    // 0xFFFFFFFF
    str r1, [r0]                           // 将r1的值写入r0的地址中

    /*将pad引脚复用为gpio* */
    ldr r0, =0x20e006c   // 将gpio1_io04_mux_ctl_register的地址赋值给r0
    ldr r1, =0x5                           // 将0x5赋值给r1
    str r1, [r0]                           // 将r1的值写入r0的地址中

    /*设置gpio1_io04的电气属性 
     * bit0:压摆率 0低速率
     * bit5:3: 驱动能力 110:R0/6
     * bit 7:6: speed 10
     * bit11: 0关闭开漏输出
     * bit12: 1 pull/keeper 使能
     * bit13: 0 keeper
     * bit15-14:00 100k 下拉
     * bit16: 0 关闭Hyst
     * 10b0
    */

    ldr r0, =0x20e02f8   // 将gpio1_io04_pad_ctl_register的地址赋值给r0
    ldr r1, =0x10b0                       // 将0x10b0赋值给r1
    str r1, [r0]                           // 将r1的值写入r0的地址中

    /*设置gpio_gdir.[4]设置为1,即设置为输出模式*/
    ldr r0, =0x0209C004           // 将gpio1_gdir_register的地址赋值给r0
    ldr r1, =0x10                           // 将0x10赋值给r1
    str r1, [r0]                           // 将r1的值写入r0的地址中

    ldr r0, =0x0209C000             // 将gpio1_dr_register的地址赋值给r0
    ldr r1, =0x0                           // 将0x0赋值给r1
    str r1, [r0]                           // 将r1的值写入r0的地址中

    mov pc, lr                             // 跳转到lr寄存器的地址中

