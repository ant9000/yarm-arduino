# Path to top level ASF directory relative to this project directory.
PRJ_PATH = ASF/

# Target CPU architecture: cortex-m3, cortex-m4
ARCH = cortex-m0plus

# Target part: none, sam3n4 or sam4l4aa
PART = saml21j18b

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET_FLASH = yarm_bootloader.elf

# List of C source files.
CSRCS = \
       common/services/sleepmgr/saml/sleepmgr.c           \
       common/services/usb/class/cdc/device/udi_cdc.c     \
       common/services/usb/class/cdc/device/udi_cdc_desc.c \
       common/services/usb/udc/udc.c                      \
       common/utils/interrupt/interrupt_sam_nvic.c        \
       ../main.c                                          \
       ../sam_ba_monitor.c                                \
       ../usart_sam_ba.c                                  \
       sam0/boards/saml21_xplained_pro_b/board_init.c     \
       sam0/drivers/extint/extint_callback.c              \
       sam0/drivers/extint/extint_sam_l_c/extint.c        \
       sam0/drivers/port/port.c                           \
       sam0/drivers/sercom/sercom.c                       \
       sam0/drivers/sercom/sercom_interrupt.c             \
       sam0/drivers/sercom/usart/usart.c                  \
       sam0/drivers/sercom/usart/usart_interrupt.c        \
       sam0/drivers/system/clock/clock_saml21/clock.c     \
       sam0/drivers/system/clock/clock_saml21/gclk.c      \
       sam0/drivers/system/interrupt/system_interrupt.c   \
       sam0/drivers/system/pinmux/pinmux.c                \
       sam0/drivers/system/system.c                       \
       sam0/drivers/usb/stack_interface/usb_device_udd.c  \
       sam0/drivers/usb/stack_interface/usb_dual.c        \
       sam0/drivers/usb/usb_sam_l/usb.c                   \
       sam0/utils/cmsis/saml21/source/gcc/startup_saml21.c \
       sam0/utils/cmsis/saml21/source/system_saml21.c     \
       sam0/utils/syscalls/gcc/syscalls.c

# List of assembler source files.
ASSRCS = 

# List of include paths.
INC_PATH = \
       common/boards                                      \
       common/services/sleepmgr                           \
       common/services/usb                                \
       common/services/usb/class/cdc                      \
       common/services/usb/class/cdc/device               \
       common/services/usb/udc                            \
       common/utils                                       \
       ..                                                 \
       ../conf                                            \
       sam0/boards                                        \
       sam0/boards/saml21_xplained_pro_b                  \
       sam0/drivers/extint                                \
       sam0/drivers/extint/extint_sam_l_c                 \
       sam0/drivers/port                                  \
       sam0/drivers/sercom                                \
       sam0/drivers/sercom/usart                          \
       sam0/drivers/system                                \
       sam0/drivers/system/clock                          \
       sam0/drivers/system/clock/clock_saml21             \
       sam0/drivers/system/interrupt                      \
       sam0/drivers/system/interrupt/system_interrupt_saml21 \
       sam0/drivers/system/pinmux                         \
       sam0/drivers/system/power                          \
       sam0/drivers/system/power/power_sam_l              \
       sam0/drivers/system/reset                          \
       sam0/drivers/system/reset/reset_sam_l              \
       sam0/drivers/usb                                   \
       sam0/drivers/usb/stack_interface                   \
       sam0/drivers/usb/usb_sam_l                         \
       sam0/utils                                         \
       sam0/utils/cmsis/saml21/include_b                  \
       sam0/utils/cmsis/saml21/source                     \
       sam0/utils/header_files                            \
       sam0/utils/preprocessor                            \
       thirdparty/CMSIS/Include                           \
       thirdparty/CMSIS/Lib/GCC

# Additional search paths for libraries.
LIB_PATH =  \
       thirdparty/CMSIS/Lib/GCC                          

# List of libraries to use during linking.
LIBS =  \
       arm_cortexM0l_math                                

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT_FLASH = sam0/utils/linker_scripts/saml21/gcc/saml21j18b_flash.ld

# Path relative to top level directory pointing to a linker script.
DEBUG_SCRIPT_FLASH = sam0/boards/saml21_xplained_pro_b/debug_scripts/gcc/saml21_xplained_pro_flash.gdb

# Additional options for debugging. By default the common Makefile.in will
# add -g3.
DBGFLAGS = 

# Application optimization used during compilation and linking:
# -O0, -O1, -O2, -O3 or -Os
OPTIMIZATION = -Os

# Extra flags to use when archiving.
ARFLAGS = 

# Extra flags to use when assembling.
ASFLAGS = 

# Extra flags to use when compiling.
CFLAGS = 

# Extra flags to use when preprocessing.
#
# Preprocessor symbol definitions
#   To add a definition use the format "-D name[=definition]".
#   To cancel a definition use the format "-U name".
#
# The most relevant symbols to define for the preprocessor are:
#   BOARD      Target board in use, see boards/board.h for a list.
#   EXT_BOARD  Optional extension board in use, see boards/board.h for a list.
CPPFLAGS = \
       -D ARM_MATH_CM0PLUS=true                           \
       -D BOARD=SAML21_XPLAINED_PRO                       \
       -D EXTINT_CALLBACK_MODE=true                       \
       -D UDD_ENABLE                                      \
       -D USART_CALLBACK_MODE=true                        \
       -D USB_DEVICE_LPM_SUPPORT                          \
       -D __SAML21J18B__

# Extra flags to use when linking
LDFLAGS = \
                                                          \
       -Wl,--defsym,STACK_SIZE=0x400                      \
       -Wl,--defsym,__stack_size__=0x400

BUILD_DIR = build/
