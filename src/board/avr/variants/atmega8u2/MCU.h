/*

Copyright 2015-2020 Igor Petrovic

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#pragma once

#include <avr/io.h>

///
/// \brief Common defines for all variants based on this MCU.
/// @{

///
/// \brief Total available bytes for data in EEPROM.
///
#define EEPROM_SIZE 512

///
/// \brief Size of single flash page in bytes.
/// Used in bootloader mode when updating firmware.
///
#define BTLDR_FLASH_PAGE_SIZE SPM_PAGESIZE

/// @}