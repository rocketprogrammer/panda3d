// Filename: config_nametag.h
// Created by:  drose (19Jul01)
//
////////////////////////////////////////////////////////////////////

#ifndef CONFIG_NAMETAG_H
#define CONFIG_NAMETAG_H

#include "otpbase.h"
#include "notifyCategoryProxy.h"
#include "configVariableString.h"
#include "configVariableBool.h"

#define NAMETAG_REFCOUNT_HACK \
  INLINE virtual int get_ref_count() const final { return ReferenceCount::get_ref_count(); }; \
  INLINE virtual void ref() const final { ReferenceCount::ref(); }; \
  INLINE virtual bool unref() const final { return ReferenceCount::unref(); };

NotifyCategoryDecl(nametag, EXPCL_OTP, EXPTP_OTP);

extern ConfigVariableString nametag_fixed_bin;

extern EXPCL_OTP void init_libnametag();

#endif
