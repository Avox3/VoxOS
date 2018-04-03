


boot_stage1:
  %include "src/boot/Stage1/Boot1.asm"
  align 512, db 0

boot_stage2:
  %include "src/boot/Stage2/Stage2.asm"
  align 512, db 0
