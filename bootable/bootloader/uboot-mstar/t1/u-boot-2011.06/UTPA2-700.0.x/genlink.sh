#!/bin/bash
#Project
#Remove old links
rm -f $PWD/build
rm -f $PWD/project
rm -f $PWD/tmplib
rm -f $PWD/tools
rm -f $PWD/verify
rm -f $PWD/version
mv $PWD/projects/build $PWD/build
mv $PWD/projects/project $PWD/project
mv $PWD/projects/tmplib $PWD/tmplib
mv $PWD/projects/tools $PWD/tools
mv $PWD/projects/verify $PWD/verify
mv $PWD/projects/version $PWD/version

mkdir -p $PWD/mxlib/utopia_adaption
#msos
#Remove old links
rm -f $PWD/mxlib/msos
rm -f $PWD/mxlib/utopia_core
rm -f $PWD/mxlib/utopia_adaption/utopia
rm -f $PWD/mxlib/drv/mmio
rm -f $PWD/mxlib/hal/maserati/mmio
rm -f $PWD/mxlib/hal/maserati/msos
mv $PWD/modules/msos/msos $PWD/mxlib/msos
mv $PWD/modules/msos/utopia_core $PWD/mxlib/utopia_core
mv $PWD/modules/msos/utopia_adaption/utopia $PWD/mxlib/utopia_adaption/utopia
mv $PWD/modules/msos/drv/mmio $PWD/mxlib/drv/mmio
mv $PWD/modules/msos/hal/maserati/mmio $PWD/mxlib/hal/maserati/mmio
mv $PWD/modules/msos/hal/maserati/msos $PWD/mxlib/hal/maserati/msos


#Modules
#Remove old links
rm -f $PWD/mxlib/drv/ace
rm -f $PWD/mxlib/drv/pnl
rm -f $PWD/mxlib/drv/xc
rm -f $PWD/mxlib/api/ace
rm -f $PWD/mxlib/api/pnl
rm -f $PWD/mxlib/api/xc
rm -f $PWD/mxlib/hal/maserati/ace
rm -f $PWD/mxlib/hal/maserati/pnl
rm -f $PWD/mxlib/hal/maserati/xc
rm -f $PWD/mxlib/utopia_adaption/ace
rm -f $PWD/mxlib/utopia_adaption/pnl
rm -f $PWD/mxlib/utopia_adaption/xc

rm -f $PWD/mxlib/api/acp

rm -f $PWD/mxlib/drv/ch34

rm -f $PWD/mxlib/api/dac
rm -f $PWD/mxlib/drv/dac

rm -f $PWD/mxlib/api/dlc
rm -f $PWD/mxlib/drv/dlc
rm -f $PWD/mxlib/hal/maserati/dlc
rm -f $PWD/mxlib/utopia_adaption/dlc

rm -f $PWD/mxlib/drv/mfc
rm -f $PWD/mxlib/hal/maserati/mfc

rm -f $PWD/mxlib/drv/pq
rm -f $PWD/mxlib/hal/maserati/pq
rm -f $PWD/mxlib/utopia_adaption/pq

rm -f $PWD/mxlib/drv/sc
rm -f $PWD/mxlib/hal/maserati/sc

rm -f $PWD/mxlib/drv/ve
rm -f $PWD/mxlib/hal/maserati/ve
rm -f $PWD/mxlib/utopia_adaption/ve

rm -f $PWD/mxlib/drv/wble
rm -f $PWD/mxlib/hal/maserati/wble

rm -f $PWD/mxlib/api/audio
rm -f $PWD/mxlib/drv/audio
rm -f $PWD/mxlib/drv/audsp
rm -f $PWD/mxlib/hal/maserati/audio
rm -f $PWD/mxlib/hal/maserati/audsp
rm -f $PWD/mxlib/utopia_adaption/audio

rm -f $PWD/mxlib/drv/demod
rm -f $PWD/mxlib/drv/dmd
rm -f $PWD/mxlib/drv/dvb_extdemod
rm -f $PWD/mxlib/hal/maserati/demod
rm -f $PWD/mxlib/utopia_adaption/demod

rm -f $PWD/mxlib/drv/ca2
rm -f $PWD/mxlib/drv/dscmb
rm -f $PWD/mxlib/drv/dscmb2
rm -f $PWD/mxlib/drv/nds
rm -f $PWD/mxlib/drv/nsk2
rm -f $PWD/mxlib/drv/tcf
rm -f $PWD/mxlib/hal/maserati/dscmb
rm -f $PWD/mxlib/hal/maserati/tcf
rm -f $PWD/mxlib/utopia_adaption/dscmb

