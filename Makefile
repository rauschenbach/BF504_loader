PROGNAME = bf504f_loader
CC_DIR = C:/Program Files (x86)/Analog Devices/VisualDSP 5.1.2
CC = ${CC_DIR}/ccblkfn.exe
AS = ${CC_DIR}/easmBLKFN.exe


REVISION = -si-revision 0.1
CFLAGS = -g -structs-do-not-overlap -no-multiline -D__PROCESSOR_SPEED__=192000000 -DBOOT_LOADER -double-size-32 -decls-strong -warn-protos ${REVISION} -proc ADSP-BF504F -file-attr ProjectName=${PROGNAME}
ASFLAG = -proc ADSP-BF504F ${REVISION} -MM -file-attr ProjectName={PROGNAME} 
LDFLAG = -flags-link -MDUSER_CRT=ADI_QUOTEbf504f_loader_basiccrt.dojADI_QUOTE,-MD__cplusplus -flags-link -e \
	-flags-link -ev -flags-link -od,.\Release -proc ADSP-BF504F  ${REVISION} -flags-link -MM


OBJS = $(addprefix Release/,diskio.doj ff.doj ffunicode.doj flash.doj bf504f_loader_basiccrt.doj bf504f_loader_heaptab.doj led.doj main.doj pll.doj ports.doj \
   rele.doj rsi.doj sport0.doj timer0.doj xpander.doj)


TMPFILES =  *.c~ *.h~

Release/bootloader.bin: Release/${PROGNAME}.dxe Makefile
	${CC_DIR}/elfloader.exe Release\${PROGNAME}.dxe -b Flash -f binary -Width 16 -o Release/bootloader.bin ${REVISION} -proc ADSP-BF504F -MM


Release/${PROGNAME}.dxe: ${OBJS}
	${CC} $^ -o $@ -T ${PROGNAME}.ldf -map Release\${PROGNAME}.map.xml -flags-link -xref -L .\Release ${LDFLAG}

clean:
	@del /F /Q $(subst /,\,${OBJS}) Release\${PROGNAME}* Release\*.xml Release\*ldr ${TMPFILES}

Release/%.doj: %.c 
	${CC} ${CFLAGS} -c $^ -o $@

Release/${PROGNAME}_basiccrt.doj: ${PROGNAME}_basiccrt.s
	${AS} ${PROGNAME}_basiccrt.s ${ASFLAG} -o Release/${PROGNAME}_basiccrt.doj 

Release/main.doj: main.c globdefs.h ffconf.h 
	@echo ".\main.c"
	${CC} ${CFLAGS} -c main.c  -o Release/main.doj


.PHONY : clean
