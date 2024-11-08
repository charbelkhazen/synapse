#include "functions.h"
#include <iostream>
#include <stdio.h>
#include <jpeglib.h>
#include <cstdio>
#include <cstdlib>
#include <filesystem>

void compress_jpeg(const std::string& input_file, const std::string& output_filename, int quality) {
    // Create the output directory if it doesn't exist
    const std::string output_dir = "../../output/";
    if (!std::filesystem::exists(output_dir)) {
        std::filesystem::create_directory(output_dir);
    }

    // Construct the full path for the output file
    std::string output_file_path = output_dir + output_filename;

    // Open the input file
    FILE* infile = fopen(input_file.c_str(), "rb");
    if (!infile) {
        perror("Can't open input file");
        return;
    }

    // Decompress structure and error handling
    jpeg_decompress_struct cinfo;
    jpeg_compress_struct cinfo_out;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    // Buffer to hold scanlines
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)(
        (j_common_ptr)&cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, 1);

    // Create the output file in the output directory
    FILE* outfile = fopen(output_file_path.c_str(), "wb");
    if (!outfile) {
        perror("Can't open output file");
        fclose(infile); // Close infile before returning
        return;
    }

    // Setup compression
    cinfo_out.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, outfile);

    cinfo_out.image_width = cinfo.output_width;
    cinfo_out.image_height = cinfo.output_height;
    cinfo_out.input_components = cinfo.output_components;
    cinfo_out.in_color_space = cinfo.out_color_space;

    // Set compression parameters
    jpeg_set_defaults(&cinfo_out);
    jpeg_set_quality(&cinfo_out, quality, TRUE);
    jpeg_start_compress(&cinfo_out, TRUE);

    // Read and write scanlines
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        jpeg_write_scanlines(&cinfo_out, buffer, 1);
    }

    // Finish compression and decompression
    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(infile);
    fclose(outfile);
}