rm -f $PWD/mxlib/api/gfx
rm -f $PWD/mxlib/api/gop
rm -f $PWD/mxlib/api/gopsc
rm -f $PWD/mxlib/api/gopscd
rm -f $PWD/mxlib/drv/ge
rm -f $PWD/mxlib/drv/gop
rm -f $PWD/mxlib/drv/gopsc
rm -f $PWD/mxlib/drv/gopscd
rm -f $PWD/mxlib/hal/maserati/ge
rm -f $PWD/mxlib/hal/maserati/gop
rm -f $PWD/mxlib/utopia_adaption/gfx
rm -f $PWD/mxlib/utopia_adaption/gop

rm -f $PWD/mxlib/api/cec
rm -f $PWD/mxlib/api/hdmitx
rm -f $PWD/mxlib/api/hdmitx2
rm -f $PWD/mxlib/drv/cec
rm -f $PWD/mxlib/drv/hdmitx
rm -f $PWD/mxlib/drv/hdmitx2
rm -f $PWD/mxlib/drv/mhl
rm -f $PWD/mxlib/drv/hdcp
rm -f $PWD/mxlib/api/mhl
rm -f $PWD/mxlib/hal/maserati/cec
rm -f $PWD/mxlib/hal/maserati/mhl
rm -f $PWD/mxlib/hal/maserati/hdcp
rm -f $PWD/mxlib/utopia_adaption/cec
rm -f $PWD/mxlib/utopia_adaption/mhl

rm -f $PWD/mxlib/drv/mfe
rm -f $PWD/mxlib/drv/mfe_ex
rm -f $PWD/mxlib/api/mfe
rm -f $PWD/mxlib/api/mfe_ex
rm -f $PWD/mxlib/hal/maserati/mfe
rm -f $PWD/mxlib/hal/maserati/mfe_ex

rm -f $PWD/mxlib/drv/pcmcia
rm -f $PWD/mxlib/hal/maserati/pcmcia

rm -f $PWD/mxlib/drv/bdma
rm -f $PWD/mxlib/hal/maserati/bdma
rm -f $PWD/mxlib/utopia_adaption/bdma

rm -f $PWD/mxlib/drv/cmdq
rm -f $PWD/mxlib/hal/maserati/cmdq

rm -f $PWD/mxlib/drv/ddc2bi
rm -f $PWD/mxlib/hal/maserati/ddc2bi

rm -f $PWD/mxlib/drv/flash
rm -f $PWD/mxlib/hal/maserati/flash

rm -f $PWD/mxlib/drv/hwi2c
rm -f $PWD/mxlib/hal/maserati/hwi2c
rm -f $PWD/mxlib/utopia_adaption/hwi2c

rm -f $PWD/mxlib/drv/ipauth

rm -f $PWD/mxlib/drv/ir

rm -f $PWD/mxlib/drv/mspi
rm -f $PWD/mxlib/hal/maserati/mspi

rm -f $PWD/mxlib/drv/pwm
rm -f $PWD/mxlib/hal/maserati/pwm

rm -f $PWD/mxlib/drv/rtc
rm -f $PWD/mxlib/hal/maserati/rtc

rm -f $PWD/mxlib/drv/sar
rm -f $PWD/mxlib/hal/maserati/sar

rm -f $PWD/mxlib/drv/uart
rm -f $PWD/mxlib/hal/maserati/uart

rm -f $PWD/mxlib/drv/wdt
rm -f $PWD/mxlib/hal/maserati/wdt

rm -f $PWD/mxlib/drv/aesdma
rm -f $PWD/mxlib/drv/cipher
rm -f $PWD/mxlib/hal/maserati/aesdma
rm -f $PWD/mxlib/utopia_adaption/aesdma

rm -f $PWD/mxlib/api/hsl

rm -f $PWD/mxlib/api/mbx
rm -f $PWD/mxlib/drv/mbx
rm -f $PWD/mxlib/hal/maserati/mbx

rm -f $PWD/mxlib/api/swi2c

rm -f $PWD/mxlib/drv/cpu
rm -f $PWD/mxlib/hal/maserati/cpu

rm -f $PWD/mxlib/drv/gpio
rm -f $PWD/mxlib/hal/maserati/gpio

rm -f $PWD/mxlib/drv/irq
rm -f $PWD/mxlib/hal/maserati/irq

rm -f $PWD/mxlib/drv/miu
rm -f $PWD/mxlib/hal/maserati/miu
rm -f $PWD/mxlib/utopia_adaption/miu

