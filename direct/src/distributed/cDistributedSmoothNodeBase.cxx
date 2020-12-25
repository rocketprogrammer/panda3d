// Filename: cDistributedSmoothNodeBase.cxx
// Created by:  drose (03Sep04)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#include "cDistributedSmoothNodeBase.h"
#include "cConnectionRepository.h"
#include "dcField.h"
#include "dcClass.h"
#include "dcmsgtypes.h"
#include "config_distributed.h"

#ifdef HAVE_PYTHON
#include "py_panda.h"
#endif

static const PN_stdfloat smooth_node_epsilon = 0.01;
static const double network_time_precision = 100.0;  // Matches ClockDelta.py

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
CDistributedSmoothNodeBase::
CDistributedSmoothNodeBase(): _store_e(0), _dirty_e(false){
  _repository = nullptr;
  _is_ai = false;
  _ai_id = 0;

#ifdef HAVE_PYTHON
  _clock_delta = nullptr;
#endif
}

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
CDistributedSmoothNodeBase::
~CDistributedSmoothNodeBase() {
}

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::initialize
//       Access: Published
//  Description: Initializes the internal structures from some
//               constructs that are normally stored only in Python.
//               Also reads the current node's pos & hpr values in
//               preparation for transmitting them via one of the
//               broadcast_pos_hpr_*() methods.
////////////////////////////////////////////////////////////////////
void CDistributedSmoothNodeBase::
initialize(const NodePath &node_path, DCClass *dclass, CHANNEL_TYPE do_id) {
  _node_path = node_path;
  _dclass = dclass;
  _do_id = do_id;

  nassertv(!_node_path.is_empty());

  _store_xyz = _node_path.get_pos();
  _store_hpr = _node_path.get_hpr();
  _store_stop = false;
}

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::refresh_pos_hpr
//       Access: Published
//  Description: Examines the complete pos/hpr information to see
//               which of the six elements have changed, and
//               stores the info internally. You must send any
//               updates for any detected changes manually.
//               Returns flags associated with the changed values.
////////////////////////////////////////////////////////////////////
int CDistributedSmoothNodeBase::
refresh_pos_hpr() {
  // We may not be initialized yet.
  if (_node_path.is_empty()) {
    return 0;
  }

  LPoint3f xyz = _node_path.get_pos();
  LVecBase3f hpr = _node_path.get_hpr();

  int flags = 0;

  if (!IS_THRESHOLD_EQUAL(_store_xyz[0], xyz[0], smooth_node_epsilon)) {
    _store_xyz[0] = xyz[0];
    flags |= F_new_x;
  }

  if (!IS_THRESHOLD_EQUAL(_store_xyz[1], xyz[1], smooth_node_epsilon)) {
    _store_xyz[1] = xyz[1];
    flags |= F_new_y;
  }

  if (!IS_THRESHOLD_EQUAL(_store_xyz[2], xyz[2], smooth_node_epsilon)) {
    _store_xyz[2] = xyz[2];
    flags |= F_new_z;
  }

  if (!IS_THRESHOLD_EQUAL(_store_hpr[0], hpr[0], smooth_node_epsilon)) {
    _store_hpr[0] = hpr[0];
    flags |= F_new_h;
  }

  if (!IS_THRESHOLD_EQUAL(_store_hpr[1], hpr[1], smooth_node_epsilon)) {
    _store_hpr[1] = hpr[1];
    flags |= F_new_p;
  }

  if (!IS_THRESHOLD_EQUAL(_store_hpr[2], hpr[2], smooth_node_epsilon)) {
    _store_hpr[2] = hpr[2];
    flags |= F_new_r;
  }

  if (_dirty_e) {
    flags |= F_new_e;
  }

  return flags;
}
////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::send_everything
//       Access: Published
//  Description: Broadcasts the current pos/hpr in its complete form.
////////////////////////////////////////////////////////////////////
void CDistributedSmoothNodeBase::
send_everything() {
  _dirty_e = false;
  d_setSmPosHprE(_store_xyz[0], _store_xyz[1], _store_xyz[2],
                 _store_hpr[0], _store_hpr[1], _store_hpr[2],
                 _store_e);
}

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::broadcast_pos_hpr_full
//       Access: Published
//  Description: Examines the complete pos/hpr information to see
//               which of the six elements have changed, and
//               broadcasts the appropriate messages.
////////////////////////////////////////////////////////////////////
void CDistributedSmoothNodeBase::
broadcast_pos_hpr_full() {
  LPoint3f xyz = _node_path.get_pos();
  LVecBase3f hpr = _node_path.get_hpr();

  int flags = refresh_pos_hpr();

  if (flags == 0) {
    // No change.  Send one and only one "stop" message.
    if (!_store_stop) {
      _store_stop = true;
      d_setSmStop();
    }

  } else if (only_changed(flags, F_new_h)) {
    // Only change in H.
    _store_stop = false;
    d_setSmH(_store_hpr[0]);

  } else if (only_changed(flags, F_new_z)) {
    // Only change in Z.
    _store_stop = false;
    d_setSmZ(_store_xyz[2]);

  } else if (only_changed(flags, F_new_x | F_new_y)) {
    // Only change in X, Y
    _store_stop = false;
    d_setSmXY(_store_xyz[0], _store_xyz[1]);

  } else if (only_changed(flags, F_new_x | F_new_z)) {
    // Only change in X, Z
    _store_stop = false;
    d_setSmXZ(_store_xyz[0], _store_xyz[2]);

  } else if (only_changed(flags, F_new_x | F_new_y | F_new_z)) {
    // Only change in X, Y, Z
    _store_stop = false;
    d_setSmPos(_store_xyz[0], _store_xyz[1], _store_xyz[2]);

  } else if (only_changed(flags, F_new_h | F_new_p | F_new_r)) {
    // Only change in H, P, R
    _store_stop = false;
    d_setSmHpr(_store_hpr[0], _store_hpr[1], _store_hpr[2]);

  } else if (only_changed(flags, F_new_x | F_new_y | F_new_h)) {
    // Only change in X, Y, H
    _store_stop = false;
    d_setSmXYH(_store_xyz[0], _store_xyz[1], _store_hpr[0]);

  } else if (only_changed(flags, F_new_x | F_new_y | F_new_z | F_new_h)) {
    // Only change in X, Y, Z, H
    _store_stop = false;
    d_setSmXYZH(_store_xyz[0], _store_xyz[1], _store_xyz[2], _store_hpr[0]);

  } else if (flags & F_new_e) {
    // Only change in embedded
    _store_stop = false;
    send_everything();

  } else {
    // Any other change
    _store_stop = false;
    d_setSmPosHpr(_store_xyz[0], _store_xyz[1], _store_xyz[2],
                  _store_hpr[0], _store_hpr[1], _store_hpr[2]);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::broadcast_pos_hpr_xyh
//       Access: Published
//  Description: Examines only X, Y, and H of the pos/hpr information,
//               and broadcasts the appropriate messages.
////////////////////////////////////////////////////////////////////
void CDistributedSmoothNodeBase::
broadcast_pos_hpr_xyh() {
  LPoint3f xyz = _node_path.get_pos();
  LVecBase3f hpr = _node_path.get_hpr();

  int flags = 0;

  if (!IS_THRESHOLD_EQUAL(_store_xyz[0], xyz[0], smooth_node_epsilon)) {
    _store_xyz[0] = xyz[0];
    flags |= F_new_x;
  }

  if (!IS_THRESHOLD_EQUAL(_store_xyz[1], xyz[1], smooth_node_epsilon)) {
    _store_xyz[1] = xyz[1];
    flags |= F_new_y;
  }

  if (!IS_THRESHOLD_EQUAL(_store_hpr[0], hpr[0], smooth_node_epsilon)) {
    _store_hpr[0] = hpr[0];
    flags |= F_new_h;
  }

  if (flags == 0) {
    // No change.  Send one and only one "stop" message.
    if (!_store_stop) {
      _store_stop = true;
      d_setSmStop();
    }

  } else if (only_changed(flags, F_new_h)) {
    // Only change in H.
    _store_stop = false;
    d_setSmH(_store_hpr[0]);

  } else if (only_changed(flags, F_new_x | F_new_y)) {
    // Only change in X, Y
    _store_stop = false;
    d_setSmXY(_store_xyz[0], _store_xyz[1]);

  } else {
    // Any other change.
    _store_stop = false;
    d_setSmXYH(_store_xyz[0], _store_xyz[1], _store_hpr[0]);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::broadcast_pos_hpr_xy
//       Access: Published
//  Description: Examines only X and Y of the pos/hpr information,
//               and broadcasts the appropriate messages.
////////////////////////////////////////////////////////////////////
void CDistributedSmoothNodeBase::
broadcast_pos_hpr_xy() {
  LPoint3f xyz = _node_path.get_pos();

  int flags = 0;

  if (!IS_THRESHOLD_EQUAL(_store_xyz[0], xyz[0], smooth_node_epsilon)) {
    _store_xyz[0] = xyz[0];
    flags |= F_new_x;
  }

  if (!IS_THRESHOLD_EQUAL(_store_xyz[1], xyz[1], smooth_node_epsilon)) {
    _store_xyz[1] = xyz[1];
    flags |= F_new_y;
  }

  if (flags == 0) {
    // No change.  Send one and only one "stop" message.
    if (!_store_stop) {
      _store_stop = true;
      d_setSmStop();
    }

  } else {
    // Any other change.
    _store_stop = false;
    d_setSmXY(_store_xyz[0], _store_xyz[1]);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::begin_send_update
//       Access: Private
//  Description: Fills up the packer with the data appropriate for
//               sending an update on the indicated field name, up
//               until the arguments.
////////////////////////////////////////////////////////////////////
void CDistributedSmoothNodeBase::
begin_send_update(DCPacker &packer, const string &field_name) {
  DCField *field = _dclass->get_field_by_name(field_name);
  nassertv(field != nullptr);

  if (_is_ai) {

    packer.raw_pack_uint8(1);
    packer.RAW_PACK_CHANNEL(_do_id);
    packer.RAW_PACK_CHANNEL(_ai_id);
    //packer.raw_pack_uint8('A');
    packer.raw_pack_uint16(STATESERVER_OBJECT_SET_FIELD);
    packer.raw_pack_uint32(_do_id);
    packer.raw_pack_uint16(field->get_number());

  } else {
    packer.raw_pack_uint16(CLIENT_OBJECT_SET_FIELD);
    packer.raw_pack_uint32(_do_id);
    packer.raw_pack_uint16(field->get_number());
  }

  packer.begin_pack(field);
  packer.push();
}

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::finish_send_update
//       Access: Private
//  Description: Appends the timestamp and sends the update.
////////////////////////////////////////////////////////////////////
void CDistributedSmoothNodeBase::
finish_send_update(DCPacker &packer) {
#ifdef HAVE_PYTHON
  PyObject *clock_delta = PyObject_GetAttrString(_clock_delta, "delta");
  nassertv(clock_delta != nullptr);
  double delta = PyFloat_AsDouble(clock_delta);
  Py_DECREF(clock_delta);
#else
  static const double delta = 0.0f;
#endif  // HAVE_PYTHON

  double local_time = ClockObject::get_global_clock()->get_real_time();

  int network_time = (int)cfloor(((local_time - delta) * network_time_precision) + 0.5);
  // Preserves the lower NetworkTimeBits of the networkTime value,
  // and extends the sign bit all the way up.
  network_time = ((network_time + 0x8000) & 0xFFFF) - 0x8000;
  packer.pack_int(network_time);

  packer.pop();
  bool pack_ok = packer.end_pack();
  if (pack_ok) {
    Datagram dg(packer.get_data(), packer.get_length());
    _repository->send_datagram(dg);

  } else {
#ifndef NDEBUG
    if (packer.had_range_error()) {
      ostringstream error;
      error << "Node position out of range for DC file: "
            << _node_path << " pos = " << _store_xyz
            << " hpr = " << _store_hpr
            << " embedded = " << _store_e;

#ifdef HAVE_PYTHON
      string message = error.str();
      distributed_cat.warning()
        << message << "\n";
      PyErr_SetString(PyExc_ValueError, message.c_str());
#else
      nassert_raise(error.str());
#endif

    } else {
      const char *message = "Unexpected pack error in DC file.";
#ifdef HAVE_PYTHON
      distributed_cat.warning()
        << message << "\n";
      PyErr_SetString(PyExc_TypeError, message);
#else
      nassert_raise(message);
#endif
    }
#endif
  }
}

////////////////////////////////////////////////////////////////////
//     Function: CDistributedSmoothNodeBase::set_embedded_val
//       Access: Private
//  Description: published function to set current embedded value for
//               this object. It will be sent out with the next
//               telemetry broadcast.
//               We expose this because we can't infer changes in this
//               value from the contained NodePath as we can with
//               telemetry.
////////////////////////////////////////////////////////////////////
void CDistributedSmoothNodeBase::
set_embedded_val(uint64_t e) {
  if (e != _store_e) {
    _store_e = e;
    _dirty_e = true;
  }
}

uint64_t CDistributedSmoothNodeBase::
get_embedded_val() const {
  return _store_e;
}
