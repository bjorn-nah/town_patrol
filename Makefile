# Include paths we may need for compilations
ifndef PREFIX
	PREFIX=/usr/share
endif
ifeq ($(CC65_HOME),)
	CC65_HOME=$(PREFIX)/cc65
endif
CC65_INC=$(CC65_HOME)/include
CC65_LIB=$(CC65_HOME)/lib
CA65_INC=$(CC65_HOME)/asminc

# Compiling for Atari Lynx system
SYS=lynx

# Names of tools
CO=co65
CC=cc65
AS=ca65
AR=ar65
CL=cl65
# adapt to your sprite packer folder:
SPRPCK=..\sprite_packer\sprpck
CP=cp
RM=del
ECHO=echo

CODE_SEGMENT=CODE
DATA_SEGMENT=DATA
RODATA_SEGMENT=RODATA
BSS_SEGMENT=BSS
 
SEGMENTS=--code-name $(CODE_SEGMENT) \
				--rodata-name $(RODATA_SEGMENT) \
				--bss-name $(BSS_SEGMENT) \
				--data-name $(DATA_SEGMENT)

# Flag for assembler
AFLAGS=-I $(CA65_INC) -t $(SYS)

# Flags for C-code compiler
CFLAGS=-I $(CC65_INC) -t $(SYS) --add-source -O -Or -Cl -Os

# name of your product:
target = town_patrol
# adpat to your resourses
objects = main.o title_screen.o game.o result_screen.o\
	soundbs.o idonno.o\
	title01.o title02.o sky03.o\
	sky02.o shoot01.o\
	hero_run01.o hero_run02.o hero_run03.o hero_run04.o hero_shr01.o hero_shr02.o hero_shr03.o hero_shr04.o hero_shu01.o hero_shu02.o hero_shu03.o hero_shu04.o\
	enemy_fly01.o enemy_fly02.o enemy_fly03.o enemy_fly04.o missile00.o\
	enemy_run01.o enemy_run02.o enemy_run03.o enemy_run04.o enemy_run11.o enemy_run12.o enemy_run13.o\
	explode00.o explode01.o explode02.o explode03.o explode04.o explode05.o explode06.o explode07.o explode08.o explode09.o\
	explode10.o explode11.o explode12.o explode13.o explode14.o explode15.o explode16.o explode17.o explode18.o explode19.o\
	explode20.o explode21.o explode22.o explode23.o explode24.o explode25.o explode26.o explode27.o explode28.o explode29.o\
	skyline01.o skyline02.o skyline03.o skyline04.o skyline05.o skyline06.o skyline07.o skyline11.o skyline12.o skyline13.o\
	city01.o city02.o city03.o city04.o


$(target) : $(objects)
	$(CL) -t $(SYS) -o $@.lnx -m lynxjam2024.map $(objects) lynx.lib
	$(CL) -t $(SYS) --config lynx-bll.cfg -o $@.o -m lynxjam2024.map $(objects) lynx.lib 
	

.SUFFIXES: .o .c .s .asm .bmp

# Rule for making a *.o file out of a *.c file
%.o: %.c
	$(CC) $(CFLAGS) $(SEGMENTS) -o $(patsubst %c, %s, $(notdir $<)) $<
	$(AS) -o $@ $(AFLAGS) $(*).s
	$(RM) $*.s

# Rule for making a *.o file out of a *.s or *.asm file
%.o: %.s
	$(AS) -o $@ $(AFLAGS) $<
%.o: %.asm
	$(AS) -o $@ $(AFLAGS) $<

# Rule for making a *.o file out of a *.bmp file with sprite offset
%_spr.o : %_spr.bmp
	# adapts to your needs : 
	$(SPRPCK) -t6 -p0 $<
	$(ECHO) .global _$*_spr > $*_spr.s
	$(ECHO) .segment "$(RODATA_SEGMENT)" >> $*_spr.s
	$(ECHO) _$*_spr: .incbin "$*_spr.spr" >> $*_spr.s
	$(AS) -o $@ $(AFLAGS) $*_spr.s
	$(RM) $*_spr.s
	$(RM) $*_spr.pal
	$(RM) $*_spr.spr

# Rule for making a *.o file out of a *.bmp file
%.o : %.bmp
	$(SPRPCK) -t6 -p2 $<
	$(ECHO) .global _$* > $*.s
	$(ECHO) .segment "$(RODATA_SEGMENT)" >> $*.s
	$(ECHO) _$*: .incbin "$*.spr" >> $*.s
	$(AS) -o $@ $(AFLAGS) $*.s
	$(RM) $*.s
	$(RM) $*.pal
	$(RM) $*.spr