rm -f $PWD/mxlib/drv/pm
rm -f $PWD/mxlib/hal/maserati/pm

rm -f $PWD/mxlib/drv/pws
rm -f $PWD/mxlib/hal/maserati/pws

rm -f $PWD/mxlib/drv/seal
rm -f $PWD/mxlib/hal/maserati/seal

rm -f $PWD/mxlib/drv/sem
rm -f $PWD/mxlib/hal/maserati/sem

rm -f $PWD/mxlib/drv/smbx

rm -f $PWD/mxlib/drv/sys
rm -f $PWD/mxlib/hal/maserati/sys

rm -f $PWD/mxlib/drv/urdma
rm -f $PWD/mxlib/hal/maserati/urdma

rm -f $PWD/mxlib/api/dmx
rm -f $PWD/mxlib/drv/fq
rm -f $PWD/mxlib/drv/mmfi
rm -f $PWD/mxlib/drv/mmfi2
rm -f $PWD/mxlib/drv/rasp
rm -f $PWD/mxlib/drv/tso
rm -f $PWD/mxlib/drv/tso2
rm -f $PWD/mxlib/drv/tsp
rm -f $PWD/mxlib/drv/tsp2
rm -f $PWD/mxlib/drv/tsp3
rm -f $PWD/mxlib/drv/tsp4
rm -f $PWD/mxlib/hal/maserati/fq
rm -f $PWD/mxlib/hal/maserati/mmfi
rm -f $PWD/mxlib/hal/maserati/tso
rm -f $PWD/mxlib/hal/maserati/tsp
rm -f $PWD/mxlib/utopia_adaption/dmx

rm -f $PWD/mxlib/drv/usb_ecos
rm -f $PWD/mxlib/drv/usbhost
rm -f $PWD/mxlib/hal/maserati/usbhost

rm -f $PWD/mxlib/drv/avd
rm -f $PWD/mxlib/drv/vbi
rm -f $PWD/mxlib/hal/maserati/avd
rm -f $PWD/mxlib/hal/maserati/vbi
rm -f $PWD/mxlib/utopia_adaption/avd
rm -f $PWD/mxlib/utopia_adaption/vbi

rm -f $PWD/mxlib/api/gpd
rm -f $PWD/mxlib/api/jpeg
rm -f $PWD/mxlib/api/jpeg_ex
rm -f $PWD/mxlib/api/njpeg_ex
rm -f $PWD/mxlib/api/vdec
rm -f $PWD/mxlib/api/vdec_ex
rm -f $PWD/mxlib/api/vdec_v3
rm -f $PWD/mxlib/drv/gpd
rm -f $PWD/mxlib/drv/hvd
rm -f $PWD/mxlib/drv/hvd_ex
rm -f $PWD/mxlib/drv/hvd_v3
rm -f $PWD/mxlib/drv/jpd
rm -f $PWD/mxlib/drv/jpd_ex
rm -f $PWD/mxlib/drv/mjpeg
rm -f $PWD/mxlib/drv/mjpeg_ex
rm -f $PWD/mxlib/drv/mjpeg_v3
rm -f $PWD/mxlib/drv/mvd
rm -f $PWD/mxlib/drv/mvd_ex
rm -f $PWD/mxlib/drv/mvd_v3
rm -f $PWD/mxlib/drv/mvop
rm -f $PWD/mxlib/drv/njpd_ex
rm -f $PWD/mxlib/drv/rvd
rm -f $PWD/mxlib/drv/stbdc
rm -f $PWD/mxlib/hal/maserati/gpd
rm -f $PWD/mxlib/hal/maserati/hvd
rm -f $PWD/mxlib/hal/maserati/hvd_ex
rm -f $PWD/mxlib/hal/maserati/hvd_v3
rm -f $PWD/mxlib/hal/maserati/jpd
rm -f $PWD/mxlib/hal/maserati/jpd_ex
rm -f $PWD/mxlib/hal/maserati/mvd
rm -f $PWD/mxlib/hal/maserati/mvd_ex
rm -f $PWD/mxlib/hal/maserati/mvd_v3
rm -f $PWD/mxlib/hal/maserati/mvop
rm -f $PWD/mxlib/hal/maserati/njpd_ex
rm -f $PWD/mxlib/hal/maserati/vpu
rm -f $PWD/mxlib/hal/maserati/vpu_ex
rm -f $PWD/mxlib/hal/maserati/vpu_v3
rm -f $PWD/mxlib/utopia_adaption/gpd
rm -f $PWD/mxlib/utopia_adaption/mvop
rm -f $PWD/mxlib/utopia_adaption/njpeg_ex
rm -f $PWD/mxlib/utopia_adaption/vdec_v3

