
.align 3
.section .data
.global _num_app
_num_app:
.quad 2
.quad app_0_start
.quad app_1_start
.quad app_1_end
.section .data
.global app_0_start
.global app_0_end
.align 3
app_0_start:
.incbin "../user/bin/time"
app_0_end:
.section .data
.global app_1_start
.global app_1_end
.align 3
app_1_start:
.incbin "../user/bin/write"
app_1_end: