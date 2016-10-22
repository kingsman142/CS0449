#include <stdio.h>
#include <string.h>
#include <math.h>

#pragma pack(1)

//Image header
struct header{
	short format_identifier;
	int file_size;
	short reserved_value;
	short reserved_value_2;
	int pixel_array_offset;
} image_header;

//Device-independent bitmap
struct dib{
	int header_size;
	int image_width;
	int image_height;
	short num_color_panes;
	short num_bits_per_pixel;
	int compression_scheme;
	int image_size;
	int horizontal_resolution;
	int vertical_resolution;
	int num_palette_colors;
	int num_important_colors;
} image_dib;

//Pixel struct for the image
struct pixel{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

//Command-line argument [0]: image transformation
//Command-line argument [1]: image filename
int main(int argc, char* argv[]){	
	FILE * image = fopen(argv[2], "rb+");
	
	//Checks whether the file exists or not
	if(image == 0){
		printf("Image is a null pointer! Oh no! Exiting now...\n");
		return 1;
	}

	int i;
	int j;

	//Read in the two structs, one at a time.
	fread(&image_header, 14, 1, image);
	fread(&image_dib, 40, 1, image);

	//Important information to keep for later for comparisons.
	//They need to be cast to char pointers because you can't 
	//pass an int into strcmp.
	char * identifier[2];
	identifier[0] = (char *) image_header.format_identifier;
	identifier[1] = (char *) (image_header.format_identifier >> 8);
	
	//Check if there are any errors with the format_identifier, header size, or number of bits per pixel.
	if(strcmp(identifier, "BM") != 0 || image_dib.header_size != 40 || image_dib.num_bits_per_pixel != 24){
		if(image_dib.header_size != 40) printf("Size of DIB is not 40!\n");
		if(image_dib.num_bits_per_pixel != 24) printf("Whoops, the num bits per pixel is %d!\n", image_dib.num_bits_per_pixel);
		if(strcmp(identifier, "BM") != 0) printf("The identifier is not BM.  It's actually %s with size of %d!\n", (char *) image_header.format_identifier, sizeof(image_header.format_identifier));
		printf("That file format is not supported, sorry!\n");
		return 1;
	}
	
	fseek(image, image_header.pixel_array_offset, SEEK_SET);	
	int extra_image_padding = 4 - (image_dib.image_width*3 % 4);
	
	//If the image width is a multiple of 4, there should not be a padding of 4 bytes, it should be 0 bytes.
	if(extra_image_padding == 4){
		extra_image_padding = 0;
	}	

	//Check to make sure the image transformation flag is valid.
	if(strcmp(argv[1], "-invert") == 0){
		printf("Invert beginning...\n");
	} else if(strcmp(argv[1], "-grayscale") == 0){
		printf("Grayscale beginning...\n");
	} else{
		printf("The first command-line argument was %s. This is invalid. Only accepted transformations are -invert and -grayscale. Exiting...\n", argv[1]);
		return 1;
	}
	
	//Loop th rough all pixels in the image.
	for(i = 0; i < image_dib.image_height; i++){
		for(j = 0; j < image_dib.image_width; j++){
			struct pixel p;
			fread(&p, 3, 1, image);
			
			//Invert the pixel
			if(strcmp("-invert", argv[1]) == 0){
				p.blue = ~p.blue;
				p.green = ~p.green;
				p.red = ~p.red;
			} else if(strcmp("-grayscale", argv[1]) == 0){ //Grayscale the pixel
				double normalized_blue = ((double) p.blue)/255.0;
				double normalized_green = ((double) p.green)/255.0;
				double normalized_red = ((double) p.red)/255.0;
				double grey_value = .2126*normalized_red + .7152*normalized_green + .0722*normalized_blue;
				
				if(grey_value <= .0031308){
					normalized_blue = 12.92*grey_value;
					normalized_green = 12.92*grey_value;
					normalized_red = 12.92*grey_value;
				} else{
					normalized_blue = 1.055*pow(grey_value, 1.0/2.4) - .055;
					normalized_green = 1.055*pow(grey_value, 1.0/2.4) - .055;
					normalized_red = 1.055*pow(grey_value, 1.0/2.4) - .055;
				}

				p.blue = (unsigned char) (normalized_blue*255.0);
				p.green = (unsigned char) (normalized_green*255.0);
				p.red = (unsigned char) (normalized_red*255.0);
			}
			
			fseek(image, -3, SEEK_CUR); 
			fwrite(&p, 3, 1, image); //Write the pixel back to its original image
		}
		
		//Make sure the row of pixels is a multiple of 4 bytes, so add some padding if that's not the case.
		fseek(image, extra_image_padding, SEEK_CUR);
	}

	printf("Finished!\n");

	return 1;
}
