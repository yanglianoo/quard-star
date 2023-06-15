/** @file
  Module entry point library for STANDALONE MM core.

Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2016 - 2018, ARM Limited. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __MODULE_ENTRY_POINT_H__
#define __MODULE_ENTRY_POINT_H__

///
/// Global variable that contains a pointer to the Hob List passed into the STANDALONE MM Core entry point.
///
extern VOID  *gHobList;

/**
  The entry point of PE/COFF Image for the STANDALONE MM Core.

  This function is the entry point for the STANDALONE MM Core. This function is required to call
  ProcessModuleEntryPointList() and ProcessModuleEntryPointList() is never expected to return.
  The STANDALONE MM Core is responsible for calling ProcessLibraryConstructorList() as soon as the EFI
  System Table and the image handle for the STANDALONE MM Core itself have been established.
  If ProcessModuleEntryPointList() returns, then ASSERT() and halt the system.

  @param  HobStart  Pointer to the beginning of the HOB List passed in from the PEI Phase.

**/
VOID
EFIAPI
_ModuleEntryPoint (
  IN VOID  *HobStart
  );

/**
  Required by the EBC compiler and identical in functionality to _ModuleEntryPoint().

  This function is required to call _ModuleEntryPoint() passing in HobStart.

  @param  HobStart  Pointer to the beginning of the HOB List passed in from the PEI Phase.

**/
VOID
EFIAPI
EfiMain (
  IN VOID  *HobStart
  );

/**
  Auto generated function that calls the library constructors for all of the module's dependent libraries.

  This function must be called by _ModuleEntryPoint().
  This function calls the set of library constructors for the set of library instances
  that a module depends on.  This includes library instances that a module depends on
  directly and library instances that a module depends on indirectly through other
  libraries. This function is auto generated by build tools and those build tools are
  responsible for collecting the set of library instances, determine which ones have
  constructors, and calling the library constructors in the proper order based upon
  each of the library instances own dependencies.

  @param  ImageHandle  The image handle of the STANDALONE MM Core.
  @param  SystemTable  A pointer to the EFI System Table.

**/
VOID
EFIAPI
ProcessLibraryConstructorList (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_MM_SYSTEM_TABLE  *MmSystemTable
  );

/**
  Autogenerated function that calls a set of module entry points.

  This function must be called by _ModuleEntryPoint().
  This function calls the set of module entry points.
  This function is auto generated by build tools and those build tools are responsible
  for collecting the module entry points and calling them in a specified order.

  @param  HobStart  Pointer to the beginning of the HOB List passed in from the PEI Phase.

**/
VOID
EFIAPI
ProcessModuleEntryPointList (
  IN VOID  *HobStart
  );

#endif
