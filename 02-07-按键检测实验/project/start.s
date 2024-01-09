.global _start  // 定义全局标号
.global _bss_start
.global _bss_end
_bss_start:
	.word __bss_start
_bss_end:
	.word __bss_end

_start:
	/* 进入SVC模式 */
	mrs r0, cpsr
	bic r0, r0, #0x1f 	/* 将r0寄存器中的低5位清零，也就是cpsr的M0~M4 	*/
	orr r0, r0, #0x13 	/* r0或上0x13,表示使用SVC模式					*/
	msr cpsr, r0		/* 将r0 的数据写入到cpsr_c中 					*/

	/* 清楚bss段 */
@ 	ldr r0, _bss_start  // r0 = _bss_start
@ 	ldr r1, _bss_end  // 	r1 = _bss_end
@ 	mov r2, #0  // r2 = 0

@ bss_loop:
@ 	stmia r0!, {r2}  // *r0 = r2, r0 += 1
@ 	cmp r0, r1  // 比较r0和r1
@ 	ble bss_loop  // 如果r0 <= r1, 则跳转到bss_loop


	ldr sp, =0X80200000	/* 设置栈指针			 */
	b main				/* 跳转到main函数 		 */