rm -f $PWD/mxlib/drv/vif
rm -f $PWD/mxlib/hal/maserati/vif
rm -f $PWD/mxlib/utopia_adaption/vif

rm -f $PWD/mxlib/drv/clkm


mv $PWD/modules/xc/drv/ace $PWD/mxlib/drv/ace
mv $PWD/modules/xc/drv/pnl $PWD/mxlib/drv/pnl
mv $PWD/modules/xc/drv/xc $PWD/mxlib/drv/xc
mv $PWD/modules/xc/api/ace $PWD/mxlib/api/ace
mv $PWD/modules/xc/api/pnl $PWD/mxlib/api/pnl
mv $PWD/modules/xc/api/xc $PWD/mxlib/api/xc
mv $PWD/modules/xc/hal/maserati/ace $PWD/mxlib/hal/maserati/ace
mv $PWD/modules/xc/hal/maserati/pnl $PWD/mxlib/hal/maserati/pnl
mv $PWD/modules/xc/hal/maserati/xc $PWD/mxlib/hal/maserati/xc
mv $PWD/modules/xc/utopia_adaption/ace $PWD/mxlib/utopia_adaption/ace
mv $PWD/modules/xc/utopia_adaption/pnl $PWD/mxlib/utopia_adaption/pnl
mv $PWD/modules/xc/utopia_adaption/xc $PWD/mxlib/utopia_adaption/xc

mv $PWD/modules/acp/api/acp $PWD/mxlib/api/acp

mv $PWD/modules/ch34/drv/ch34 $PWD/mxlib/drv/ch34

mv $PWD/modules/dac/api/dac $PWD/mxlib/api/dac
mv $PWD/modules/dac/drv/dac $PWD/mxlib/drv/dac

mv $PWD/modules/dlc/api/dlc $PWD/mxlib/api/dlc
mv $PWD/modules/dlc/drv/dlc $PWD/mxlib/drv/dlc
mv $PWD/modules/dlc/hal/maserati/dlc $PWD/mxlib/hal/maserati/dlc
mv $PWD/modules/dlc/utopia_adaption/dlc $PWD/mxlib/utopia_adaption/dlc

mv $PWD/modules/mfc/drv/mfc $PWD/mxlib/drv/mfc
mv $PWD/modules/mfc/hal/maserati/mfc $PWD/mxlib/hal/maserati/mfc

mv $PWD/modules/pq/drv/pq $PWD/mxlib/drv/pq
mv $PWD/modules/pq/hal/maserati/pq $PWD/mxlib/hal/maserati/pq
mv $PWD/modules/pq/utopia_adaption/pq $PWD/mxlib/utopia_adaption/pq

mv $PWD/modules/sc/drv/sc $PWD/mxlib/drv/sc
mv $PWD/modules/sc/hal/maserati/sc $PWD/mxlib/hal/maserati/sc

mv $PWD/modules/ve/drv/ve $PWD/mxlib/drv/ve
mv $PWD/modules/ve/hal/maserati/ve $PWD/mxlib/hal/maserati/ve
mv $PWD/modules/ve/utopia_adaption/ve $PWD/mxlib/utopia_adaption/ve

mv $PWD/modules/wble/drv/wble $PWD/mxlib/drv/wble
mv $PWD/modules/wble/hal/maserati/wble $PWD/mxlib/hal/maserati/wble

mv $PWD/modules/audio/api/audio $PWD/mxlib/api/audio
mv $PWD/modules/audio/drv/audio $PWD/mxlib/drv/audio
mv $PWD/modules/audio/drv/audsp $PWD/mxlib/drv/audsp
mv $PWD/modules/audio/hal/maserati/audio $PWD/mxlib/hal/maserati/audio
mv $PWD/modules/audio/hal/maserati/audsp $PWD/mxlib/hal/maserati/audsp
mv $PWD/modules/audio/utopia_adaption/audio $PWD/mxlib/utopia_adaption/audio

mv $PWD/modules/demodulator/drv/demod $PWD/mxlib/drv/demod
mv $PWD/modules/demodulator/drv/dmd $PWD/mxlib/drv/dmd
mv $PWD/modules/demodulator/drv/dvb_extdemod $PWD/mxlib/drv/dvb_extdemod
mv $PWD/modules/demodulator/hal/maserati/demod $PWD/mxlib/hal/maserati/demod
mv $PWD/modules/demodulator/utopia_adaption/demod $PWD/mxlib/utopia_adaption/demod

