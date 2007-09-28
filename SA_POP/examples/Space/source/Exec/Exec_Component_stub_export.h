
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl EXEC_COMPONENT_STUB
// ------------------------------
#ifndef EXEC_COMPONENT_STUB_EXPORT_H
#define EXEC_COMPONENT_STUB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (EXEC_COMPONENT_STUB_HAS_DLL)
#  define EXEC_COMPONENT_STUB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && EXEC_COMPONENT_STUB_HAS_DLL */

#if !defined (EXEC_COMPONENT_STUB_HAS_DLL)
#  define EXEC_COMPONENT_STUB_HAS_DLL 1
#endif /* ! EXEC_COMPONENT_STUB_HAS_DLL */

#if defined (EXEC_COMPONENT_STUB_HAS_DLL) && (EXEC_COMPONENT_STUB_HAS_DLL == 1)
#  if defined (EXEC_COMPONENT_STUB_BUILD_DLL)
#    define EXEC_COMPONENT_STUB_Export ACE_Proper_Export_Flag
#    define EXEC_COMPONENT_STUB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define EXEC_COMPONENT_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* EXEC_COMPONENT_STUB_BUILD_DLL */
#    define EXEC_COMPONENT_STUB_Export ACE_Proper_Import_Flag
#    define EXEC_COMPONENT_STUB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define EXEC_COMPONENT_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* EXEC_COMPONENT_STUB_BUILD_DLL */
#else /* EXEC_COMPONENT_STUB_HAS_DLL == 1 */
#  define EXEC_COMPONENT_STUB_Export
#  define EXEC_COMPONENT_STUB_SINGLETON_DECLARATION(T)
#  define EXEC_COMPONENT_STUB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* EXEC_COMPONENT_STUB_HAS_DLL == 1 */

// Set EXEC_COMPONENT_STUB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (EXEC_COMPONENT_STUB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define EXEC_COMPONENT_STUB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define EXEC_COMPONENT_STUB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !EXEC_COMPONENT_STUB_NTRACE */

#if (EXEC_COMPONENT_STUB_NTRACE == 1)
#  define EXEC_COMPONENT_STUB_TRACE(X)
#else /* (EXEC_COMPONENT_STUB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define EXEC_COMPONENT_STUB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (EXEC_COMPONENT_STUB_NTRACE == 1) */

#endif /* EXEC_COMPONENT_STUB_EXPORT_H */

// End of auto generated file.
