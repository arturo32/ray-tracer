#include <iostream>  // std::cout, std::cerr
#include <fstream>
using std::ifstream;
#include <sstream>
using std::ostringstream;
#include <string>
using std::string;

#include "../core/api.hpp"
#include "../core/rt3.hpp"

using namespace rt3;

/// Lambda expression that returns a lowercase version of the input string.
auto CSTR_LOWERCASE = [](const char *t) -> std::string {
  std::string str{t};
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
};

void usage(const char *msg = nullptr)
{
  if (msg)
    std::cerr << "RT3: " << msg << "\n\n";

  std::cerr << "Usage: rt3 [<options>] <input_scene_file>\n"
            << "  Rendering simulation options:\n"
            << "    --help                     Print this help text.\n"
            << "    --cropwindow <x0,x1,y0,y1> Specify an image crop window.\n"
            << "    --quick                    Reduces quality parameters to "
               "render image quickly.\n"
            << "    --outfile <filename>       Write the rendered image to "
               "<filename>.\n\n";
  exit(msg ? 1 : 0);
}

int main(int argc, char *argv[])
{
  RunningOptions opt;  // Stores incoming arguments.

  // ================================================
  // (1) Validate command line arguments.
  // ================================================
  if (argc == 1)
    usage();  // No input arguments, send default message to the user, and exit.

  // Prepare to parse input argumnts.
  std::ostringstream oss;
  for (int i{ 1 }; i < argc; ++i) {
    std::string option = CSTR_LOWERCASE(argv[i]);
    if (option == "--cropwindow" or option == "-cropwindow" or option == "--crop"
        or option == "-crop") {
      if (i + 4 >= argc)  // The option's argument is missing.
        usage("missing value after --cropwindow argument");
      // Get crop values.
      opt.crop_window[0][0] = std::stof(argv[++i]);
      opt.crop_window[0][1] = std::stof(argv[++i]);
      opt.crop_window[1][0] = std::stof(argv[++i]);
      opt.crop_window[1][1] = std::stof(argv[++i]);
    } else if (option == "--outfile" or option == "-outfile" or option == "-o") {
      if (i + 1 == argc)  // The option's argument is missing.
        usage("missing value after --outfile argument");
      // Get output image file name.
      opt.outfile = std::string{ argv[++i] };
    } else if (option == "--quickrender" or option == "-quickrender" or option == "-q"
               or option == "--quick" or option == "-quick") {
      opt.quick_render = true;
    } else if (option == "--help" or option == "-help" or option == "-h") {
      usage();
    } else {
      opt.filename = std::string(argv[i]);
      // Stream object to handle the input file.
      std::ifstream scene_file_ifs{ opt.filename };
      // Check whether we had problem opening input level file.
      if (!scene_file_ifs.is_open()) {
        std::ostringstream oss;
        oss << "Sorry, could not open level file [" << opt.filename << "].\n";
        RT3_ERROR(oss.str());
      }
      scene_file_ifs.close();
    }
  }  // for to traverse the argument list.

  // ================================================
  // (2) Welcome message
  // ================================================
  RT3_MESSAGE(std::string{ " +=========================================+\n" }
              + std::string{ " |  Ray Tracer Teaching Tool -- RT3, v1.0  |\n" }
              + std::string{ " |  copyright DIMAp/UFRN 2017-2020.        |\n" }
              + std::string{ " +=========================================+\n\n" });

  // ================================================
  // (3) Initialize the renderer engine and load a scene.
  // ================================================
  API::init_engine(opt);
  API::run();
  API::clean_up();

  RT3_MESSAGE("\n    Thanks for using RT3!\n\n");

  return EXIT_SUCCESS;
}
