<#
.Synopsis
   Wrapper for .v20 files to be launched in VICE VIC-20 emulator.
.DESCRIPTION
   This file will extract ROMs from a .v20 file and launch the emulator with apropriate command line switches.
.EXAMPLE
   powershell.exe -file v20_loader.ps1 MyRomFile.v20
.NOTES
   This file will extract ROMs from the .v20 file to your default temp directory.
   Recommended usage is to associate this script with the .v20 file extention.
   Run this script without parameters to create the association automatically.   

   ---[ Instructions for dummies: ]---
   
   1. Copy this script into your VICE directory (next to the xvic.exe file).
   2. Right click the script file and choose "Run with Powershell".
   3. Press enter to acknowledge the message about file association creation.
   4. Double click your .v20 file to launch them in VICE emulator.

#>


# The maximum version of the .v20 file fomat supported by this script.
$maximumSupportedVersion=1

# The path to the current script.
$scriptPath = split-path -parent $MyInvocation.MyCommand.Definition

# VICE emulator is expected to be found in the same folder as the script.
$emulatorPath = "$scriptPath\xvic.exe"
$emulatorParam = ""

# Check that VICE emulator is in the script folder.
if(-not(Test-Path $emulatorPath)){
    Write-Host -ForegroundColor red "The vice emulator (xvic.exe) was not found. Move this script to your VICE emulator folder and try again."
    Read-Host -Prompt "Press enter to continue."
    exit
}

# If no v20 file was supplied on the command line, install the file association, and exit.
if($args.Count -eq 0){
    New-Item -Path HKCU:\SOFTWARE\Classes\.v20 -Force | Out-Null
    New-ItemProperty -path HKCU:\SOFTWARE\Classes\.v20 -Name "(default)" -PropertyType String -Value v20_loader | Out-Null
 
    New-Item -Path HKCU:\SOFTWARE\Classes\v20_loader\shell\open\command -Force | Out-Null
    New-ItemProperty -path HKCU:\SOFTWARE\Classes\v20_loader\shell\open\command -Name "(default)" -PropertyType String -Value "powershell.exe -file `"C:\Users\k\Downloads\GTK3VICE 3.4\v20_loader.ps1`" `"%1`"" | Out-Null
 
    New-Item -Path HKCU:\SOFTWARE\Classes\v20_loader\DefaultIcon -Force | Out-Null
    New-ItemProperty -path HKCU:\SOFTWARE\Classes\v20_loader\DefaultIcon -Name "(default)" -PropertyType String -Value "$emulatorPath,0" | Out-Null
 
    Write-Host "File association installed. Double click on a .v20 file to load it in the VICE emulator."
    Read-Host -Prompt "Press enter to continue" | Out-Null
    exit
}

# Check if the supplied ROM file exists
$romPath = $args[0]
$romPat = "C:\Users\k\Downloads\example roms\Ms.PacMan.v20"
if(-not(Test-Path $romPath)){
    Write-Host -ForegroundColor red "Can not find $romPath."
    Read-Host -Prompt "Press enter to continue" | Out-Null
    exit
}
Write-Host "v20 path: $romPath" 


# Read the .v20 file as byte array
$bytes = Get-Content $romPath -Encoding Byte -ReadCount 0
Write-Host "$($bytes.Length) bytes read."

# Test the "magic bytes" in the beginning of the .v20 files.
if([System.Text.Encoding]::ASCII.GetString($bytes[0..6]) -ne "VIC-20v"){
    Write-Host -ForegroundColor red "Supplied file is not a correctly formatted v20 file."
    Read-Host -Prompt "Press enter to continue" | Out-Null
    exit
}

# Verify the version of the v20 file.
if($bytes[7] -gt $maximumSupportedVersion) {
    Write-Host -ForegroundColor Yellow "WARNING: Your ROM uses a newer version of the v20 format than this script supports, we will attempt to load as normal: some features may not work or the load may fail all together."
    Read-Host -Prompt "Press enter to continue" | Out-Null
}

# The rom name
$romName = [System.Text.Encoding]::ASCII.GetString($bytes[8..39]).Trim([char]0)
if($romName -ne ""){
    Write-Host "Rom name: $romName."
} else {
    Write-Host "This is an unnamed ROM."
}

# Auto Exec information    
$autoExec = [System.Text.Encoding]::ASCII.GetString($bytes[40..49]).Trim([char]0)
if ($autoExec -ne ""){
    Write-Host "AUTO_EXEC information: $autoExec"
    $emulatorParam +=  " -keybuf `"$($autoExec.ToLower())`""
}else{
    Write-Host "No AUTO_EXEC information in this file."
}

# Parse video block
$Video = $bytes[51]
if ($Video -eq 1){
    $emulatorParam += " -pal"
}else{
    $emulatorParam += " -ntsc"
}
   
#Parse file structure
$map = $bytes[50]

$len2=0
$len4=0
$len6=0
$lenA=0
$lenB=0

$offset2=0
$offset4=0
$offset6=0
$offsetA=0
$offsetB=0

if($map -band 1){
    $len2=1
    if($map -band 2) { $len2=2 }
}

$offset4 = $len2

if($map -band 4){
    $len4=1
    if($map -band 8) { $len4=2 } 
}

$offset6 = $len4 + $offset4

if($map -band 16){
    $len6=1
    if($map -band 32) { $len6=2 }
}

$offsetA = $len6 + $offset6

if($map -band 64) { $lenA=1}

$offsetB = $lenA + $offsetA

if($map -band 128) { $lenB=1}


# Change current directory to Temp
Push-Location $env:TEMP
Write-Host "Current directory: $(get-location)"
    
# Extract ROM files from the v20 file.

if($len2 -ne 0) {
    $from = 64 + $offset2 * 4096
    $to = $from + $len2 * 4096 - 1
    Set-Content rom.20 -Value $bytes[$from..$to]  -Encoding Byte
    $emulatorParam += " -cart2 rom.20"
}

if($len4 -ne 0) {
    $from = 64 + $offset4 * 4096
    $to = $from + $len4 * 4096 - 1
    Set-Content rom.40 -Value $bytes[$from..$to]  -Encoding Byte
    $emulatorParam += " -cart4 rom.40"
}

if($len6 -ne 0) {
    $from = 64 + $offset6 * 4096
    $to = $from + $len6 * 4096 - 1
    Set-Content "rom.60" -Value $bytes[$from..$to]  -Encoding Byte
    $emulatorParam += " -cart6 rom.60"
}

if($lenA -ne 0) {
    $from = 64 + $offsetA * 4096
    $to = $from + $lenA * 4096 - 1
    Set-Content "rom.a0" -Value $bytes[$from..$to]  -Encoding Byte
    $emulatorParam += " -cartA rom.a0"
}

if($lenB -ne 0) {
    $from = 64 + $offsetB * 4096
    $to = $from + $lenB * 4096 - 1
    Set-Content "rom.b0" -Value $bytes[$from..$to]  -Encoding Byte
    $emulatorParam += " -cartB rom.b0"
}

Write-Host "Emulator path: $emulatorPath"
Write-Host "Emulator parameters: $emulatorParam"
Write-Host "Executing emulator, please wait." 
& "$EmulatorPath" $emulatorParam.Split(" ")
sleep 5

Pop-Location

