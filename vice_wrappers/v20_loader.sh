#!/bin/bash

#V20 LAUNCHER SCRIPT VERSION 1.0
#By Aubrey MCLEOD

MAXIMUM_SUPPORTED_VERSION=1

EMULATOR="/Applications/Vice/bin/xvic"
#EMULATOR="PATH_TO_EMULATOR" #update this with the path to your xVic executable.
ROM="$1"

TEMP_DIR_NAME=".temp_v20"
WORKING_DIRECTORY="$(dirname "${ROM}")"

RAW_BYTES=$(xxd -p "$ROM" | tr -d '\n')

#Test that we are dealing with the correct file type.
if [[ "$(echo ${RAW_BYTES:0:14} | xxd -r -p)" == "VIC-20v" ]]; then
  if [[ "${RAW_BYTES:14:2}" > "$MAXIMUM_SUPPORTED_VERSION" ]]; then
    echo "WARNING: Your ROM uses a newer version of the v20 format than this script supports, we will attempt to load as normal: some features may not work or the load may fail all together."
  fi

  #Parse name block
  ROM_NAME="$(echo ${RAW_BYTES:16:64} | xxd -r -p)"
  if [[ $ROM_NAME ]]; then
    echo "Attempting to load" "${ROM_NAME}"
  else
    echo "Attempting to load unnamed ROM."
  fi

  #Parse autoexec block
  AE="$(echo ${RAW_BYTES:80:20} | xxd -r -p)"
  if [[ $AE ]]; then
    echo "Found AUTO_EXEC:" "${AE}"
    ae_flag[0]="-keybuf"
    ae_flag[1]="$(echo "${AE}" | tr '[:upper:]' '[:lower:]')\n"
  else
    echo "No AUTO_EXEC found..."
  fi

  #parse video block
  VIDEO="${RAW_BYTES:102:2}"
  if [[ "$VIDEO" == "01" ]]; then
    video_flag="-pal"
  else
    video_flag="-ntsc"
  fi

  #parse file structure
  MAP=$(printf %d \'$(echo ${RAW_BYTES:100:2} | xxd -r -p))

  len2=0
  len4=0
  len6=0
  lenA=0
  lenB=0

  offset2=0
  offset4=0
  offset6=0
  offsetA=0
  offsetB=0


  if (( $MAP & 1 )); then
    len2=1
    if (( $MAP & 2 )); then
      len2=2
    fi
  fi

  offset4=$len2

  if (( $MAP & 4 )); then
    len4=1
    if (( $MAP & 8 )); then
      len4=2
    fi
  fi

  offset6=$(($len4+$offset4))

  if (( $MAP & 16 )); then
    len6=1
    if (( $MAP & 32 )); then
      len6=2
    fi
  fi

  offsetA=$(($len6+$offset6))

  if (( $MAP & 64 )); then
    lenA=1
  fi

  offsetB=$(($lenA+$offsetA))

  if (( $MAP & 128 )); then
    lenB=1
  fi
  
  #make files
  mkdir "$WORKING_DIRECTORY/$TEMP_DIR_NAME"
  cd "$WORKING_DIRECTORY/$TEMP_DIR_NAME"
  
  echo "DEBUG pwd: $PWD"

  if [[ $len2 != 0 ]]; then
    echo "${RAW_BYTES:$(( 128 + offset2 * (4096*2) )):$(( len2*4096*2))}" | xxd -r -p - rom.20
    cart2[0]="-cart2"
    cart2[1]="$PWD/rom.20"
  fi

  if [[ $len4 != 0 ]]; then
    echo "${RAW_BYTES:$(( 128 + offset4 * (4096*2) )):$(( len4*4096*2))}" | xxd -r -p - rom.40
    cart4[0]="-cart4"
    cart4[1]="$PWD/rom.40"
  fi

  if [[ $len6 != 0 ]]; then
    echo "${RAW_BYTES:$(( 128 + offset6 * (4096*2) )):$(( len6*4096*2))}" | xxd -r -p - rom.60
    cart6[0]="-cart6"
    cart6[1]="$PWD/rom.60"
  fi

  if [[ $lenA != 0 ]]; then
    echo "${RAW_BYTES:$(( 128 + offsetA * (4096*2) )):$(( lenA*4096*2))}" | xxd -r -p - rom.a0
    cartA[0]="-cartA"
    cartA[1]="$PWD/rom.a0"
  fi

  if [[ $lenB != 0 ]]; then
    echo "${RAW_BYTES:$(( 128 + offsetB * (4096*2) )):$(( lenB*4096*2))}" | xxd -r -p - rom.b0
    cartB[0]="-cartB"
    cartB[1]="$PWD/rom.b0"
  fi


  #INIT EMULATOR
  "$EMULATOR" "${ae_flag[@]}" "${video_flag}" "${cart2[@]}" "${cart4[@]}" "${cart6[@]}" "${cartA[@]}" "${cartB[@]}"

  #cleanup
  cd ..
  rm -r $TEMP_DIR_NAME


else
  echo "Supplied file is not a correctly formatted v20 file."
fi