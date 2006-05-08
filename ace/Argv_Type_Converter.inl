// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ACE_TCHAR**
ACE_Argv_Type_Converter::get_TCHAR_argv (void)
{
#if defined (ACE_USES_WCHAR)
  return get_Wide_argv ();
#else
  return get_ASCII_argv ();
#endif
}

ACE_INLINE wchar_t**
ACE_Argv_Type_Converter::get_Wide_argv (void)
{
  if (this->char_passed_)
    {
      this->align_wchar_with_char ();
    }

  this->wchar_passed_ = true;
  return this->wchar_argv_;
}

ACE_INLINE char**
ACE_Argv_Type_Converter::get_ASCII_argv (void)
{
  if (this->wchar_passed_)
    {
      this->align_char_with_wchar ();
    }

  this->char_passed_ = true;

  return this->char_argv_;
}

ACE_INLINE int&
ACE_Argv_Type_Converter::get_argc (void)
{
  return this->saved_argc_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
