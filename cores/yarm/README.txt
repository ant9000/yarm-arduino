CHECKED:

./Arduino.h
    digitalPinToInterrupt() is ok for ARDUINO_SAMD_VARIANT_COMPLIANCE >= 10606
    for ARDUINO_SAMD_VARIANT_COMPLIANCE < 10606 it is not defined?

./cortex_handlers.c
    exception_table adapted from sam0/utils/cmsis/saml21/include_b/saml21e18b.h

./pulse.c
    pulseIn(): need to check loop constants length=13, offset=16

./Reset.cpp
    APP_START is defined as __text_start__, provided by linker script:
      ~/.arduino15/packages/arduino/hardware/samd/1.6.11/variants/arduino_zero/linker_scripts/gcc/flash_with_bootloader.ld
    NB: needs ARDUINO_SAMD_VARIANT_COMPLIANCE >= 10610

./SERCOM.h
    SERCOM_FREQ_REF is for a 48MHz CPU clock
    enums need to be validated

./SERCOM.cpp
    SERCOM::initClockNVIC modified to use SAML21 clock and bus code
 
./Tone.cpp
    use TC4 instead of TC5; modified clock, bus and tc code for SAML21

TODO:

./startup.c
    SystemInit() sets up the clocks

./USB/samd21_host.h
    hardware dependent

./USB/samd21_host.c
    hardware dependent

./USB/SAMD21_USBDevice.h
    hardware dependent

./USB/USBCore.cpp
    linked to USBDevice_SAMD21G18x (from SAMD21_USBDevice.h)

./WInterrupts.c
    hardware dependent

./wiring_analog.c
    hardware dependent

./wiring.c
    check SystemCoreClock; init() is hardware dependent

./wiring_digital.c
    hardware dependent

./wiring_private.c
    hardware dependent

./WVariant.h
    hardware dependent
