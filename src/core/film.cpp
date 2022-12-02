#include "film.hpp"

#include <cmath>

#include "api.hpp"
#include "image_io.hpp"
#include "paramset.hpp"

namespace rt3 {

//=== Film Method Definitions
Film::Film(const Point2i &resolution, const std::string &filename,
		   image_type_e imgt, std::vector<real_type> crop,
		   bool gamma_corrected, real_type gamma)
		: m_full_resolution{resolution}, m_filename{filename}, m_image_type{imgt},
		m_color_buffer_ptr{std::make_unique<unsigned char[]>(resolution.x()*resolution.y()*3)},
		gamma_corretion{gamma_corrected}, igamma{1/gamma} {
			crop_window[0][0] = crop.at(0);
			crop_window[0][1] = crop.at(1);
			crop_window[1][0] = crop.at(2);
			crop_window[1][1] = crop.at(3);
}

Film::~Film() {}

/// Add the color to image.
void Film::add_sample(const Point2i &pixel_coord, const ColorXYZ &pixel_color) {
	uint pos = 3*((pixel_coord.y() * m_full_resolution.x()) + pixel_coord.x());
	m_color_buffer_ptr.get()[pos] = pixel_color.x() ;
	m_color_buffer_ptr.get()[pos+1] = pixel_color.y();
	m_color_buffer_ptr.get()[pos+2] = pixel_color.z();
}

/// Convert image to RGB, compute final pixel values, write image.
void Film::write_image(void) const {
	switch (m_image_type)
	{
	case Film::image_type_e::PNG:
		save_png(m_color_buffer_ptr.get(),
						 m_full_resolution.x(), m_full_resolution.y(), 3,
						 m_filename+".png");
		break;
	case Film::image_type_e::PPM3:
		save_ppm3(m_color_buffer_ptr.get(),
						 m_full_resolution.x(), m_full_resolution.y(), 3,
						 m_filename+".ppm");
		break;
	case Film::image_type_e::PPM6:
		save_ppm6(m_color_buffer_ptr.get(),
						 m_full_resolution.x(), m_full_resolution.y(), 3,
						 m_filename+".ppm");
		break;
	default:
		save_png(m_color_buffer_ptr.get(),
						 m_full_resolution.x(), m_full_resolution.y(), 3,
						 m_filename+".png");
		break;
	}
}

// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Film object.
Film *create_film(const ParamSet &ps) {
	std::cout << ">>> Inside create_film()\n";
	std::string filename;
	// Let us check whether user has provided an output file name via
	// command line arguments in main().
	if (API::curr_run_opt.outfile != "") {
		// Retrieve filename defined via CLI.
		filename = API::curr_run_opt.outfile;
		// Retrieve another filename, now from the ParamSet object.
		std::string filename_from_file = retrieve(ps, "filename", std::string{""});
		if (filename_from_file != "")  // We also get a filename from scene file...
			RT3_WARNING(string{"Output filename supplied on command line, \""} +
									API::curr_run_opt.outfile +
									string{"\" is overriding filename provided in scene "
												 "description file, \""} +
									filename_from_file);
	} else {
		// Try yo retrieve filename from scene file.
		filename = retrieve(ps, "filename", std::string{"image.png"});
	}

	// Read resolution.
	// Aux function that retrieves info from the ParamSet.
	int xres = retrieve(ps, "x_res", int(1280));
	// Aux function that retrieves info from the ParamSet.
	int yres = retrieve(ps, "y_res", int(720));
	// Quick render?
	if (API::curr_run_opt.quick_render == true) {
		// decrease resolution.
		xres = std::max(1u, xres / 4u);
		yres = std::max(1u, yres / 4u);
	}

	std::string img_type = retrieve(ps, "img_type", std::string{"png"});

	Film::image_type_e type;
	if (img_type == "png") {
		type = Film::image_type_e::PNG;
	} else if (img_type == "ppm3") {
		type = Film::image_type_e::PPM3;
	} else if (img_type == "ppm6") {
		type = Film::image_type_e::PPM6;
	} else {
		type = Film::image_type_e::PNG;
		RT3_WARNING(string{"The output image type"}+ "" + 
								"is not supported. P");
	}
	
	// TODO
	// Read crop window information.
	std::vector<real_type> crop_window = retrieve(ps, "crop_window", std::vector<real_type>{0,0,0,0});
	std::cout << crop_window << std::endl;

	std::string gamma_corrected = retrieve(ps, "gamma_corrected", std::string{"false"});
	real_type gamma = retrieve(ps, "gamma", 2.2);

	// Note that the image type is fixed here. Must be read from ParamSet, though.
	return new Film(Point2i{xres, yres}, filename, type, crop_window, gamma_corrected=="true", gamma);
}
}  // namespace rt3
