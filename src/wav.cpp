#include "wav.h"
#include "component.h"
#include "gtkmm/filechooserdialog.h"
#include "window.h"
#include <iostream>
#include <sndfile.h>

extern synthetisens::Window* window;
extern synthetisens::component* speaker;

void save_speaker_wav() {
  Gtk::FileChooserDialog* dialog = new Gtk::FileChooserDialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog->set_transient_for(*window);
  dialog->set_modal(true);
  dialog->set_position(Gtk::WIN_POS_CENTER);
  dialog->add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog->add_button("_Save", Gtk::RESPONSE_OK);

  int result = dialog->run();
  dialog->close();

  if (result != Gtk::RESPONSE_OK) {
    return;
  } 

  synthetisens::signal* sig = speaker->generate_output(0).value.signal;

  string filename = dialog->get_filename();
  
  //Save the signal into the wav file
  SF_INFO sfinfo;
  sfinfo.samplerate = SAMPLE_FREQ;
  sfinfo.channels = 1;
  sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
  
  SNDFILE* sndfile = sf_open(filename.data(), SFM_WRITE, &sfinfo);
  if (!sndfile) {
    std::cerr << "Impossible d'ouvrir le fichier : " << filename << std::endl;
  }

  sf_count_t frames_written = sf_writef_double(sndfile, sig->get_values(), sig->size);
  sf_close(sndfile);
}

synthetisens::signal& load_wav_signal() {
    Gtk::FileChooserDialog* dialog = new Gtk::FileChooserDialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog->set_transient_for(*window);
    dialog->set_modal(true);
    dialog->set_position(Gtk::WIN_POS_CENTER);
    dialog->add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog->add_button("_Open", Gtk::RESPONSE_OK);
    
    int result = dialog->run();
    dialog->close();

    if (result != Gtk::RESPONSE_OK) {
      return *(new synthetisens::signal());
    } 

    std::string filename = dialog->get_filename();
    SF_INFO sfinfo;
    SNDFILE* sndfile = sf_open(filename.data(), SFM_READ, &sfinfo);

    if (!sndfile) {
        std::cerr << "Impossible d'ouvrir le fichier : " << filename << std::endl;
    }
    
    const int framecount = sfinfo.frames;
    const int channels = sfinfo.channels;
    const int buffer_size = framecount * channels;
    double* buffer = new double[buffer_size];
    sf_readf_double(sndfile, buffer, buffer_size);
    sf_close(sndfile);

    double* nbuf = new double[framecount];
    for(int i = 0; i < framecount; i++){
      for( int j = 0; j < channels; j++){
        nbuf[i] = nbuf[i] + buffer[i*channels + j]; 
      }
    }

    synthetisens::signal* output_signal = new synthetisens::signal(framecount, nbuf, false);
    synthetisens::signal* noutput_signal = &change_samplerate(*output_signal, sfinfo.samplerate, SAMPLE_FREQ);
   return *noutput_signal;
}
