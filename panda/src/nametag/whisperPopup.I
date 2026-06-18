// Filename: whisperPopup.I
// Created by:  drose (25Jul01)
//
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//     Function: WhisperPopup::WhisperColor::get_text_color
//       Access: Published
//  Description: Returns the text (foreground) color for the indicated
//               button state.
////////////////////////////////////////////////////////////////////
INLINE const LColorf &WhisperPopup::WhisperColor::
get_text_color(PGButton::State state) const {
  nassertr((int)state >= 0 && (int)state < num_button_states, _text[0]);
  return _text[state];
}

////////////////////////////////////////////////////////////////////
//     Function: WhisperPopup::WhisperColor::get_balloon_color
//       Access: Published
//  Description: Returns the balloon (background) color for the
//               indicated button state.
////////////////////////////////////////////////////////////////////
INLINE const LColorf &WhisperPopup::WhisperColor::
get_balloon_color(PGButton::State state) const {
  nassertr((int)state >= 0 && (int)state < num_button_states, _balloon[0]);
  return _balloon[state];
}