mv $PWD/modules/dscmb/drv/ca2 $PWD/mxlib/drv/ca2
mv $PWD/modules/dscmb/drv/dscmb $PWD/mxlib/drv/dscmb
mv $PWD/modules/dscmb/drv/dscmb2 $PWD/mxlib/drv/dscmb2
mv $PWD/modules/dscmb/drv/nds $PWD/mxlib/drv/nds
mv $PWD/modules/dscmb/drv/nsk2 $PWD/mxlib/drv/nsk2
mv $PWD/modules/dscmb/drv/tcf $PWD/mxlib/drv/tcf
mv $PWD/modules/dscmb/hal/maserati/dscmb $PWD/mxlib/hal/maserati/dscmb
mv $PWD/modules/dscmb/hal/maserati/tcf $PWD/mxlib/hal/maserati/tcf
mv $PWD/modules/dscmb/utopia_adaption/dscmb $PWD/mxlib/utopia_adaption/dscmb

mv $PWD/modules/graphic/api/gfx $PWD/mxlib/api/gfx
mv $PWD/modules/graphic/api/gop $PWD/mxlib/api/gop
mv $PWD/modules/graphic/api/gopsc $PWD/mxlib/api/gopsc
mv $PWD/modules/graphic/api/gopscd $PWD/mxlib/api/gopscd
mv $PWD/modules/graphic/drv/ge $PWD/mxlib/drv/ge
mv $PWD/modules/graphic/drv/gop $PWD/mxlib/drv/gop
mv $PWD/modules/graphic/drv/gopsc $PWD/mxlib/drv/gopsc
mv $PWD/modules/graphic/drv/gopscd $PWD/mxlib/drv/gopscd
mv $PWD/modules/graphic/hal/maserati/ge $PWD/mxlib/hal/maserati/ge
mv $PWD/modules/graphic/hal/maserati/gop $PWD/mxlib/hal/maserati/gop
mv $PWD/modules/graphic/utopia_adaption/gfx $PWD/mxlib/utopia_adaption/gfx
mv $PWD/modules/graphic/utopia_adaption/gop $PWD/mxlib/utopia_adaption/gop

mv $PWD/modules/hdmi/api/cec $PWD/mxlib/api/cec
mv $PWD/modules/hdmi/api/hdmitx $PWD/mxlib/api/hdmitx
mv $PWD/modules/hdmi/api/hdmitx2 $PWD/mxlib/api/hdmitx2
mv $PWD/modules/hdmi/drv/cec $PWD/mxlib/drv/cec
mv $PWD/modules/hdmi/drv/hdmitx $PWD/mxlib/drv/hdmitx
mv $PWD/modules/hdmi/drv/hdmitx2 $PWD/mxlib/drv/hdmitx2
mv $PWD/modules/hdmi/drv/mhl $PWD/mxlib/drv/mhl
mv $PWD/modules/hdmi/drv/hdcp $PWD/mxlib/drv/hdcp
mv $PWD/modules/hdmi/api/mhl $PWD/mxlib/api/mhl
mv $PWD/modules/hdmi/hal/maserati/cec $PWD/mxlib/hal/maserati/cec
mv $PWD/modules/hdmi/hal/maserati/mhl $PWD/mxlib/hal/maserati/mhl
mv $PWD/modules/hdmi/hal/maserati/hdcp $PWD/mxlib/hal/maserati/hdcp
mv $PWD/modules/hdmi/utopia_adaption/cec $PWD/mxlib/utopia_adaption/cec
mv $PWD/modules/hdmi/utopia_adaption/mhl $PWD/mxlib/utopia_adaption/mhl

mv $PWD/modules/mfe/drv/mfe $PWD/mxlib/drv/mfe
mv $PWD/modules/mfe/drv/mfe_ex $PWD/mxlib/drv/mfe_ex
mv $PWD/modules/mfe/api/mfe $PWD/mxlib/api/mfe
mv $PWD/modules/mfe/api/mfe_ex $PWD/mxlib/api/mfe_ex
mv $PWD/modules/mfe/hal/maserati/mfe $PWD/mxlib/hal/maserati/mfe
mv $PWD/modules/mfe/hal/maserati/mfe_ex $PWD/mxlib/hal/maserati/mfe_ex

mv $PWD/modules/pcmcia/drv/pcmcia $PWD/mxlib/drv/pcmcia
mv $PWD/modules/pcmcia/hal/maserati/pcmcia $PWD/mxlib/hal/maserati/pcmcia

