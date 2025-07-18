# SPDX-License-Identifier: MIT
# Copyright (c) 2023-2024 Advanced Micro Devices, Inc.  All rights reserved.

if("${proc_compiler}" STREQUAL "armcc")
add_subdirectory(armcc)
elseif(NOT ("${proc_compiler}" STREQUAL "iccarm.exe"))
add_subdirectory(gcc)
endif()

collect (PROJECT_LIB_HEADERS smc.h)
collect (PROJECT_LIB_SOURCES xil_cache.c)
collect (PROJECT_LIB_HEADERS xil_cache.h)
collect (PROJECT_LIB_HEADERS xil_cache_l.h)
collect (PROJECT_LIB_HEADERS xil_errata.h)
collect (PROJECT_LIB_SOURCES xil_misc_psreset_api.c)
collect (PROJECT_LIB_HEADERS xil_misc_psreset_api.h)
collect (PROJECT_LIB_SOURCES xil_mmu.c)
collect (PROJECT_LIB_HEADERS xil_mmu.h)
collect (PROJECT_LIB_HEADERS xl2cc.h)
collect (PROJECT_LIB_SOURCES xl2cc_counter.c)
collect (PROJECT_LIB_HEADERS xl2cc_counter.h)
collect (PROJECT_LIB_HEADERS xparameters_ps.h)
collect (PROJECT_LIB_HEADERS xpm_counter.h)
collect (PROJECT_LIB_HEADERS xpseudo_asm.h)
collect (PROJECT_LIB_HEADERS xreg_cortexa9.h)
collect (PROJECT_LIB_HEADERS xcortexa9.h)
collect (PROJECT_LIB_HEADERS xcortexa9_config.h)
