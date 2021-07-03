// Filename: toontownsymbols.h
// Created by:  drose (18Feb00)
/////////////////////////////////////////////

#ifndef TOONTOWNSYMBOLS_H
#define TOONTOWNSYMBOLS_H

/* See dtoolsymbols.h for a rant on the purpose of this file.  */

#ifdef BUILDING_TOONTOWN
  #define EXPCL_TOONTOWN EXPORT_CLASS
  #define EXPTP_TOONTOWN EXPORT_TEMPL
#else
  #define EXPCL_TOONTOWN IMPORT_CLASS
  #define EXPTP_TOONTOWN IMPORT_TEMPL
#endif

#endif
