#!/bin/sh

####
# Copyright (C) 2017  AlephObjects, Inc.
#
#
# The bash script in this page is free software: you can
# redistribute it and/or modify it under the terms of the GNU Affero
# General Public License (GNU AGPL) as published by the Free Software
# Foundation, either version 3 of the License, or (at your option)
# any later version.  The code is distributed WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU AGPL for more details.
#

####
# The following variables list the models and toolheads to build for:

UNIVERSAL_TOOLHEADS="AchemonSphinx_SmallLayer CecropiaSilk_SingleExtruderAeroV2 BandedTiger_HardenedSteel DingyCutworm_HardenedSteelPlus"

MINI_MODELS="Gladiola_Mini Gladiola_MiniLCD"
MINI_TOOLHEADS="Gladiola_SingleExtruder Albatross_Flexystruder Finch_Aerostruder $UNIVERSAL_TOOLHEADS"

TAZ_MODELS="Juniper_TAZ5 Oliveoil_TAZ6"
TAZ_TOOLHEADS="Tilapia_SingleExtruder Kanyu_Flexystruder Opah_Moarstruder Javelin_DualExtruderV2 Longfin_FlexyDually Yellowfin_DualExtruderV3 Angelfish_Aerostruder $UNIVERSAL_TOOLHEADS"

TAZ7_MODELS="Quiver_TAZPro"
TAZ7_TOOLHEADS="Quiver_DualExtruder $UNIVERSAL_TOOLHEADS"

MINI2_MODELS="Hibiscus_Mini2"
MINI2_TOOLHEADS="$UNIVERSAL_TOOLHEADS"

#TAZ7_MODELS="Quiver_TAZPro"
#TAZ7_TOOLHEADS="Angelfish_Aerostruder"

####
# usage
#
# Prints out a usage summary
#
usage() {
  echo
  echo "Usage: $0 [-s|--short-names] [-h|--hash] [-c|--config] [printer_model toolhead_name]"
  echo
  echo "   -s|--short-names  Omits LulzBot code names from generated .hex files"
  echo
  echo "   -h|--hash         Records md5sum of the .hex files. These files will be generated:"
  echo "                       md5sums-full*    Sum w/  embedded version str and timestamp"
  echo "                       md5sums-bare*    Sum w/o embedded version str and timestamp"
  echo
  echo "   -c|--config       Save the values of 'Configuration.h' and 'Configuration_adv.h'"
  echo "                     that are used for the specified printer and toolhead."
  echo "   -n|--dry-run      Just print commands, don't execute"
  echo
  exit
}

####
# compile_dependencies <printer>
#
# Compiles dependencies for the specific printer
#
compile_dependencies() {
  printer=$1   ; shift 1

  get_arch_info $printer

  case $printer in
    Quiver_TAZPro)
      ARCHIM_SRC="ArduinoAddons/arduino-1.8.5/packages/ultimachine/hardware/sam/1.6.9-b"
      (cd "$ARCHIM_SRC/system/libsam/build_gcc"; ARM_GCC_TOOLCHAIN="$gcc_path" make)
      cp -u $ARCHIM_SRC/variants/arduino_due_x/libsam_sam3x8e_gcc_rel.a     $ARCHIM_SRC/variants/archim/libsam_sam3x8e_gcc_rel.a
      cp -u $ARCHIM_SRC/variants/arduino_due_x/libsam_sam3x8e_gcc_rel.a.txt $ARCHIM_SRC/variants/archim/libsam_sam3x8e_gcc_rel.a.txt
      ;;
    *) ;;
  esac
}

####
# compile_firmware <printer> <toolhead> [makeopts]
#
# Compiles firmware for the specified printer and toolhead
#
get_arch_info() {
  printer=$1   ; shift 1
  case $printer in
    Quiver_TAZPro)
      gcc_path=$ARM_TOOLS_PATH
      format=bin
      ;;
    *)
      gcc_path=$AVR_TOOLS_PATH
      format=hex
      ;;
  esac
}

####
# compile_firmware <printer> <toolhead> [makeopts]
#
# Compiles firmware for the specified printer and toolhead
#
compile_firmware() {
  printer=$1   ; shift 1
  toolhead=$1  ; shift 1
  # Build the firmware
  (cd Marlin; make clean; make $MAKE_FLAGS AVR_TOOLS_PATH=${gcc_path}/ MODEL=${printer} TOOLHEAD=${toolhead} $*) || exit
}

####
# record_checksum <hex_file> <checksum-file-prefix>
#
# Records the md5sum of a hex file to the checksum file
#
record_checksum() {
  HEX_NAME=`basename $1`
  VERSION=`echo $HEX_NAME | sed -r "s/Marlin_(.+)_(.+)_(.+)_(.+)_(.+)_(.+).hex/\5-\6/"`
  VARIANT=`echo $HEX_NAME | sed -r "s/Marlin_(.+)_(.+)_(.+)_(.+)_(.+)_(.+).hex/\1_\2 \3_\4/"`
  cat $1 | md5sum | sed "s/-/$VARIANT/" >> ${2}-${VERSION}.txt
}

