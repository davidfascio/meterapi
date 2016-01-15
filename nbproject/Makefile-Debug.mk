#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin_4.x-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ComSerialInterface.o \
	${OBJECTDIR}/EventsEngine.o \
	${OBJECTDIR}/G155MeterInterface.o \
	${OBJECTDIR}/MeterCommons.o \
	${OBJECTDIR}/MeterControl.o \
	${OBJECTDIR}/MeterInterface.o \
	${OBJECTDIR}/Meters_Table.o \
	${OBJECTDIR}/ScorpioMeterInterface.o \
	${OBJECTDIR}/SystemQueue.o \
	${OBJECTDIR}/Utility.o \
	${OBJECTDIR}/handlerG155.o \
	${OBJECTDIR}/handlerSCORPIO.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/metersapi.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/metersapi.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/metersapi ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/ComSerialInterface.o: ComSerialInterface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ComSerialInterface.o ComSerialInterface.c

${OBJECTDIR}/EventsEngine.o: EventsEngine.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EventsEngine.o EventsEngine.c

${OBJECTDIR}/G155MeterInterface.o: G155MeterInterface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/G155MeterInterface.o G155MeterInterface.c

${OBJECTDIR}/MeterCommons.o: MeterCommons.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MeterCommons.o MeterCommons.c

${OBJECTDIR}/MeterControl.o: MeterControl.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MeterControl.o MeterControl.c

${OBJECTDIR}/MeterInterface.o: MeterInterface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MeterInterface.o MeterInterface.c

${OBJECTDIR}/Meters_Table.o: Meters_Table.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Meters_Table.o Meters_Table.c

${OBJECTDIR}/ScorpioMeterInterface.o: ScorpioMeterInterface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ScorpioMeterInterface.o ScorpioMeterInterface.c

${OBJECTDIR}/SystemQueue.o: SystemQueue.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SystemQueue.o SystemQueue.c

${OBJECTDIR}/Utility.o: Utility.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Utility.o Utility.c

${OBJECTDIR}/handlerG155.o: handlerG155.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/handlerG155.o handlerG155.c

${OBJECTDIR}/handlerSCORPIO.o: handlerSCORPIO.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/handlerSCORPIO.o handlerSCORPIO.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/metersapi.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