# Rule for making a *.o file out of a *.pcx file
%.o : %.pcx
	$(SPRPCK) -t3 -p2 $<
	$(ECHO) .global _$* > $*.s
	$(ECHO) .segment "$(RODATA_SEGMENT)" >> $*.s
	$(ECHO) _$*: .incbin "$*.spr" >> $*.s
	$(AS) -o $@ $(AFLAGS) $*.s
	$(RM) $*.s
	$(RM) $*.pal
	$(RM) $*.spr

hero_%.o : hero_%.pcx
	$(SPRPCK) -t3 -p2 -a013010	 $<
	$(ECHO) .global _hero_$* > hero_$*.s
	$(ECHO) .segment "$(RODATA_SEGMENT)" >> hero_$*.s
	$(ECHO) _hero_$*: .incbin "hero_$*.spr" >> hero_$*.s
	$(AS) -o $@ $(AFLAGS) hero_$*.s
	$(RM) hero_$*.s
	$(RM) hero_$*.pal
	$(RM) hero_$*.spr

enemy_fly%.o : enemy_fly%.pcx
	$(SPRPCK) -t3 -p2 -a007009 $<
	$(ECHO) .global _enemy_fly$* > enemy_fly$*.s
	$(ECHO) .segment "$(RODATA_SEGMENT)" >> enemy_fly$*.s
	$(ECHO) _enemy_fly$*: .incbin "enemy_fly$*.spr" >> enemy_fly$*.s
	$(AS) -o $@ $(AFLAGS) enemy_fly$*.s
	$(RM) enemy_fly$*.s
	$(RM) enemy_fly$*.pal
	$(RM) enemy_fly$*.spr
	
enemy_run%.o : enemy_run%.pcx
	$(SPRPCK) -t3 -p2 -a008010 $<
	$(ECHO) .global _enemy_run$* > enemy_run$*.s
	$(ECHO) .segment "$(RODATA_SEGMENT)" >> enemy_run$*.s
	$(ECHO) _enemy_run$*: .incbin "enemy_run$*.spr" >> enemy_run$*.s
	$(AS) -o $@ $(AFLAGS) enemy_run$*.s
	$(RM) enemy_run$*.s
	$(RM) enemy_run$*.pal
	$(RM) enemy_run$*.spr
	
shoot%.o : shoot%.pcx
	$(SPRPCK) -t3 -p2 -a003003 $<
	$(ECHO) .global _shoot$* > shoot$*.s
	$(ECHO) .segment "$(RODATA_SEGMENT)" >> shoot$*.s
	$(ECHO) _shoot$*: .incbin "shoot$*.spr" >> shoot$*.s
	$(AS) -o $@ $(AFLAGS) shoot$*.s
	$(RM) shoot$*.s
	$(RM) shoot$*.pal
	$(RM) shoot$*.spr
	
explode%.o : explode%.pcx
	$(SPRPCK) -t3 -p2 -a016016 $<
	$(ECHO) .global _explode$* > explode$*.s
	$(ECHO) .segment "$(RODATA_SEGMENT)" >> explode$*.s
	$(ECHO) _explode$*: .incbin "explode$*.spr" >> explode$*.s
	$(AS) -o $@ $(AFLAGS) explode$*.s
	$(RM) explode$*.s
	$(RM) explode$*.pal
	$(RM) explode$*.spr
	
missile%.o : missile%.pcx
	$(SPRPCK) -t3 -p2 -a003004 $<
	$(ECHO) .global _missile$* > missile$*.s
	$(ECHO) .segment "$(RODATA_SEGMENT)" >> missile$*.s
	$(ECHO) _missile$*: .incbin "missile$*.spr" >> missile$*.s
	$(AS) -o $@ $(AFLAGS) missile$*.s
	$(RM) missile$*.s
	$(RM) missile$*.pal
	$(RM) missile$*.spr


lynx-stdjoy.o:
	$(CP) $(CC65_HOME)/target/$(SYS)/drv/joy/$*.joy .
	$(CO) --code-label _$(SYS)joy $*.joy
	$(AS) -o $@ $(AFLAGS) $*.s
	$(RM) $*.joy
	$(RM) $*.s

lynx-160-102-16.o:
	$(CP) $(CC65_HOME)/target/$(SYS)/drv/tgi/$*.tgi .
	$(CO) --code-label _$(SYS)tgi -o $*.s $*.tgi
	$(AS) -o $@ $(AFLAGS) $*.s
	$(RM) $*.tgi
	$(RM) $*.s

all: $(target)

clean :
	$(RM) $(objects)
	$(RM) $(target)
	$(RM) *.spr
	$(RM) *.tgi
	$(RM) *.joy
#	$(RM) *.s
	$(RM) *.pal
	$(RM) *.lnx
	$(RM) *.o