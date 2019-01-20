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

MINI_MODELS="Gladiola_Mini Gladiola_MiniLCD"
MINI_TOOLHEADS="Gladiola_SingleExtruder Albatross_Flexystruder Finch_Aerostruder AchemonSphinx_SmallLayer CecropiaSilk_SingleExtruderAeroV2 BandedTiger_HardenedSteel DingyCutworm_HardenedSteelPlus"

TAZ_MODELS="Juniper_TAZ5 Oliveoil_TAZ6"
TAZ_TOOLHEADS="Tilapia_SingleExtruder Kanyu_Flexystruder Opah_Moarstruder Javelin_DualExtruderV2 Longfin_FlexyDually Yellowfin_DualExtruderV3 Angelfish_Aerostruder AchemonSphinx_SmallLayer CecropiaSilk_SingleExtruderAeroV2 BandedTiger_HardenedSteel DingyCutworm_HardenedSteelPlus"

MINI2_MODELS="Hibiscus_Mini2"
MINI2_TOOLHEADS="AchemonSphinx_SmallLayer CecropiaSilk_SingleExtruderAeroV2 BandedTiger_HardenedSteel DingyCutworm_HardenedSteelPlus"

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
  echo
  exit
}

####
# compile_firmware <printer> <toolhead> [makeopts]
#
# Compiles firmware for the specified printer and toolhead
#
compile_firmware() {
  printer=$1  ; shift 1
  toolhead=$1 ; shift 1
  (cd Marlin; make clean; make AVR_TOOLS_PATH=${AVR_TOOLS_PATH}/ MODEL=${printer} TOOLHEAD=${toolhead} $*) || exit
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
  if [ $MAKE_HASHES ]; then
    generate_bare_checksum $1 $2
  fi
  echo
  echo Building for $1 and $2
  echo
  compile_firmware $1 $2
  if [ $MAKE_HASHES ]; then
    record_checksum Marlin/applet/*.hex build/md5sums-full
  fi
  mv Marlin/applet/*.hex build
  if [ $GENERATE_CONFIG ]; then
    build_config $1 $2
    mv Marlin/applet/*.config build
  fi
}

####
# check_tool <exec_name>
#
# Checks whether a tool exists in the AVR_TOOLS_PATH
#
check_tool() {
  if [ ! -x "$AVR_TOOLS_PATH/$1" ]; then
    echo Cannot locate $1 in $AVR_TOOLS_PATH.
    exit 1
  fi
}

####
# locate_avr_tools
#
# Attempts to locate the avr tools, otherwise prompts
# the user for a location.
#
locate_avr_tools() {
  AVR_OBJCOPY=`which avr-objcopy`
  if [ $? -eq 0 ]; then
    AVR_TOOLS_PATH=`dirname $AVR_OBJCOPY`
  fi
  while [ ! -x $AVR_TOOLS_PATH/avr-gcc ]
  do
    echo
    echo avr-gcc tools not found!
    echo
    read -p "Type path to avr-gcc tools: " AVR_TOOLS_PATH
    if [ -z $AVR_TOOLS_PATH ]; then
      echo Aborting.
      exit
    fi
  done
}

####
# check_avr_tools
#
# Verify that all the AVR tools we need exist in the located
# directory.
#
check_avr_tools() {
  echo
  echo Using $AVR_TOOLS_PATH for avr-gcc tools.
  echo

  check_tool avr-gcc
  check_tool avr-objcopy
  check_tool avr-g++
  check_tool avr-objdump
  check_tool avr-ar
  check_tool avr-size
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
    -*|--*)
      usage
      ;;
    *)
      break
      ;;
  esac
done

locate_avr_tools
check_avr_tools

rm -rf build
mkdir  build

if [ $# -eq 2 ]
then
  build_firmware $1 $2
else
  build_for_mini
  build_for_taz
fi

if [ $SHORTNAMES ]; then
  rename 's/Marlin_(.+)_(.+)_(.+)_(.+)_(.+)_(.+).hex/Marlin_$2_$4_$5_$6.hex/'       build/*
  rename 's/Marlin_(.+)_(.+)_(.+)_(.+)_(.+)_(.+).config/Marlin_$2_$4_$5_$6.config/' build/*
fi

build_summary
