// Filename: otpsymbols.h
// Created by:  drose (18Feb00)
/////////////////////////////////////////////

#ifndef OTPSYMBOLS_H
#define OTPSYMBOLS_H

/* See dtoolsymbols.h for a rant on the purpose of this file.  */

#ifdef BUILDING_OTP
  #define EXPCL_OTP EXPORT_CLASS
  #define EXPTP_OTP EXPORT_TEMPL
#else
  #define EXPCL_OTP IMPORT_CLASS
  #define EXPTP_OTP IMPORT_TEMPL
#endif

#endif
