#include "note.h"

namespace synthetisens {
  double note_freq[] = {
    130.81, // C
    138.59, // C#
    146.83, // D
    155.56, // D#
    164.81, // E
    174.61, // F
    185.0, // F#
    196.0, // G
    207.65, // G#
    220.0, // A
    233.08, // A#
    246.94, // B
  };

  key_type key_types[] = {
    WHITE_KEY, // C
    BLACK_KEY, // C#
    WHITE_KEY, // D
    BLACK_KEY, // D#
    WHITE_KEY, // E
    WHITE_KEY, // F
    BLACK_KEY, // F#
    WHITE_KEY, // G
    BLACK_KEY, // G#
    WHITE_KEY, // A
    BLACK_KEY, // A#
    WHITE_KEY, // B
  };
}