####
# generate_bare_checksum <printer> <toolhead>
#
# Builds firmware without timestamp and version strings, and
# saves md5sum to a file. These are useful to see if anything
# actually changed between two versions.
#
generate_bare_checksum() {
  echo
  echo Generating bare checksum for $1 and $2
  echo
  compile_firmware $1 $2 NO_TIMESTAMP=1 NO_VERSION=1
  record_checksum Marlin/applet/*.hex build/md5sums-bare
}

####
# build_config <printer> <toolhead>
#
# Compiles Configuration.h and Configuration_adv.h for the specified printer and toolhead
#
build_config() {
  echo
  echo Generating config for $1 and $2
  echo
  compile_firmware $1 $2 config
}

####
# build_firmware <printer> <toolhead> <dest-dir>
#
# Compiles firmware for the specified printer and toolhead
#
build_firmware() {
  printer=$1   ; shift 1
  toolhead=$1  ; shift 1
  get_arch_info $printer
  if [ $MAKE_HASHES ]; then
    generate_bare_checksum $printer $toolhead
  fi
  echo
  echo Building for $printer and $toolhead
  echo
  compile_firmware $printer $toolhead
  if [ $MAKE_HASHES ]; then
    record_checksum Marlin/applet/*.hex build/md5sums-full
  fi
  if [ $DRY_RUN ]; then
    return
  fi
  mv Marlin/applet/*.$format build
  chmod a-x build/*
  if [ $GENERATE_CONFIG ]; then
    build_config $printer $toolhead
    mv Marlin/applet/*.config build
  fi
}

####
# check_tool <path> <exec_name>
#
# Checks whether a tool exists in path
#
check_tool() {
  if [ ! -x "$1/$2" ]; then
    echo Cannot locate $2 in $1.
    exit 1
  fi
}

####
# locate_tools <path_var> <prefix>
#
# Attempts to locate a tool, otherwise prompts
# the user for a location. The found path is
# stored in <path_var>
#
locate_tools() {
  DEST_VAR=$1
  TOOL_BINARY=`which $2-objcopy`
  if [ $? -eq 0 ]; then
    TOOLS_PATH=`dirname $TOOL_BINARY`
  fi
  while [ ! -x $TOOLS_PATH/$2-objcopy ]
  do
    echo
    echo $2-objcopy not found!
    echo
    read -p "Type path to $2 tools: " TOOLS_PATH
    if [ -z $TOOLS_PATH ]; then
      echo Aborting.
      exit
    fi
  done
  eval "$DEST_VAR=$TOOLS_PATH"
}

####
# check_tools <path> <prefix>
#
# Verify that all the AVR tools we need exist in the located
# directory.
#
check_tools() {
  echo
  echo Using $1 for $2 tools.
  echo

  check_tool $1 $2-gcc
  check_tool $1 $2-objcopy
  check_tool $1 $2-g++
  check_tool $1 $2-objdump
  check_tool $1 $2-ar
  check_tool $1 $2-size
}

####
# build_for_mini
#
# Build all the toolhead variants for the mini
#
build_for_mini() {
  for model in $MINI_MODELS
  do
    for toolhead in $MINI_TOOLHEADS
    do
      build_firmware ${model} ${toolhead}
    done
  done
  for model in $MINI2_MODELS
  do
    for toolhead in $MINI2_TOOLHEADS
    do
      build_firmware ${model} ${toolhead}
    done
  done
}

####
# build_for_taz
#
# Build all the toolhead variants for the TAZ
#
build_for_taz() {
  for model in $TAZ_MODELS
  do
    for toolhead in $TAZ_TOOLHEADS
    do
      build_firmware ${model} ${toolhead}
    done
  done
  for model in $TAZ7_MODELS
  do
    for toolhead in $TAZ7_TOOLHEADS
    do
      build_firmware ${model} ${toolhead}
    done
  done
}

####
# build_summary
#
# Print out a summary of hex files that were created
#
build_summary() {
  echo
  echo
  echo
  echo Generated files stored in "`pwd`/build":
  echo
  ls build
  echo
}

############################################
# MAIN SCRIPT
############################################

while true
do
  case $1 in
    -h|--hash)
      MAKE_HASHES=1
      shift
      ;;
    -s|--short-names)
      SHORTNAMES=1
      shift
      ;;
    -c|--config)
      GENERATE_CONFIG=1
      shift
      ;;
    -n|--dry-run)
      MAKE_FLAGS=-n
      DRY_RUN=1
      shift
      ;;
    -*|--*)
      usage
      ;;
    *)
      break
      ;;
  esac
done

locate_tools AVR_TOOLS_PATH avr
locate_tools ARM_TOOLS_PATH arm-none-eabi

check_tools $AVR_TOOLS_PATH avr
check_tools $ARM_TOOLS_PATH arm-none-eabi

MAKE_FLAGS="$MAKE_FLAGS -j $(grep -c ^processor /proc/cpuinfo)"

rm -rf build
mkdir  build

if [ $# -eq 2 ]
then
  compile_dependencies $1
  build_firmware $1 $2
else
  compile_dependencies Quiver_TAZPro
  build_for_mini
  build_for_taz
fi

if [ $SHORTNAMES ]; then
  rename 's/Marlin_(.+)_(.+)_(.+)_(.+)_(.+)_(.+).hex/Marlin_$2_$4_$5_$6.hex/'       build/*
  rename 's/Marlin_(.+)_(.+)_(.+)_(.+)_(.+)_(.+).config/Marlin_$2_$4_$5_$6.config/' build/*
fi

build_summary
