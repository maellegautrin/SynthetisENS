#ifndef _NOTE_H
#define _NOTE_H

namespace synthetisens {

  enum key {
    C,
    Cs,
    D,
    Ds,
    E,
    F,
    Fs,
    G,
    Gs,
    A,
    As,
    B,

    KEY_COUNT
  };

  enum key_type {
    WHITE_KEY,
    BLACK_KEY
  };

  extern double note_freq[];

  extern key_type key_types[];

}

#endif // !_NOTE_H
