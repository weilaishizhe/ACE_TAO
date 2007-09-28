
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl FILTER_COMPONENT_SVNT
// ------------------------------
#ifndef FILTER_COMPONENT_SVNT_EXPORT_H
#define FILTER_COMPONENT_SVNT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (FILTER_COMPONENT_SVNT_HAS_DLL)
#  define FILTER_COMPONENT_SVNT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && FILTER_COMPONENT_SVNT_HAS_DLL */

#if !defined (FILTER_COMPONENT_SVNT_HAS_DLL)
#  define FILTER_COMPONENT_SVNT_HAS_DLL 1
#endif /* ! FILTER_COMPONENT_SVNT_HAS_DLL */

#if defined (FILTER_COMPONENT_SVNT_HAS_DLL) && (FILTER_COMPONENT_SVNT_HAS_DLL == 1)
#  if defined (FILTER_COMPONENT_SVNT_BUILD_DLL)
#    define FILTER_COMPONENT_SVNT_Export ACE_Proper_Export_Flag
#    define FILTER_COMPONENT_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define FILTER_COMPONENT_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* FILTER_COMPONENT_SVNT_BUILD_DLL */
#    define FILTER_COMPONENT_SVNT_Export ACE_Proper_Import_Flag
#    define FILTER_COMPONENT_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define FILTER_COMPONENT_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* FILTER_COMPONENT_SVNT_BUILD_DLL */
#else /* FILTER_COMPONENT_SVNT_HAS_DLL == 1 */
#  define FILTER_COMPONENT_SVNT_Export
#  define FILTER_COMPONENT_SVNT_SINGLETON_DECLARATION(T)
#  define FILTER_COMPONENT_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* FILTER_COMPONENT_SVNT_HAS_DLL == 1 */

// Set FILTER_COMPONENT_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (FILTER_COMPONENT_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define FILTER_COMPONENT_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define FILTER_COMPONENT_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !FILTER_COMPONENT_SVNT_NTRACE */

#if (FILTER_COMPONENT_SVNT_NTRACE == 1)
#  define FILTER_COMPONENT_SVNT_TRACE(X)
#else /* (FILTER_COMPONENT_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define FILTER_COMPONENT_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (FILTER_COMPONENT_SVNT_NTRACE == 1) */

#endif /* FILTER_COMPONENT_SVNT_EXPORT_H */

// End of auto generated file.