mv $PWD/modules/bdma/drv/bdma $PWD/mxlib/drv/bdma
mv $PWD/modules/bdma/hal/maserati/bdma $PWD/mxlib/hal/maserati/bdma
mv $PWD/modules/bdma/utopia_adaption/bdma $PWD/mxlib/utopia_adaption/bdma

mv $PWD/modules/cmdq/drv/cmdq $PWD/mxlib/drv/cmdq
mv $PWD/modules/cmdq/hal/maserati/cmdq $PWD/mxlib/hal/maserati/cmdq

mv $PWD/modules/ddc2bi/drv/ddc2bi $PWD/mxlib/drv/ddc2bi
mv $PWD/modules/ddc2bi/hal/maserati/ddc2bi $PWD/mxlib/hal/maserati/ddc2bi

mv $PWD/modules/flash/drv/flash $PWD/mxlib/drv/flash
mv $PWD/modules/flash/hal/maserati/flash $PWD/mxlib/hal/maserati/flash

mv $PWD/modules/hwi2c/drv/hwi2c $PWD/mxlib/drv/hwi2c
mv $PWD/modules/hwi2c/hal/maserati/hwi2c $PWD/mxlib/hal/maserati/hwi2c
mv $PWD/modules/hwi2c/utopia_adaption/hwi2c $PWD/mxlib/utopia_adaption/hwi2c

mv $PWD/modules/ipauth/drv/ipauth $PWD/mxlib/drv/ipauth

mv $PWD/modules/ir/drv/ir $PWD/mxlib/drv/ir

mv $PWD/modules/mspi/drv/mspi $PWD/mxlib/drv/mspi
mv $PWD/modules/mspi/hal/maserati/mspi $PWD/mxlib/hal/maserati/mspi

mv $PWD/modules/pwm/drv/pwm $PWD/mxlib/drv/pwm
mv $PWD/modules/pwm/hal/maserati/pwm $PWD/mxlib/hal/maserati/pwm

mv $PWD/modules/rtc/drv/rtc $PWD/mxlib/drv/rtc
mv $PWD/modules/rtc/hal/maserati/rtc $PWD/mxlib/hal/maserati/rtc

mv $PWD/modules/sar/drv/sar $PWD/mxlib/drv/sar
mv $PWD/modules/sar/hal/maserati/sar $PWD/mxlib/hal/maserati/sar

mv $PWD/modules/uart/drv/uart $PWD/mxlib/drv/uart
mv $PWD/modules/uart/hal/maserati/uart $PWD/mxlib/hal/maserati/uart

mv $PWD/modules/wdt/drv/wdt $PWD/mxlib/drv/wdt
mv $PWD/modules/wdt/hal/maserati/wdt $PWD/mxlib/hal/maserati/wdt

mv $PWD/modules/security/drv/aesdma $PWD/mxlib/drv/aesdma
mv $PWD/modules/security/drv/cipher $PWD/mxlib/drv/cipher
mv $PWD/modules/security/hal/maserati/aesdma $PWD/mxlib/hal/maserati/aesdma
mv $PWD/modules/security/utopia_adaption/aesdma $PWD/mxlib/utopia_adaption/aesdma

mv $PWD/modules/hsl/api/hsl $PWD/mxlib/api/hsl

mv $PWD/modules/mbx/api/mbx $PWD/mxlib/api/mbx
mv $PWD/modules/mbx/drv/mbx $PWD/mxlib/drv/mbx
mv $PWD/modules/mbx/hal/maserati/mbx $PWD/mxlib/hal/maserati/mbx

mv $PWD/modules/swi2c/api/swi2c $PWD/mxlib/api/swi2c

mv $PWD/modules/cpu/drv/cpu $PWD/mxlib/drv/cpu
mv $PWD/modules/cpu/hal/maserati/cpu $PWD/mxlib/hal/maserati/cpu

mv $PWD/modules/gpio/drv/gpio $PWD/mxlib/drv/gpio
mv $PWD/modules/gpio/hal/maserati/gpio $PWD/mxlib/hal/maserati/gpio

mv $PWD/modules/irq/drv/irq $PWD/mxlib/drv/irq
mv $PWD/modules/irq/hal/maserati/irq $PWD/mxlib/hal/maserati/irq

mv $PWD/modules/miu/drv/miu $PWD/mxlib/drv/miu
mv $PWD/modules/miu/hal/maserati/miu $PWD/mxlib/hal/maserati/miu
mv $PWD/modules/miu/utopia_adaption/miu $PWD/mxlib/utopia_adaption/miu

