#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg_scale;
    // i: row
    for (int i = 0; i < height; i++)
    {
        // j: col
        for (int j = 0; j < width; j++)
        {
            avg_scale = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg_scale;
            image[i][j].rgbtGreen = avg_scale;
            image[i][j].rgbtRed = avg_scale;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp;
    // i: row
    for (int i = 0; i < height; i++)
    {
        // n is half point integer, so it only loops from 0 to mid point
        // j: col
        for (int j = 0, n = round(width / 2.0); j < n; j++)
        {
            tmp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // make a copy of original image
    RGBTRIPLE og_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            og_image[i][j] = image[i][j];
        }
    }

    // i: loop through each row in image
    for (int i = 0; i < height; i++)
    {
        // j: loop through each col
        for (int j = 0; j < width; j++)
        {
            // loop through each pixel for its 3x3 box
            // count number of pixels included in 3x3 box calculation
            int count = 0;

            // initialize RGB scales
            int blue = 0, green = 0, red = 0;

            // loop through each row in 3x3 box
            for (int k = (i - 1 > 0) ? i - 1 : 0; k < height && k <= 1 + i; k++)
            {
                // loop through each col in 3x3 box
                for (int l = (j - 1 > 0) ? j - 1 : 0; l < width && l <= 1 + j; l++)
                {
                    count++;
                    blue += og_image[k][l].rgbtBlue;
                    green += og_image[k][l].rgbtGreen;
                    red += og_image[k][l].rgbtRed;
                }
            }
            // update image with average RGB
            image[i][j].rgbtBlue = round(blue / (float) count);
            image[i][j].rgbtGreen = round(green / (float) count);
            image[i][j].rgbtRed = round(red / (float) count);
        }
    }
    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // define a black pixel for border
    RGBTRIPLE black_pixel;
    black_pixel.rgbtBlue = 0;
    black_pixel.rgbtGreen = 0;
    black_pixel.rgbtRed = 0;

    // make a copy of image with 1 pixel solid black border around the edge
    RGBTRIPLE image_black_border[height + 2][width + 2];
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || i == height + 1 || j == 0 || j == width + 1)
            {
                // add black_pixel to border
                image_black_border[i][j] = black_pixel;
            }
            else
            {
                // copy image to image_black_border
                image_black_border[i][j] = image[i - 1][j - 1];
            }
        }
    }

    // define Gx and Gy boxes
    int Gx_box[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1},
    };
    int Gy_box[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1},
    };

    // loop through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialize Gx and Gy to 0 and final value for each RGB
            int Gx_blue = 0, Gy_blue = 0, Gx_green = 0, Gy_green = 0, Gx_red = 0, Gy_red = 0;
            int blue_value, green_value, red_value;

            // through through 3x3 box to calculate Gx and Gy
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    Gx_blue += Gx_box[k][l] * image_black_border[i + k][j + l].rgbtBlue;
                    Gy_blue += Gy_box[k][l] * image_black_border[i + k][j + l].rgbtBlue;
                    Gx_green += Gx_box[k][l] * image_black_border[i + k][j + l].rgbtGreen;
                    Gy_green += Gy_box[k][l] * image_black_border[i + k][j + l].rgbtGreen;
                    Gx_red += Gx_box[k][l] * image_black_border[i + k][j + l].rgbtRed;
                    Gy_red += Gy_box[k][l] * image_black_border[i + k][j + l].rgbtRed;
                }
            }

            // final value using formula: square root of Gx^2 + Gy^2
            blue_value = round(sqrt((float)pow(Gx_blue, 2) + pow(Gy_blue, 2)));
            green_value = round(sqrt((float)pow(Gx_green, 2) + pow(Gy_green, 2)));
            red_value = round(sqrt((float)pow(Gx_red, 2) + pow(Gy_red, 2)));
            // rewrite image with new RBG color, max 255
            image[i][j].rgbtBlue = (blue_value <= 255) ? blue_value : 255;
            image[i][j].rgbtGreen = (green_value <= 255) ? green_value : 255;
            image[i][j].rgbtRed = (red_value <= 255) ? red_value : 255;
        }
    }
    return;
}