mv $PWD/modules/pm/drv/pm $PWD/mxlib/drv/pm
mv $PWD/modules/pm/hal/maserati/pm $PWD/mxlib/hal/maserati/pm

mv $PWD/modules/pws/drv/pws $PWD/mxlib/drv/pws
mv $PWD/modules/pws/hal/maserati/pws $PWD/mxlib/hal/maserati/pws

mv $PWD/modules/seal/drv/seal $PWD/mxlib/drv/seal
mv $PWD/modules/seal/hal/maserati/seal $PWD/mxlib/hal/maserati/seal

mv $PWD/modules/sem/drv/sem $PWD/mxlib/drv/sem
mv $PWD/modules/sem/hal/maserati/sem $PWD/mxlib/hal/maserati/sem

mv $PWD/modules/smbx/drv/smbx $PWD/mxlib/drv/smbx

mv $PWD/modules/sys/drv/sys $PWD/mxlib/drv/sys
mv $PWD/modules/sys/hal/maserati/sys $PWD/mxlib/hal/maserati/sys

mv $PWD/modules/urdma/drv/urdma $PWD/mxlib/drv/urdma
mv $PWD/modules/urdma/hal/maserati/urdma $PWD/mxlib/hal/maserati/urdma

mv $PWD/modules/dmx/api/dmx $PWD/mxlib/api/dmx
mv $PWD/modules/dmx/drv/fq $PWD/mxlib/drv/fq
mv $PWD/modules/dmx/drv/mmfi $PWD/mxlib/drv/mmfi
mv $PWD/modules/dmx/drv/mmfi2 $PWD/mxlib/drv/mmfi2
mv $PWD/modules/dmx/drv/rasp $PWD/mxlib/drv/rasp
mv $PWD/modules/dmx/drv/tso $PWD/mxlib/drv/tso
mv $PWD/modules/dmx/drv/tso2 $PWD/mxlib/drv/tso2
mv $PWD/modules/dmx/drv/tsp $PWD/mxlib/drv/tsp
mv $PWD/modules/dmx/drv/tsp2 $PWD/mxlib/drv/tsp2
mv $PWD/modules/dmx/drv/tsp3 $PWD/mxlib/drv/tsp3
mv $PWD/modules/dmx/drv/tsp4 $PWD/mxlib/drv/tsp4
mv $PWD/modules/dmx/hal/maserati/fq $PWD/mxlib/hal/maserati/fq
mv $PWD/modules/dmx/hal/maserati/mmfi $PWD/mxlib/hal/maserati/mmfi
mv $PWD/modules/dmx/hal/maserati/tso $PWD/mxlib/hal/maserati/tso
mv $PWD/modules/dmx/hal/maserati/tsp $PWD/mxlib/hal/maserati/tsp
mv $PWD/modules/dmx/utopia_adaption/dmx $PWD/mxlib/utopia_adaption/dmx

mv $PWD/modules/usb/drv/usb_ecos $PWD/mxlib/drv/usb_ecos
mv $PWD/modules/usb/drv/usbhost $PWD/mxlib/drv/usbhost
mv $PWD/modules/usb/hal/maserati/usbhost $PWD/mxlib/hal/maserati/usbhost

mv $PWD/modules/vd/drv/avd $PWD/mxlib/drv/avd
mv $PWD/modules/vd/drv/vbi $PWD/mxlib/drv/vbi
mv $PWD/modules/vd/hal/maserati/avd $PWD/mxlib/hal/maserati/avd
mv $PWD/modules/vd/hal/maserati/vbi $PWD/mxlib/hal/maserati/vbi
mv $PWD/modules/vd/utopia_adaption/avd $PWD/mxlib/utopia_adaption/avd
mv $PWD/modules/vd/utopia_adaption/vbi $PWD/mxlib/utopia_adaption/vbi

mv $PWD/modules/gpd/api/gpd $PWD/mxlib/api/gpd
mv $PWD/modules/gpd/drv/gpd $PWD/mxlib/drv/gpd
mv $PWD/modules/gpd/hal/maserati/gpd $PWD/mxlib/hal/maserati/gpd
mv $PWD/modules/gpd/utopia_adaption/gpd $PWD/mxlib/utopia_adaption/gpd

mv $PWD/modules/vdec_v1/drv/hvd $PWD/mxlib/drv/hvd
mv $PWD/modules/vdec_v1/drv/mjpeg $PWD/mxlib/drv/mjpeg
mv $PWD/modules/vdec_v1/drv/mvd $PWD/mxlib/drv/mvd
mv $PWD/modules/vdec_v1/drv/rvd $PWD/mxlib/drv/rvd
mv $PWD/modules/vdec_v1/api/vdec $PWD/mxlib/api/vdec
mv $PWD/modules/vdec_v1/hal/maserati/hvd $PWD/mxlib/hal/maserati/hvd
mv $PWD/modules/vdec_v1/hal/maserati/mvd $PWD/mxlib/hal/maserati/mvd
mv $PWD/modules/vdec_v1/hal/maserati/vpu $PWD/mxlib/hal/maserati/vpu

mv $PWD/modules/vdec_v2/drv/hvd_ex $PWD/mxlib/drv/hvd_ex
mv $PWD/modules/vdec_v2/drv/mjpeg_ex $PWD/mxlib/drv/mjpeg_ex
mv $PWD/modules/vdec_v2/drv/mvd_ex $PWD/mxlib/drv/mvd_ex
mv $PWD/modules/vdec_v2/api/vdec_ex $PWD/mxlib/api/vdec_ex
mv $PWD/modules/vdec_v2/hal/maserati/hvd_ex $PWD/mxlib/hal/maserati/hvd_ex
mv $PWD/modules/vdec_v2/hal/maserati/mvd_ex $PWD/mxlib/hal/maserati/mvd_ex
mv $PWD/modules/vdec_v2/hal/maserati/vpu_ex $PWD/mxlib/hal/maserati/vpu_ex

mv $PWD/modules/vdec_v3/drv/hvd_v3 $PWD/mxlib/drv/hvd_v3
mv $PWD/modules/vdec_v3/drv/mjpeg_v3 $PWD/mxlib/drv/mjpeg_v3
mv $PWD/modules/vdec_v3/drv/mvd_v3 $PWD/mxlib/drv/mvd_v3
mv $PWD/modules/vdec_v3/api/vdec_v3 $PWD/mxlib/api/vdec_v3
mv $PWD/modules/vdec_v3/hal/maserati/hvd_v3 $PWD/mxlib/hal/maserati/hvd_v3
mv $PWD/modules/vdec_v3/hal/maserati/mvd_v3 $PWD/mxlib/hal/maserati/mvd_v3
mv $PWD/modules/vdec_v3/hal/maserati/vpu_v3 $PWD/mxlib/hal/maserati/vpu_v3
mv $PWD/modules/vdec_v3/utopia_adaption/vdec_v3 $PWD/mxlib/utopia_adaption/vdec_v3

mv $PWD/modules/ojpd_vdec_v1/drv/jpd $PWD/mxlib/drv/jpd
mv $PWD/modules/ojpd_vdec_v1/api/jpeg $PWD/mxlib/api/jpeg
mv $PWD/modules/ojpd_vdec_v1/hal/maserati/jpd $PWD/mxlib/hal/maserati/jpd

mv $PWD/modules/ojpd_vdec_v2/drv/jpd_ex $PWD/mxlib/drv/jpd_ex
mv $PWD/modules/ojpd_vdec_v2/api/jpeg_ex $PWD/mxlib/api/jpeg_ex
mv $PWD/modules/ojpd_vdec_v2/hal/maserati/jpd_ex $PWD/mxlib/hal/maserati/jpd_ex

mv $PWD/modules/mvop/drv/mvop $PWD/mxlib/drv/mvop
mv $PWD/modules/mvop/hal/maserati/mvop $PWD/mxlib/hal/maserati/mvop
mv $PWD/modules/mvop/utopia_adaption/mvop $PWD/mxlib/utopia_adaption/mvop

mv $PWD/modules/njpd/drv/njpd_ex $PWD/mxlib/drv/njpd_ex
mv $PWD/modules/njpd/api/njpeg_ex $PWD/mxlib/api/njpeg_ex
mv $PWD/modules/njpd/hal/maserati/njpd_ex $PWD/mxlib/hal/maserati/njpd_ex
mv $PWD/modules/njpd/utopia_adaption/njpeg_ex $PWD/mxlib/utopia_adaption/njpeg_ex

mv $PWD/modules/stbdc/drv/stbdc $PWD/mxlib/drv/stbdc

mv $PWD/modules/vif/drv/vif $PWD/mxlib/drv/vif
mv $PWD/modules/vif/hal/maserati/vif $PWD/mxlib/hal/maserati/vif
mv $PWD/modules/vif/utopia_adaption/vif $PWD/mxlib/utopia_adaption/vif

mv $PWD/modules/clkm/drv/clkm $PWD/mxlib/drv/clkm
